<?php
/**
 * @file
 * Default template for aakb survey user profile page.
 */
?>
<div class="entity clearfix">
  <h2>Få nyt fra biblioteket</h2>

  <div class="content">
    <div class="field field-type-text field-label-inline clearfix">
      <?php foreach ($types as $title => $info) : ?>
        <div class="field-label"><?php print $title?>:&nbsp;</div>
        <div class="field-items">
          <div class="field-item"><?php print $info['description'] ?></div>
        </div>
      <?php endforeach; ?>
    </div>
  </div>
</div>