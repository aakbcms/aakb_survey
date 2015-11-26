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
    '#content' => isset($content['verify']) ? $content['verify'] : t('Please configure the module Aakb survey first.'),
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
  return array(
    '#theme' => 'aakb_survey_landing_page',
    '#content' => isset($content['no_thanks']) ? $content['no_thanks'] : t('Please configure the module Aakb survey first.'),
  );
}

/**
 * Thanks menu callback.
 *
 * @return array
 *   Render array with page content.
 */
function aakb_survey_user_consent_thanks() {
  return array(
    '#theme' => 'aakb_survey_landing_page',
    '#content' => isset($content['thanks']) ? $content['thanks'] : t('Please configure the module Aakb survey first.'),
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
  );

  $types = _aakb_survey_consent_types();
  foreach ($types as $type => $info) {
    $form['consents'][$type] = array(
      '#type' => 'checkbox',
      '#title' => $info['title'],
      '#description' => $info['description'],
    );
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, submit my selections'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('No, thanks'),
    '#href' => 'survey/'. $hash .'/consent/no',
  );

  $form['hash'] = array(
    '#type' => 'value',
    '#value' => $hash,
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * Handles user consent form selections and redirect to thanks page.
 */
function _aakb_survey_consent_form_submit($form, $form_state) {
  $values = $form_state['values'];

  // Reduce the consent selections to a bitmap.
  $consents = array_keys(array_filter($values['consents']));
  $consent = array_reduce($consents, function($carry, $item) {
    return $carry | $item;
  }, 0);

  // Update the database with the consent.
  db_update('aakb_survey')
    ->fields(array(
      'consent' => $consent,
    ))
    ->condition('hash', $values['hash'])
    ->execute();

  // @TODO: Why don't from redirect work?
  drupal_goto('/survey/'. $values['hash'] .'/consent/thanks');
}
