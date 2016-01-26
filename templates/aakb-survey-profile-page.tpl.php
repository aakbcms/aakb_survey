<?php
/**
 * @file
 * Default template for aakb survey user profile page.
 */
?>
<div class="entity clearfix">
  <h2><?php print t('Participation in surveys') ?></h2>

  <div class="content">
    <?php foreach ($types as $title => $info) : ?>
      <div class="field field-type-list-text field-label-inline clearfix">
          <div class="field-label"><?php print $title?>:&nbsp;</div>
          <div class="field-items">
            <div class="field-item"><?php print $info['description'] ?></div>
          </div>
      </div>
    <?php endforeach; ?>
  </div>
</div>

