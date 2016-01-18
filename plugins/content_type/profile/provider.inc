<?php
/**
 * @file
 * Wrappers profile2 provider profile into a panels pane.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('AAKB Survey consents'),
  'description' => t('Display consent selections.'),
  'single' => TRUE,
  'render callback' => 'aakb_survey_profile_provider_type_render',
  'required context' => new ctools_context_required(t('User'), 'user'),
  'category' => t('User'),
);

/**
 * Render callback function.
 */
function aakb_survey_profile_provider_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->content = '';

  if (isset($context->data)) {
    $account = $context->data;

    // Load user consents.
    $consents = db_select('aakb_survey', 'aas')
      ->fields('aas', array('consent'))
      ->condition('uid', $account->uid, '=')
      ->execute()
      ->fetchField();



    $types = _aakb_survey_consent_types();
    $data = array();
    foreach ($types as $type => $info) {
      if ($consents & $type) {
        $data[$info['title']] = array(
          'description' => $info['description'],
        );
      }
    }

    $block->content = array(
      '#theme' => 'aakb_survey_profile_page',
      '#types' => $types,
    );
  }

  return $block;
}

/**
 * Plugin settings form.
 */
function aakb_survey_profile_provider_edit_form($form, &$form_state) {
  return $form;
}
