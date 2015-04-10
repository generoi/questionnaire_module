<?php

/**
 * @file
 * Default theme implementation to display a questionnaire result.
 *
 * Available variables:
 * - $entity: The parent entity of the question field.
 * - $percentage: The percent result of the questionnaire.
 * - $stars: Amount of stars according to the result.
 * - $text: Result text.
 * - $share: A FB share button.
 * - $reset: A button for restarting the questionnaire.
 */
?>
<div id="questionnaire-result">
  <div>
    <h2><?php print $percentage; ?> %</h2>
    <?php print render($stars); ?>
    <p><?php print $text; ?></p>
  </div>

  <div>
    <?php print render($share); ?>
    <?php print render($reset); ?>
  </div>

  <?php if (!empty($motivation)): ?>
    <div>
      <?php print $motivation; ?>
    </div>
  <?php endif; ?>
</div>
