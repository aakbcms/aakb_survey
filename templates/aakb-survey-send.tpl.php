<?php
/**
 * @file
 * Default template for Aakb survey send page.
 */
?>
<h3>Send invitation mail's</h3>
<p>Users that have not be notified: <strong><?php print $users; ?></strong></p>
<?php if ($show_send) : ?>
  <input class='js-aakb-survey-sure' type="checkbox" name="sure" value="yes"> I'am sure that i want to send mails.
  <div class="form-actions form-wrapper" id="edit-actions">
    <input id="edit-actions-submit" value="Send invitation mails" class="form-submit form-button-disabled js-aakb-survey-send">
  </div>
<?php endif; ?>

