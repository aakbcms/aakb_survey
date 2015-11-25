<?php
/**
 * @file
 *
 */

function aakb_survey_admin_status() {
  return t('We need to find something for this page?');
}

/**
 * Administration form for mail templates.
 *
 * @return array
 *   The generated form.
 */
function aakb_survey_admin_template_form() {
  $form = array(
    '#tree' => TRUE,
  );

  $default = variable_get('aakb_survey_template', array());

  $form['aakb_survey_template'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail template'),
    '#description' => t('Template used to send the invite mail to all users in the import.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['aakb_survey_template']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail title'),
    '#description' => t('The mails title'),
    '#default_value' => isset($default['title']) ? $default['title'] : '',
    '#required' => TRUE,
  );

  $form['aakb_survey_template']['reply'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply mail address'),
    '#description' => t('The mail address that is show as reply to in the mail'),
    '#default_value' => isset($default['reply']) ? $default['reply'] : '',
    '#required' => TRUE,
  );

  $form['aakb_survey_template']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t('The content for the mail. Tokens available [link], [mail]'),
    '#default_value' => isset($default['body']) ? $default['body'] : '',
    '#required' => TRUE,
    '#rows' => 15,
  );

  return system_settings_form($form);
}

/**
 * Administration form for mail templates.
 *
 * @return array
 *   The generated form.
 */
function aakb_survey_admin_landing_form() {
  $form = array(
    '#tree' => TRUE,
  );

  $default = variable_get('aakb_survey_landing_page', array());

  $form['aakb_survey_landing_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Landing pages'),
    '#description' => t('Templates used to display the different landing pages.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['aakb_survey_landing_page']['verify'] = array(
    '#type' => 'textarea',
    '#title' => t('Acceptance page'),
    '#description' => t('The text displayed when the user uses the mail link. Tokens available [yes_link], [no_link], [checkboxes]'),
    '#default_value' => isset($default['verify']) ? $default['verify'] : '',
    '#required' => TRUE,
    '#rows' => 15,
  );

  $form['aakb_survey_landing_page']['thanks'] = array(
    '#type' => 'textarea',
    '#title' => t('Thanks page'),
    '#description' => t('The page thanking for accepting. Tokens available [name]'),
    '#default_value' => isset($default['body']) ? $default['body'] : '',
    '#required' => TRUE,
    '#rows' => 15,
  );

  $form['aakb_survey_landing_page']['no_thanks'] = array(
    '#type' => 'textarea',
    '#title' => t('No thanks page'),
    '#description' => t('No thanks page.'),
    '#default_value' => isset($default['body']) ? $default['body'] : '',
    '#required' => TRUE,
    '#rows' => 15,
  );


  return system_settings_form($form);
}

function aakb_survey_admin_export_form() {
  $form = array();
  return system_settings_form($form);
}


function aakb_survey_admins_send_form() {
  $form = array();
  return system_settings_form($form);
}