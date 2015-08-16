<?php

/**
 * Implements hook_html_head_alter().
 */
function materialize_html_head_alter(&$head_elements) {
  // HTML5 charset declaration.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
  // Optimize mobile viewport.
  $head_elements['mobile_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  );
  // Remove image toolbar in IE.
  $head_elements['ie_image_toolbar'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'ImageToolbar',
      'content' => 'false',
    ),
  );
}

/**
 * Implements hook_date_select_process_alter().
 *
 * Make date selects use proper month names instead of odd abbreviations.
 * @see date_parts_element().
 */
function materialize_date_select_process_alter(&$element, $form_state, $context) {
  if (is_array($element['#default_value'])) {
    $date = date_select_input_date($element, $element['#default_value']);
  }
  elseif (!empty($element['#default_value'])) {
    $date = date_default_date($element);
  }
  $part_required = (bool) $element['#required'] && is_object($date);
  // Change the month to use full names
  if (isset($element['month'])) {
    // Keep the label
    $labels = array_values($element['month']['#options']);
    $first = array_shift($labels);
    // Use date_month_names() instead of date_month_names_abbr().
    $element['month']['#options'] = date_month_names($part_required);
    $element['month']['#options'][''] = $first;
  }

  $year = date('Y');
  // Set the years default value to the current year.
  if (isset($element['year']['#options'][$year]) && empty($element['year']['#default_value'])) {
    $element['year']['#default_value'] = $year;
  }

  foreach (array('year', 'month', 'day', 'hour', 'minute', 'second') as $select) {
    if (isset($element[$select]['#options'])) {
      // Remove the leading dash in the first option (label) of date selects.
      $element[$select]['#options'][''] = preg_replace('/^-/', '', $element[$select]['#options']['']);
    }
  }
}

/**
 * Implements hook_css_alter().
 */
function materialize_css_alter(&$css) {
  $exclude = array(
    'sites/all/modules/contrib/date/date_popup/themes/datepicker.1.7.css' => FALSE,
    'sites/all/modules/contrib/date/date_api/date.css' => FALSE,
    'sites/all/modules/contrib/picture/picture_wysiwyg.css' => FALSE,
    'modules/user/user.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'sites/all/modules/contrib/logintoboggan/logintoboggan.css' => FALSE,
    'sites/all/modules/contrib/ckeditor/ckeditor.css' => FALSE,
    // Fivestar requests a bunch of images we do not need.
    'sites/all/modules/contrib/fivestar/css/fivestar.css' => FALSE,
    'sites/all/modules/contrib/viewfield/viewfield.css' => FALSE, // request fails, file does not exist.
    'modules/system/system.messages.css' => FALSE, // We use foundation
    'modules/system/system.base.css' => FALSE, // Dont load all those images
    'modules/system/system.theme.css' => FALSE, // Dont load all those images
    // Zurb removes system.menus.css
    'modules/filter/filter.css' => FALSE, // Dont load all those images
  );
  $css = array_diff_key($css, $exclude);
}

/**
 * Implements hook_js_alter().
 */
function materialize_js_alter(&$javascript) {
  $exclude = array(
    'misc/textarea.js' => FALSE,
    'misc/tableheader.js' => FALSE,
    'misc/tabledrag.js' => FALSE,
    'sites/all/modules/contrib/admin_menu/admin_devel/admin_devel.js' => FALSE,
  );
  $javascript = array_diff_key($javascript, $exclude);
}
