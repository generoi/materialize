<?php

/**
 * Implements template_process_html().
 */
function materialize_process_html(&$variables) {
  // Add conditional scripts to the markup.
  $variables['conditional_scripts'] = '';
  foreach($variables['conditional_scripts_array'] as $script) {
    $element = array(
      '#tag' => 'script',
      '#attributes' =>  array(
        'src' => file_create_url($script['file']),
      ),
      '#browsers' => $script['condition'],
      '#pre_render' => array('drupal_pre_render_conditional_comments'),
    );
    $variables['conditional_scripts'] .= drupal_render($element);
  }
}

/**
 * Implements hook_process_html_tag()
 *
 * Prunes HTML tags: http://sonspring.com/journal/html5-in-drupal-7#_pruning
 */
function materialize_process_html_tag(&$variables) {
  $element = &$variables['element'];
  // Remove type="..." and CDATA prefix/suffix.
  unset($element['#attributes']['type'], $element['#value_prefix'], $element['#value_suffix']);
  // Remove media="all" but leave others unaffected.
  if (isset($element['#attributes']['media']) && $element['#attributes']['media'] === 'all') {
    unset($element['#attributes']['media']);
  }
}
