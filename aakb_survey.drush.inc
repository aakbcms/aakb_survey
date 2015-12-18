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
    'aakb-survey-mail-send' => array(
      'description' => "Send queued emails.",
      'options' => array(
        'items' => array(
          'description' => 'The number of mails to send each minute.',
          'example-value' => 30,
          'value' => 'required',
        ),
      ),
      'aliases' => array('aakb-ssm'),
      'drupal dependencies' => array(
        'queue_mail',
      ),
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
          // Randomness is added to hash to ensure that no body can guess it
          // from knowing another users mail address.
          'hash' => sha1($data[0] . rand()),
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

/**
 * Send mails based on currently queued mails.
 */
function drush_aakb_survey_mail_send() {
  $sent_mails_count = 0;

  // Prevent session information from being saved while queue is running.
  drupal_save_session(FALSE);

  // Force the current user to anonymous to ensure consistent permissions on
  // queue runs.
  $original_user = $GLOBALS['user'];
  $GLOBALS['user'] = drupal_anonymous_user();

  // Grab the defined cron queues.
  $queues = module_invoke_all('cron_queue_info');
  drupal_alter('cron_queue_info', $queues);

  // And fetch and process our queue.
  $queue_name = QUEUE_MAIL_QUEUE_NAME;
  if (isset($queues[$queue_name])) {
    $info = $queues[$queue_name];
    $function = $info['worker callback'];
    $items = drush_get_option('items', 0);

    // Calculate sleep time.
    $interval = round(60 / $items);

    $queue = _queue_mail_get_queue();
    while ($item = $queue->claimItem()) {
      _aakb_survey_send_queue_mail($function, $item->data);
      $queue->deleteItem($item);
      echo '.';
      sleep($interval);
      $sent_mails_count++;
    }
  }

  echo "\n";

  // Restore the user.
  $GLOBALS['user'] = $original_user;
  drupal_save_session(TRUE);

  drush_log(dt('Processed @count queued items.', array('@count' => $sent_mails_count)), 'ok');
}