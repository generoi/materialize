<?php

/**
 * Implements template_preprocess_html().
 */
function materialize_preprocess_html(&$variables) {
  // Add debugging helpers when the debug parameter is set.
  if (isset($_GET['debug']) || isset($_GET['DEBUG'])) {
    drupal_add_js('window.DEBUG = true;', 'inline');
    if (strpos($_GLOBAL['base_url'], '.dev') !== FALSE) {
      drupal_add_js($_GLOBALS['base_url'] . ':9090/target/target-script-min.js#anonymous');
    }
  }
  if (isset($_GET['firebug'])) {
    drupal_add_js('https://getfirebug.com/releases/lite/1.3/firebug-lite.js');
  }

  // Add shims and libraries.
  $condition = array('IE' => 'lt IE 9', '!IE' => FALSE);
  $options = array('every_page' => TRUE);

  $variables['conditional_scripts_array'][] = array(
    'file' =>  libraries_get_path('es5-shim') . '/es5-shim.js',
    'condition' => $condition,
  );
  $variables['conditional_scripts_array'][] = array(
    'file' =>  libraries_get_path('console-polyfill') . '/index.js',
    'condition' => $condition,
  );

  drupal_add_js(libraries_get_path('Array.from') . '/array-from.js', $options);
  drupal_add_js(libraries_get_path('jquery-touchswipe') . '/jquery.touchSwipe.min.js', $options);
  drupal_add_js(libraries_get_path('fastclick') . '/lib/fastclick.js', $options);
}

/**
 * Implements template_preprocess_page().
 */
function materialize_preprocess_page(&$variables) {
  // Remove tabs as contextual links are the preferred way at Genero.
  if (isset($variables['tabs'])) {
    unset($variables['tabs']);
  }
  $variables['printed_main_menu'] = '';
  $variables['printed_secondary_menu'] = '';
  if (!empty($variables['main_menu'])) {
    $variables['printed_main_menu'] = theme('links__system_main_menu', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('left'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h4',
        'class' => array('element-invisible'),
      ),
    ));
  }
  if (!empty($variables['secondary_menu'])) {
    $variables['printed_secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $variables['secondary_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('right'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h4',
        'class' => array('element-invisible'),
      ),
    ));
  }
}

/**
 * Implements template_preprocess_region().
 */
function materialize_preprocess_region(&$variables) {
}

/**
 * Implements template_preprocess_taxonomy_term().
 */
function materialize_preprocess_taxonomy_term(&$variables) {
  // Add a layout class for the view-mode
  $view_mode = !empty($variables['view_mode']) ? $variables['view_mode'] : 'default';
  $variables['classes_array'][] = 'l-vm-' . $view_mode;
}

/**
 * Implements template_preprocess_node
 */
function materialize_preprocess_node(&$variables) {
  // Add a layout class for the view-mode
  $view_mode = !empty($variables['view_mode']) ? $variables['view_mode'] : 'default';
  $variables['classes_array'][] = 'l-vm-' . $view_mode;

  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    // Add a class to full page nodes
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Implements template_preprocess().
 */
function materialize_preprocess(&$variables, $hook) {
  switch ($hook) {
    case 'node':
    case 'taxonomy_term':
      // If the entity has chosen to display the DS preprocessed field,
      // region_content_field, load all blocks from the content_field region into
      // that field.
      if (!empty($variables['preprocess_fields']) && in_array('region_content_field', $variables['preprocess_fields'])) {
        $variables['region_content_field'] = array();
        if ($blocks = block_get_blocks_by_region('content_field')) {
          $variables['region_content_field'] = $blocks;
        }
        if (module_exists('context')) {
          $reaction = context_get_plugin('reaction', 'block');
          if ($context_blocks = $reaction->block_get_blocks_by_region('content_field')) {
            $variables['region_content_field'] = array_merge($variables['region_content_field'], $context_blocks);
            // @TODO proper sort
            // uasort($variables['region_content_field'], array('context_reaction_block', 'block_sort'));
          }
        }
        $variables['region_content_field'] = drupal_render($variables['region_content_field']);
      }
      break;
  }
}

/**
 * Implements template_preprocess_search_result().
 */
function materialize_preprocess_search_result(&$variables) {
  $nid = $variables['result']['node']->nid;
  $variables['node'] = node_view(node_load($nid), 'search');
}

/**
 * Implements template_preprocess_block()
 */
function materialize_preprocess_block(&$variables) {
  // Add `class` variable defined through blocktheme as real classes.
  if (isset($variables['blocktheme_vars']['class'])) {
    $blocktheme_classes = explode(' ', $variables['blocktheme_vars']['class']);
    $variables['classes_array'] = array_merge($variables['classes_array'], $blocktheme_classes);
  }

  switch ($variables['block']->module) {
    case 'bean':
      // Add a layout class for the view-mode
      if (!empty($variables['elements']['bean'])) {
        foreach (element_children($variables['elements']['bean']) as $key) {
          $element = $variables['elements']['bean'][$key];
          $view_mode = !empty($element['#view_mode']) ? $element['#view_mode'] : 'default';
          $variables['classes_array'][] = 'l-vm-' . $view_mode;
        }
      }
      break;
  }
}
