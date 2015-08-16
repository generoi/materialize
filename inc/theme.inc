<?php

/**
 * Overrides theme_links() for locale_block.
 */
function materialize_links__locale_block($variables) {
  // Set a .menu class to the <ul>-element so it can be styled like
  // menu_block:s.
  $variables['attributes']['class'][] = 'menu';
  return theme('links', $variables);
}

/**
 * Implements theme_context_bean_child().
 *
 * Dont render the title for the child beans, leave it up that bean instead.
 */
function materialize_context_bean_child(&$variables) {
  $child_bean = entity_view('bean', array($variables['entity']), $variables['view_mode']);
  $content = '<div id="block-bean-' . $variables['entity']->delta . '" class="block block-bean ' . $variables['css_class'] . '">';
  $content .= drupal_render($child_bean);
  $content .= '</div>';
  return $content;
}
