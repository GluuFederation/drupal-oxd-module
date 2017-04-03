<?php

namespace Drupal\gluu_sso\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Database\Connection;
use Drupal\gluu_sso\Plugin\oxds\Register_site;
use Drupal\Core\Routing\TrustedRedirectResponse;
/**
 * Class DefaultForm.
 *
 * @package Drupal\gluu_sso\Form
 */
class DefaultForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'gluu_sso.default',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'default_form';
  }

  /**
   * {@inheritdoc}
   */

  public function buildForm(array $form, FormStateInterface $form_state) {

	$config = $this->config('gluu_sso.default');
	$oxdprovider=$config->get('openidurl');
  $acr=$config->get('user_acr');
  $oxdid=$config->get('gluu_oxd_id');
  $cilent_id=$config->get('cilent_id','show');
  if($cilent_id=='show'){$type='textfield';$title='Client ID:';} else{$type='hidden';$title='';}
	$cilent_secret=$config->get('cilent_secret','show');
	 if($cilent_secret=='show'){$type='textfield';$title='Client Secret:';} else{$type='hidden';$title='';}
   if($oxdprovider ==''){  $disabled='FALSE'; } else{ $disabled='TRUE';}
	$form['information'] = array(
		  '#type' => 'vertical_tabs',
		  '#default_tab' => 'edit-general',
	);

	$form['general'] = array(
		  '#type' => 'details',
		  '#title' => $this->t('General'),
		  '#group' => 'information',
	);
	$form['general']['form_key'] = array(
		  '#type' => 'hidden',
		  '#default_value' => 'general_register_page',
	);
	$form['general']['url'] = array(
		  '#type' => 'textfield',
		  '#title' => $this->t('Url of the open Provider'),
		  '#default_value' => $config->get('openidurl'),
		  '#disabled' => $config->get('disabledopenurl'),
	);
	$form['general']['gluu_client_id'] = array(
			  '#type' => $type,
			  '#title' => $this->t($title),
			  '#default_value' => $config->get('gluu_client_id'),
	);
	$form['general']['gluu_client_secret'] = array(
			  '#type' => $type,
			  '#title' => $this->t($title),
			  '#default_value' => $config->get('gluu_client_secret'),
	);
	$form['general']['customurl'] = array(
			  '#type' => 'textfield',
			  '#title' => $this->t('Custom url after logout'),
			  '#default_value' => $config->get('gluu_custom_logout'),
	);
	$form['general']['oxd-port'] = array(
		  '#type' => 'textfield',
		  '#title' => $this->t('Oxd-Port'),
		  '#default_value' => $config->get('oxd_port'),
    );
	$form['general']['oxd-id'] = array(
		'#type' => 'textfield',
		'#title' => $this->t('Oxd-id:'),
		'#default_value' => $config->get('gluu_oxd_id'),
		'#disabled' => TRUE,
	);
	$form['general']['enrollment'] = array(
	    '#type' => 'radios',
		'#title' => $this->t('Enrollement'),
		'#default_value' => $config->get('gluu_users_can_register'),
		'#options' => array(1 => $this->t('Automatic Regsiter any user with an account in the openid provider'), 3 => $this->t('Disable Automatic Registration')),
	);
	$form['general']['user_type'] = array(
	   '#type' => 'select',
		'#title' => $this->t('New User Default Role:'),
		'#default_value' => $config->get('gluu_user_role'),
		'#options' => array(1 => $this->t('Regular User'), 3 => $this->t('System Administrator User')),
	);
	$form['openid'] = array(
	  '#type' => 'details',
	  '#title' => $this->t('Open ID Configuration'),
	  '#group' => 'information',

	);

	$form['openid']['scopes'] = array(
	  '#type' => 'checkboxes',
	  '#options' => array('openid' => $this->t('openid'), 'email' => $this->t('email'),'profile' => $this->t('profile'),'permission' => $this->t('permission'),'IMapdata' => $this->t('IMapdata'),'clientinfo' => $this->t('clientinfo'),'address' => $this->t('address')),
	  '#title' => $this->t('Requested Scopes'),
	  '#default_value' => $config->get('gluu_scopes'),

	);
  $form['openid']['user_acr'] = array(
     '#type' => 'select',
    '#title' => $this->t('Select ACR:'),
    '#default_value' => $config->get('user_acr'),
    '#options' => array('default' => $this->t('none'), 'passport' => $this->t('passport'),'auth_ldap_server' => $this->t('auth_ldap_server'),'u2f' => $this->t('u2f'),'super_gluu' => $this->t('super_gluu'),'asimba' => $this->t('asimba'),'otp' => $this->t('otp'),'basic' => $this->t('basic'),'duo' => $this->t('duo')),
  );
	$form['Documentation'] = array(
	  '#type' => 'details',
	  '#title' => $this->t('Documentation'),
	  '#group' => 'information',
	);
  $form['actions']['submit_apply'] = [
  '#type' => 'submit',
  '#value' => t('Save Configuration'),
  '#attributes' => array('class' => array('button button--primary js-form-submit form-submit')),
  ];
  $form['actions']['submit_reset'] = [
  '#type' => 'submit',
  '#value' => t('Reset'),
  '#submit' => array('::submitFormReset'),
  '#attributes' => array('class' => array('button button--primary js-form-submit form-submit')),
  ];
	$form['#attached']['library'][] = 'gluu_sso/gluu_ssojs';
    // return parent::buildForm($form, $form_state,$base_url);
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

  public function gluu_sso_get_base_url_workflow()
	{
		// output: /myproject/index.php
		$currentPath = $_SERVER['PHP_SELF'];

		// output: Array ( [dirname] => /myproject [basename] => index.php [extension] => php [filename] => index )
		$pathInfo = pathinfo($currentPath);

		// output: localhost
		$hostName = $_SERVER['HTTP_HOST'];

		// output: http://
		$protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";

		if (strpos($pathInfo['dirname'], '\\') !== false) {
			return $protocol . $hostName . "/";
		} else {
			return $protocol . $hostName . $pathInfo['dirname'] . "/";
		}
	}
  public function gluu_sso_is_port_working_workflow()
	{
		$base_url=self::gluu_sso_get_base_url_workflow();
		$config = $this->config('gluu_sso.default');
		$gluu_oxd_port=$config->get('oxd_port');
		$connection = @fsockopen('127.0.0.1', $gluu_oxd_port);
		if (is_resource($connection)) {
			fclose($connection);

			return true;
		} else {
			return false;
		}
	}
	public function submitForm(array &$form, FormStateInterface $form_state) {

		$base_url = self::gluu_sso_get_base_url_workflow();
		$account = \Drupal::currentUser();
		/**************************************form posted values*******************************/
		$config = $this->config('gluu_sso.default');
		$values = array(
					  'gluu_provider'  => $form_state->getValue('url'),
					  'gluu_custom_logout'     => $form_state->getValue('customurl'),
					  'gluu_oxd_port' => $form_state->getValue('oxd-port'),
					  'enrollment' 	  => $form_state->getValue('enrollment'),
					  'scopes'        => $form_state->getValue('scopes'),
					  'user_acr'      => $form_state->getValue('user_acr'),
					  'gluu_cilent_id' => $form_state->getValue('gluu_client_id'),
					  'gluu_client_secret' => $form_state->getValue('gluu_client_secret'),
            'user_type'          =>$form_state->getValue('user_type'),
			);
			/*****************************checking roles and ssl***********************************/
			if ($account->id() == 1) {

				if (isset($_REQUEST['form_key']) and strpos($_REQUEST['form_key'], 'general_register_page') !== false) {
					/********************************validation phase start*****************************/
					//checking ssl activation
					if (!isset($_SERVER['HTTPS']) or $_SERVER['HTTPS'] != "on") {
						   drupal_set_message(t('OpenID Connect requires https. This plugin will not work if your website uses http only.'), 'error');
						   $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
						   return $response;
					}
					if (empty($values['gluu_oxd_port'])) {
						drupal_set_message(t('All the fields are required. Please enter valid entries.'), 'error');
						$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
						return $response;
					}
					if (intval($values['gluu_oxd_port']) > 65535 && intval($values['gluu_oxd_port']) < 0) {
						drupal_set_message(t('Enter your oxd host port (Min. number 1, Max. number 65535)'), 'error');
						$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
						return $response;
					}
					if (!empty($values['gluu_provider'])) {
						if (filter_var($values['gluu_provider'], FILTER_VALIDATE_URL) === false) {
							drupal_set_message(t('Please enter valid OpenID Provider URI.'), 'error');
							$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
							return $response;
						}
					}
					if (!empty($values['gluu_custom_logout'])) {
						if (filter_var($values['gluu_custom_logout'], FILTER_VALIDATE_URL) === false) {
							drupal_set_message(t('Please enter valid Custom URI.)'), 'error');
						} else {
							$config->set('gluu_custom_logout', $values['gluu_custom_logout']);
						}
					} else {
						$config->set('gluu_custom_logout', '');
					}
					if (!$config->get('gluu_scopes')) {
						$get_scopes = array("openid", "profile", "email");
						$config->set('gluu_scopes', $get_scopes);
						$config->save();
					}
					if (!empty($values['scopes']) && isset($values['scopes'])) {
            $config->set('gluu_scopes', $values['scopes']);
            $config->save();
            $gluu_config=$config->get("gluu_config");
            foreach ($values['scopes'] as $scope) {
  								if ($scope && !in_array($scope, $get_scopes)) {
  									array_push($gluu_config['scope'], $scope);
  								}
              }
              $config->set('gluu_config',$gluu_config);
              $config->save();
              $gluuconfig= $config->get('gluu_config');
              
            }
					if (!$config->get('gluu_users_can_register')) {
							$gluu_users_can_register = 1;
							$config->set('gluu_users_can_register', $gluu_users_can_register);
							$config->save();
					}
					if ($values['user_type'] == 3) {
            $config->set('gluu_user_role', 3);
						$config->save();
					} else {
						$config->set('gluu_user_role', 1);
						$config->save();
					}
					if ($values['enrollment'] == 3) {
						$config->set('gluu_users_can_register', 3);
						$config->save();
					} else {
						$config->set('gluu_users_can_register', 1);
						$config->save();
					}
          if(isset($values['user_acr']))
          {
            $config->set('user_acr', $values['user_acr']);
            $config->save();
          }
					//checking gluu provider services
					$gluu_provider = $form_state->getValue('url');
					if (isset($gluu_provider) and !empty($gluu_provider)) {
					$arrContextOptions = array(
							"ssl" => array(
							"verify_peer" => false,
							"verify_peer_name" => false,
							),
					);
					$json = file_get_contents($gluu_provider . '/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));
					$obj = json_decode($json);
					$config->set('openidurl',$gluu_provider);
					$config->set('oxd_port', $values['gluu_oxd_port']);
					$config->save();
          $gluu_provider=$config->get('openidurl');
					if (!empty($obj->userinfo_endpoint)) {
              if (empty($obj->registration_endpoint)) {
                drupal_set_message(t('Please enter your client_id and client_secret.'), 'status');
								$config->set('cilent_id','show');
								$config->set('cilent_secret','show');
								$config->save();
								//code saving cilent id and secret key
								$gluu_config = array(
							"gluu_oxd_port" => $_POST['gluu_oxd_port'],
							"admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
							"authorization_redirect_uri" => $base_url . 'index.php?gluuOption=oxdOpenId',
							"post_logout_redirect_uri" => $base_url . 'index.php?option=allLogout',
							"config_scopes" => ["openid", "profile", "email"],
							"gluu_client_id" => "",
							"gluu_client_secret" => "",
							"config_acr" => []
						);

						if (isset($values['gluu_client_id']) and !empty($values['gluu_client_id']) and
							isset($values['gluu_client_secret']) and !empty($values['gluu_client_secret'])
						) {
							$gluu_config = array("op_host"=> $gluu_provider,"oxd_host_port"=>$values['gluu_oxd_port'],"authorization_redirect_uri" => $base_url."gluu_sso/gluuslogout.php","post_logout_redirect_uri" => $base_url."admin/config/gluu_sso/default","scope" => [ "openid", "profile","email"],"application_type" => "web","response_types" => ["code"],"grant_types"=>["authorization_code"],"config_acr"=>[],"gluu_client_id"=>$values['gluu_client_id'],"gluu_client_secret"=>$values['gluu_client_secret']);
							$config->set('gluu_config', $gluu_config);
							$config->save();
							if (!self::gluu_sso_is_port_working_workflow()) {
								drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
								$register_site = new Register_site();
								$register_site->setRequestOpHost($gluu_config['op_host']);
								$register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
								$register_site->setRequestPostLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
								$register_site->setRequestContacts(array(\Drupal::config('system.site')->get('mail')));
								$register_site->setRequestGrantTypes($gluu_config['grant_types']);
								$register_site->setRequestResponseTypes($gluu_config['response_types']);
								$register_site->request();
								$get_scopes = $obj->scopes_supported;
							if (!empty($obj->acr_values_supported)) {
								$get_acr = $obj->acr_values_supported;
								$config->set('gluu_acr', $get_acr);
								$config->save();
								$register_site->setRequestAcrValues($gluu_config['config_acr']);
							} else {
								$register_site->setRequestAcrValues($gluu_config['config_acr']);
							}
							if (!empty($obj->scopes_supported)) {
								$get_scopes = $obj->scopes_supported;
								$config->set('gluu_scopes', $get_scopes);
								$config->save();
								$register_site->setRequestScope($obj->scopes_supported);
							} else {
								$register_site->setRequestScope($gluu_config['scope']);
							}
							$register_site->setRequestClientId($gluu_config['gluu_client_id']);
							$register_site->setRequestClientSecret($gluu_config['gluu_client_secret']);
							$status = $register_site->request();
							if ($status['message'] == 'invalid_op_host') {
								drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
							if (!$status['status']) {
								drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
							if ($status['message'] == 'internal_error') {
								drupal_set_message(t('message_error', 'ERROR: ' . $status['error_message']),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
							$gluu_oxd_id = $register_site->getResponseOxdId();
							//var_dump($register_site->getResponseObject());exit;
							if ($gluu_oxd_id) {
								$config->set('gluu_oxd_id', $gluu_oxd_id);
								$config->save();
								$gluu_provider = $register_site->getResponseOpHost();
								$config->set('gluu_provider', $gluu_provider);
								$config->save();
								drupal_set_message(t('Your settings are saved successfully.'),'status');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;


							} else {
								drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
							} else {
								drupal_set_message(t('openid_error', 'Error505.'),'error');
								$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
								return $response;
							}
						}

						else
						{		// without cilent id and secret key code

                $gluu_provider=$config->get('openidurl');
								$gluu_config = array("op_host"=> $gluu_provider,"oxd_host_port"=>$values['gluu_oxd_port'],"authorization_redirect_uri" => $base_url."gluu_sso/gluuslogout.php","post_logout_redirect_uri" => $base_url."gluu_sso/gluuslogout.php","scope" => [ "openid", "profile","email"],"application_type" => "web","response_types" => ["code"],"grant_types"=>["authorization_code"],"config_acr"=>[]);
                $config->set('gluu_config', $gluu_config);
								$config->save();
								if (!self::gluu_sso_is_port_working_workflow()) {
									drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'),'error');
									$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
									return $response;
								}
								$register_site = new Register_site();
								$register_site->setRequestOpHost($gluu_config['op_host']);
								$register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
								$register_site->setRequestPostLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
								$register_site->setRequestContacts(array(\Drupal::config('system.site')->get('mail')));
								$register_site->setRequestGrantTypes($gluu_config['grant_types']);
								$register_site->setRequestResponseTypes($gluu_config['response_types']);
								$register_site->request();
								$get_scopes = $obj->scopes_supported;
								if (!empty($obj->acr_values_supported)) {
									$register_site->setRequestAcrValues($obj->acr_values_supported);
								} else {
									$register_site->setRequestAcrValues($gluu_config['config_acr']);
								}
								if (!empty($obj->scopes_supported)) {
									$get_scopes = $obj->scopes_supported;
									$register_site->setRequestScope($obj->scopes_supported);
								} else {
									$register_site->setRequestScope($gluu_config['scope']);
								}
								$status = $register_site->request();
								$gluu_oxd_id = $register_site->getResponseOxdId();
								if ($gluu_oxd_id){
									$config = $this->config('gluu_sso.default');
									$config->set('gluu_oxd_id', $gluu_oxd_id);
									$config->save();
									$register_site->getResponseOpHost();
									$config->set('gluu_provider', $gluu_provider);
									$config->save();
									drupal_set_message(t('Your settings are saved successfully.'),'status');
									$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
									return $response;


								}
								else{
									drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"),'error');
									$response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
									return $response;
								}
						}
          }
					}//checking gluu server end points
				}
					/********************************************validation phase end**************************************/

			}//checking form key

		}

public function submitFormReset(array &$form, FormStateInterface $form_state) {

      $config = \Drupal::configFactory()->getEditable('gluu_sso.default');
      $config->clear('openidurl');
      $config->clear('gluu_custom_logout');
      $config->clear('oxd_port');
      $config->clear('gluu_oxd_id');
      $config->clear('gluu_users_can_register');
      $config->clear('gluu_user_role');
      $config->clear('gluu_scopes');
      $config->save();
      drupal_set_message("Reset successfully");
    }

}
