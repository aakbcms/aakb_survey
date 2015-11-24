<?php
/**
 * @file
 *
 */

/**
 * Implements hook_drush_command().
 */
function aakb_survey_drush_command() {
  return array(
    'aakb-survey-import' => array(
      'description' => 'Import mails into the database',
      'arguments' => array(
        'file' => 'The file to import from',
        'delimiter' => 'The char used to delimit the values',
      ),
      'options' => array(),
      'aliases' => array('aakb-si'),
      'callback' => 'drush_aakb_survey_import',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    ),
  );
}

/**
 * Import mails based on file.
 *
 * @param string $file
 *   The file to import.
 * @param string $delimiter
 *   The char used to separate the values.
 */
function drush_aakb_survey_import($file = 'mails.csv', $delimiter = ',') {
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

  // Holds mails that fails (all ready exists).
  $fails = array();

  if (($handle = fopen($file, "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, $delimiter)) !== FALSE) {
      $uid = NULL;
      // Check if this mail have a count of only one (unique mail in the
      // import).
      if ($data[1] == 1) {
        // Try to load the Drupal user.
        if ($user = user_load_by_mail($data[0])) {
          $uid = $user->uid;
        }
      }

      // Check if mail exists.
      $id = db_select('aakb_survey', 's')
        ->fields('s', array('id'))
        ->condition('mail', $data[0])
        ->execute()->fetchField();

      if (!$id) {
        // Save record into the database.
        $data = array(
          'uid' => $uid,
          'mail' => $data[0],
          'hash' => substr(user_hash_password($data[0] . $uid), 3),
          'notified' => 0,
          'notified_time' => 0,
          'consent' => AAKB_SURVEY_NO_CONSENT,
        );
        drupal_write_record('aakb_survey', $data);
      }
      else {
        $fails[$id] = $data[0];
      }

      echo '.';
    }
    fclose($handle);

    echo "\n";

    // Return failed.
    foreach ($fails as $id => $mail) {
      echo $id . ' -> ' . $mail . "\n";
    }
  }
}
