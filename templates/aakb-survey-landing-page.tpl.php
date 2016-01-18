<?php
/**
 * @file
 *
 */
?>
<div class="contextual-links-region">
  <div class="empty-sidebars default-layout">
    <div class="layout-wrapper">
      <div class="primary-content">
        <article class="page">
          <?php if ($content) : ?>
            <section class="page-content">
              <?php print render($content); ?>
            </section>
          <?php endif; ?>
          <?php if ($form) : ?>
            <footer>
              <?php print render($form); ?>
            </footer>
          <?php endif; ?>
        </article>
      </div>
    </div>
  </div>
</div>
