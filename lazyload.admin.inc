<?php
/**
 * @file
 * Administrative page callbacks for the LazyLoad module.
 */

/**
 * LazyLoad configuration form.
 */
function lazyload_admin_form($form, &$form_state) {
  $config = config('lazyload.settings');

  $form['lazy'] = array(
    '#type' => 'details',
    '#summary' => t('Lazy loading'),
  );

  $form['lazy']['lazy_threshold'] = array(
    '#type' => 'number',
    '#title' => t('Offset'),
    '#description' => t("The offset controls how early you want the elements to be loaded before they are visible. For example, value 100 means 100px before an element enters a browser viewport it will start loading. Zero value means only images that are inside visible page area will be loaded."),
    '#default_value' => $config->get('lazy_threshold'),
    '#min' => 0,
    '#max' => 9999,
    '#field_suffix' => t('pix'),
  );

  // Title and description are made as markup elements. This is needed for
  // pretty looking inline placement of the two 'number' fields below.
  $form['lazy']['img_min_title'] = array(
    '#prefix' => '<label style="margin-bottom: 6px">',
    '#markup' => t('Minimum image size'),
    '#suffix' => '</label>',
  );

  // Container for inline placement of the next two fields.
  $form['lazy']['img_min'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );

  $form['lazy']['img_min']['width'] = array(
    '#type' => 'number',
    '#default_value' => $config->get('img_min_width'),
    '#min' => 0,
    '#max' => 9999,
    '#field_suffix' => t('x'),
  );

  $form['lazy']['img_min']['height'] = array(
    '#type' => 'number',
    '#default_value' => $config->get('img_min_height'),
    '#min' => 0,
    '#max' => 9999,
    '#field_suffix' => t('pix'),
  );

  $form['lazy']['img_min_descr'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => t('Defines the minimum size of images that Lazy Load will process. Images smaller than this threshold are ignored. The size is measured as total pixels (width × height), not by width or height alone. That means, for example, both a 100×100 image and a 50×200 image equal 10,000 pixels and are treated the same. It makes sense to set minimum width and height larger than the size of the <em>Low-resolution preview image</em>. If <a href="@link" target="_blank">Thumbnail (100x100)</a> is set for a preview, then 150x150 and higher can be recommended for the minimum size.', array('@link' => url('admin/config/media/image-styles/configure/thumbnail'))),
    '#suffix' => '</div>',
  );

  $form['lazy']['loading_vis'] = array(
    '#type' => 'select',
    '#title' => t('Display while image is being loaded'),
    '#options' => array(
      'preview' => t('Low resolution preview image'),
      'blank'   => t('Blank image'),
    ),
    '#default_value' => $config->get('loading_vis'),
    '#states' => array(
      'visible' => array(
        ':input[name="lazy_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['lazy']['preview_style'] = array(
    '#type' => 'select',
    '#title' => t('Preview image style'),
    '#options' => image_style_options(FALSE, PASS_THROUGH),
    '#default_value' => $config->get('preview_style'),
    '#description' => t('Select the style for the smaller version of the main image that will be displayed as a preview. The style should keep the proportions of the main image. To create a new style or edit an existing one, go to <a href="@link" target="_blank">image styles configuration</a> page.', array('@link' => url('admin/config/media/image-styles'))),
    '#states' => array(
      'visible' => array(
        ':input[name="loading_vis"]' => array('value' => 'preview'),
      ),
    ),
  );

  $form['pages'] = array(
    '#type' => 'details',
    '#summary' => t('Page filter'),
  );

  $form['pages']['filter_rule'] = array(
    '#type' => 'radios',
    '#title' => t('Filter rule'),
    '#default_value' => $config->get('pages_filter_rule'),
    '#options' => array(
      'exclude' => t('Enable on all pages except listed'),
      'include' => t('Enable only on listed pages'),
    ),
  );

  $form['pages']['paths'] = array(
    '#type' => 'options',
    '#title' => t('Page paths'),
    // Options are stored in config as a string for compatibility
    // with backdrop_match_path() function.
    '#options' => explode("\n", $config->get('pages_paths')),
    '#key_type' => 'none',
    '#optgroups' => FALSE,
    '#default_value_allowed' => FALSE,
    // Add a function that removes irrelevant 'allowed HTML tags' text.
    // The 'form_options_expand' is a default options building function.
    '#process' => array('form_options_expand', 'lazyload_form_options_edit'),
    '#attached' => array(
      // Add CSS to hide the misleading 'Label' text in the table header.
      'css' => array (
        backdrop_get_path('module', 'lazyload') . '/css/lazyload_admin_options.css',
      ),
    ),
    '#id' => 'lazyload-path-list', // id to bind the above CSS to the element.
    '#description' => t('List of website page paths where the module will be enabled or disabled depending on the previously selected %filter_rule. You can use the wildcard(*) as a placeholder for any part of the path.', array('%filter_rule' => $form['pages']['filter_rule']['#title'])),
  );

  $form['pages']['no_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude administrative pages'),
    '#default_value' => $config->get('pages_no_admin'),
    '#description' => t("Disable Lazy Load on all administrative pages. This filter is independent from the %filter_rule and %page_paths.", array('%filter_rule' => $form['pages']['filter_rule']['#title'], '%page_paths' => $form['pages']['paths']['#title'])),
  );

  $form['advanced'] = array (
    '#type' => 'details',
    '#summary' => t('Advanced'),
  );

  $form['advanced']['disable_js_plugin'] = array (
    '#type' => 'checkbox',
    '#title' => t('Disable LazyLoad JavaScript'),
    '#default_value' => $config->get('disable_js_plugin'),
    '#description' => t('When enabled, the Lazy Load script will not run. This option is for testing only. The module will still modify the <em>img</em> tag HTML and display the preview image instead of the original, but it will not swap the preview once the original has been loaded. This is useful for checking which images are affected by the module.'),
  );

  // Submit button.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Removes text about allowed HTML tags. This text is irrelevant to
 * the current use of the options element as a path list.
 */
function lazyload_form_options_edit($element) {
  $element['#attached']['js'][0]['data']['optionsElement']['allowedtags'] = '';
  return $element;
}

/**
 * Validation function for Lazy Load admin form.
 */
function lazyload_admin_form_validate($form, &$form_state) {
  // Trim and validate the path patterns.
  foreach($form_state['values']['paths']['options'] as &$path) {
    $path = trim($path, " \n\r\t\v\x00\/"); // '\/' added to the default chars.
    if ($path !== '' && !valid_url($path)) {
      form_set_error('paths', t('The path %path is not valid.', array('%path' => $path)));
    }
  }
  unset($path);
}

/**
 * Saves configuration settings when the form is submitted.
 */
function lazyload_admin_form_submit($form, &$form_state) {
  $config = config('lazyload.settings');

  $config->set('lazy_threshold', $form_state['values']['lazy_threshold']);

  // Save pre calculated pixel count along with min width and height.
  // The pixel count is not exposed to the user but is used internally.
  $img_min_width  = $form_state['values']['width'];
  $img_min_height = $form_state['values']['height'];
  $config->set('img_min_width', $img_min_width);
  $config->set('img_min_height', $img_min_height);
  $config->set('img_min_pix_count', (int)$img_min_width * (int)$img_min_height);

  $config->set('loading_vis', $form_state['values']['loading_vis']);
  $config->set('preview_style', $form_state['values']['preview_style']);

  $config->set('pages_filter_rule', $form_state['values']['filter_rule']);
  $config->set('pages_no_admin', $form_state['values']['no_admin']);
  // Save paths as a single string compatible with backdrop_match_path().
  // backdrop_match_path() takes path patterns as one string where each pattern
  // is separated with \n or \r symbols.
  $path_patterns = implode("\n", $form_state['values']['paths']['options']);
  $config->set('pages_paths', $path_patterns);

  $config->set('disable_js_plugin', $form_state['values']['disable_js_plugin']);

  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));

}
