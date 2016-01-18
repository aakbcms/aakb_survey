<?php
/**
 * @file
 * Default template for Aakb survey export page.
 */
?>
<?php foreach ($types as $type => $info): ?>
  <h2 class="title"><?php print $info['title']?></h2>
  <p class="description"><?php print $info['description']?></p>
  <p class="consent-count"><?php print t('Users')?>: <?php print $counts[$type] ?></p>
  <?php if (isset($links[$type]) && $counts[$type] > 0) :?>
    <?php echo render($links[$type]); ?>
    <br /><br />
  <?php endif; ?>
<?php endforeach; ?>
