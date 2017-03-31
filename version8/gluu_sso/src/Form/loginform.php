<?php

namespace Drupal\gluu_sso\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\gluu_sso\Plugin\oxds\Register_site;
use Drupal\gluu_sso\Plugin\oxds\Get_authorization_url;
use Drupal\Core\Url;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Drupal\Core\Ajax\AjaxResponse;
/**
 * Class loginform.
 *
 * @package Drupal\gluu_sso\Form
 */
class loginform extends FormBase {


  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'loginform_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    echo $_GET['code'];
    $form['submit'] = [
        '#type' => 'submit',
        '#value' => $this->t('Submit'),
    ];

    return $form;
  }

  /**
    * {@inheritdoc}
    */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    parent::validateForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {


    if($_GET['code']=='')
    {
      $gluu_config = array("op_host"=> "https://ce-dev2.gluu.org/","oxd_host_port"=>8099,"authorization_redirect_uri" => "https://drupal-oxd.com/gluu_sso/form/loginform.php","post_logout_redirect_uri" => "https://drupal-oxd.com/","scope" => [ "openid", "profile","email"],"application_type" => "web","response_types" => ["code"],"grant_types"=>["authorization_code"]);
      $register_site = new Register_site();
  		$register_site->setRequestOpHost($gluu_config['op_host']);
  		$register_site->setRequestAcrValues(array('basic'));
  		$register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
  		$register_site->setRequestPostLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
  		$register_site->setRequestContacts(array('sumiti@ourdesignz.com'));
  		$register_site->setRequestGrantTypes($gluu_config['grant_types']);
  		$register_site->setRequestResponseTypes($gluu_config['response_types']);
  		$register_site->setRequestScope($gluu_config['scope']);
  		$register_site->request();
  		$oxd_id=$register_site->getResponseOxdId();
      $get_authorization_url = new Get_authorization_url();
  		$get_authorization_url->setRequestOxdId($oxd_id);
      $get_authorization_url->setRequestScope($gluu_config['scope']);
      $get_authorization_url->setRequestAcrValues(array('basic'));
      $get_authorization_url->request();
  		header("Location: ".$get_authorization_url->getResponseAuthorizationUrl());
      exit;
    }
    else
    {
        echo $_GET['code'];
    }
    // Display result.
    //foreach ($form_state->getValues() as $key => $value) {
        //drupal_set_message($key . ': ' . $value);
    //}

  }
  public function usernameValidateCallback(array &$form, FormStateInterface $form_state) {
      $ajax_response = new AjaxResponse();
      print_r($ajax_response);
  }

}
