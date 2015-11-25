<?php
/**
 * @file
 *
 */

/**
 * Implements hook_views_data().
 */
function aakb_survey_views_data() {
  $data = array();

  $data['aakb_survey']['table']['group'] = t('Aakb survey');
  $data['aakb_survey']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Aakb survey'),
    'help' => t('Contains survey consent data.'),
  );

  // Join with users table.
  $data['aakb_survey']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Node ID table field.
  $data['aakb_survey']['uid'] = array(
    'title' => t('User id'),
    'help' => t('The users selected consent information.'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Survey consent relationship'),
      'title' => t('Survey consent information'),
      'help' => t('The users selected consent information.'),
    ),
  );

  // The fields.
  $data['aakb_survey']['id'] = array(
    'title' => t('Consent id'),
    'help' => t('The consent record id.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['aakb_survey']['consent'] = array(
    'title' => t('Consent'),
    'help' => t('The consent bitmap.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'consent',
    ),
  );

  return $data;
}