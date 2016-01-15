<?php
/**
 * @file
 * The pages directed to the end-users.
 */

/**
 * Landing page for the link sent to users.
 *
 * @param string $hash
 *   Hash to ID the users.
 *
 * @return array
 *   Render array with the pages content.
 */
function aakb_survey_user_consent($hash) {
  // Select the user from the database.
  $data = db_select('aakb_survey', 's')
    ->fields('s', array('uid', 'consent'))
    ->condition('hash', $hash, '=')
    ->execute()->fetchAssoc();

  // If hash value not found redirect to page not found.
  if (!$data) {
    return MENU_NOT_FOUND;
  }

  // If uid is set in the table the user is identified and an direct link to the
  // "thanks" pages can be used.
  $login_required = FALSE;
  if (is_null($data['uid'])) {
    $login_required = TRUE;
  }

  $content = variable_get('aakb_survey_landing_page', array());

  return array(
    '#theme' => 'aakb_survey_landing_page',
    '#content' => isset($content['verify']) ? str_replace("\r", '<br />', $content['verify']) : t('Please configure the module Aakb survey first.'),
    '#login_required' => $login_required,
    '#form' => drupal_get_form('_aakb_survey_consent_form', $hash),
  );
}

/**
 * No thanks menu callback.
 *
 * @return array
 *   Render array with page content.
 */
function aakb_survey_user_consent_no() {
  $content = variable_get('aakb_survey_landing_page', array());

  return array(
    '#theme' => 'aakb_survey_landing_page',
    '#content' => isset($content['no_thanks']) ? str_replace("\r", '<br />', $content['no_thanks']) : t('Please configure the module Aakb survey first.'),
  );
}

/**
 * Thanks menu callback.
 *
 * @return array
 *   Render array with page content.
 */
function aakb_survey_user_consent_thanks() {
  $content = variable_get('aakb_survey_landing_page', array());

  return array(
    '#theme' => 'aakb_survey_landing_page',
    '#content' => isset($content['thanks']) ? str_replace("\r", '<br />', $content['thanks']) : t('Please configure the module Aakb survey first.'),
  );
}

/**
 * Implements hook_form().
 *
 * The user consent form to display on the landing page.
 */
function _aakb_survey_consent_form($form, $form_state, $hash) {
  $form = array(
    'consents' => array(
      '#tree' => TRUE,
    ),
    '#attributes' => array(
      'class' => array('aakb-survey-consent-form'),
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'aakb_survey') . '/css/aakb_survey.css',
      ),
    ),
  );

  $types = _aakb_survey_consent_types();
  foreach ($types as $type => $info) {
    $form['consents'][$type] = array(
      '#type' => 'checkbox',
      '#title' => $info['title'],
      '#description' => $info['description'],
    );
  }

  drupal_add_library('system', 'drupal.ajax');
  $form['actions']['submit'] = array(
    '#theme' => 'link',
    '#text' => t('Send'),
    '#path' => 'survey/' . $hash . '/consent/ajax',
    '#options' => array(
      'attributes' => array(
        'class' => array(
          'form-submit',
          'use-ajax',
        ),
      ),
      'html' => FALSE,
    ),
  );


  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('No, thanks'),
    '#href' => 'survey/'. $hash .'/consent/no',
    '#options' => array(
      'attributes' => array(
        'class' => array(
          'form-cancel',
        ),
      ),
    ),
  );

  $form['hash'] = array(
    '#type' => 'value',
    '#value' => $hash,
  );

  // Check if the logged in user is a library user.
  $info = _aakb_survey_get_user_consent($hash);
  if ($info['uid'] > 0) {
    $user = user_load($info['uid']);
  }
  else {
    global $user;
  }

  // Below can't use user_is_logged_in() as the user may not be global loaded.
  if ($user->uid > 0 && ding_user_is_provider_user($user) && !empty($form_state['input'])) {
    $values = $form_state['input'];

    // Reduce the consent selections to a bitmap.
    $consent = 0;
    if (!empty($values['consents'])) {
      $consents = array_keys($values['consents']);
      $consent = array_reduce($consents, function ($carry, $item) {
        return $carry | $item;
      }, 0);
    }

    // Update the database with the consent.
    db_update('aakb_survey')
      ->fields(array(
        'uid' => $user->uid,
        'consent' => $consent,
      ))
      ->condition('hash', $hash)
      ->execute();

    // @TODO: Why don't from redirect work?
    drupal_goto('/survey/'. $hash .'/consent/thanks');
  }

  return $form;
}

/**
 * Consent ajax menu callback.
 */
function aakb_survey_user_consent_ajax($hash) {
  $commands = array();

  // Check if the logged in user is a library user.
  global $user;
  if (!user_is_logged_in()) {

    // Check if user information can be store without login. If user don't have
    // a uid the user needs to login.
    $info = _aakb_survey_get_user_consent($hash);
    if (!$info['uid']) {
      // Trigger log-in (the reservation link will be triggered on success).
      $commands[] = ajax_command_ding_user_authenticate('');
    }
    else {
      // Submit the consent form.
      $commands[] = ajax_command_invoke('.aakb-survey-consent-form', 'submit');
    }
  }
  elseif (!ding_user_is_provider_user($user)) {
    // Error not library user.
    $commands[] = ajax_command_ding_popup('aakb_survey', t('Error'), '<p>' . t('Only library user can can set survey consents.') . '</p>');
  }
  else {
    // Submit the consent form.
    $commands[] = ajax_command_invoke('.aakb-survey-consent-form', 'submit');
  }

  // Return the ajax commands as an render array.
  return array('#type' => 'ajax', '#commands' => $commands);
}
