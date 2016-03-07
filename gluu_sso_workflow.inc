<?php
/**
 * @file
 * Contains workflow of the miniOrange Active Directory/sso Module.
 */

/**
 * Show workflow.
 */
function gluu_sso_workflow($form, &$form_state) {
  $form['markup_1'] = array(
      '#markup' => '<h3>Login structure</h3>',
  );
  $img_path = drupal_get_path('module', 'gluu_sso');
  $img_path = url($img_path . '/files/gluu_drupal.jpg');

  $form['markup_2'] = array(
      '#markup' => '<img src="' . $img_path . '" /><br>',
  );

  return $form;
}
