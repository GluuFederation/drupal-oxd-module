<?php
/**
 * @file
 * Contains workflow of the miniOrange Active Directory/sso Module.
 */

/**
 * Show workflow.
 */
function gluu_sso_workflow($form, &$form_state) {
  global $base_url;
  $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files';
  $form['markup_1'] = array(
      '#markup' => '<h3>Login structure</h3>',
  );
  $img_path = drupal_get_path('module', 'gluu_sso');
  $img_path = url($bu . '/gluu_drupal.jpg');

  $form['markup_2'] = array(
      '#markup' => '<img src="' . $img_path . '" /><br>',
  );

  return $form;
}
