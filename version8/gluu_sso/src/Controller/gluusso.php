<?php

namespace Drupal\gluu_sso\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\gluu_sso\Plugin\oxds\Register_site;
use Drupal\gluu_sso\Plugin\oxds\Get_authorization_url;
use Drupal\gluu_sso\Plugin\oxds\Get_tokens_by_code;
use Drupal\gluu_sso\Plugin\oxds\Get_user_info;
use Drupal\user\Entity\User;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Config;
/**
 * Class gluusso.
 *
 * @package Drupal\gluu_sso\Controller
 */
class gluusso extends ControllerBase {


  /**
   * Login.
   *
   * @return string
   *   Return Hello string.
   */
   // Pass the dependency to the object constructor

   /**
	 * Checking is oxd port working;
	 */
	public function gluu_sso_is_port_working_module()
	{
		$config = $this->config('gluu_sso.default');
		$oxd_port=$config->get('oxd_port');
		$connection = @fsockopen('127.0.0.1', '8099');
		if (is_resource($connection)) {
			fclose($connection);

			return true;
		} else {
			return false;
		}
	}
	/**
	 * Getting base url;
	 */
	public function gluu_sso_getbaseurl()
	{
		// output: /myproject/index.php
		$currentPath = $_SERVER['PHP_SELF'];

		// output: Array ( [dirname] => /myproject [basename] => index.php [extension] => php [filename] => index )
		$pathInfo = pathinfo($currentPath);

		// output: localhost
		$hostName = $_SERVER['HTTP_HOST'];

		// output: http://
		$protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";

		// return: http://localhost/myproject/
		if (strpos($pathInfo['dirname'], '\\') !== false) {
			return $protocol . $hostName . "/";
		} else {
			return $protocol . $hostName . $pathInfo['dirname'] . "/";
		}
	}
	/**
	 * Getting authorization url for gluu_sso module;
	 */
	public function gluu_sso_login_url()
	{
    $config = $this->config('gluu_sso.default');
		$openidurl=$config->get('openidurl');
		$oxd_port=$config->get('oxd_port');
		$user_acr=$config->get('user_acr');
    $oxd_id=$config->get('gluu_oxd_id');
		$scopes=$config->get('gluu_scopes');
    $gluu_config=$config->get("gluu_config");
    $base_url=self::gluu_sso_getbaseurl();
    if($oxd_id =='')
    {
      drupal_set_message('Please check your admin settings');
      $response = new RedirectResponse($base_url);
      $response->send();
      return;
    }
    else {

        $get_authorization_url = new Get_authorization_url();
        $get_authorization_url->setRequestOxdId($oxd_id);
        $get_authorization_url->setRequestScope($gluu_config['scope']);
        $get_authorization_url->setRequestAcrValues(array($user_acr));
        $get_authorization_url->request();
        header("Location: ".$get_authorization_url->getResponseAuthorizationUrl());
		    exit;
      }
	}

	/**
	 * Implements hook_user_login_validate().
	 */
  public function login() {

     $user = \Drupal::currentUser()->id();
     $base_url=self::gluu_sso_getbaseurl();
	  if ($user == 0) {
		   if (self::gluu_sso_is_port_working_module()) {
            self::gluu_sso_login_url();
		}
		else
		{
			drupal_set_message('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
			$response = new TrustedRedirectResponse($base_url);
			return $response;

		}

	}else{
      return new \Symfony\Component\HttpFoundation\RedirectResponse(\Drupal::url('user.page'));
    }

  }
  /**
	 * Implements hook_logout().
	 */
  public function gluu_sso_logout() {


    $config = $this->config('gluu_sso.default');
    $code= \Drupal::request()->query->get('code');
    $state=\Drupal::request()->query->get('state');
  	$config = $this->config('gluu_sso.default');
  	$gluu_oxd_id=$config->get('gluu_oxd_id');
  	$get_tokens_by_code = new Get_tokens_by_code();
    $get_tokens_by_code->setRequestOxdId($gluu_oxd_id);
    $get_tokens_by_code->setRequestCode($code);
    $get_tokens_by_code->setRequestState($state);
    $get_tokens_by_code->request();
    $get_tokens_by_code->getResponseAccessToken();
    $get_tokens_by_code_array = array();
    $base_url=self::gluu_sso_getbaseurl();
    $get_tokens_by_code_array = array();
    if (!empty($get_tokens_by_code->getResponseAccessToken())) {
				$get_tokens_by_code_array = $get_tokens_by_code->getResponseObject()->data->id_token_claims;
	} else {

				drupal_set_message('Missing claims : Please talk to your organizational system administrator or try again.');
				$response = new TrustedRedirectResponse($base_url);
				return $response;
		}
    $get_user_info = new Get_user_info();
    $get_user_info->setRequestOxdId($gluu_oxd_id);
    $get_user_info->setRequestAccessToken($get_tokens_by_code->getResponseAccessToken());

    $get_user_info->request();
    $response=$get_user_info->getResponseObject();
    $get_user_info_array = $get_user_info->getResponseObject()->data->claims;

    $reg_email = '';
			$reg_user_permission = '';
			if (!empty($get_user_info_array->email[0])) {
				$reg_email = $get_user_info_array->email[0];
			} elseif (!empty($get_tokens_by_code_array->email[0])) {
				$reg_email = $get_tokens_by_code_array->email[0];
			} else {

				drupal_set_message('Missing claim : (email). Please talk to your organizational system administrator.');
				$response = new TrustedRedirectResponse($base_url);
				return $response;
			}

			if (!empty($get_user_info_array->name[0])) {
				$username = $get_user_info_array->name[0];
			} else {
				$username = $reg_email;
			}
			if (!empty($get_user_info_array->permission[0])) {
				$world = str_replace("[", "", $get_user_info_array->permission[0]);
				$reg_user_permission = str_replace("]", "", $world);
			} elseif (!empty($get_tokens_by_code_array->permission[0])) {
				$world = str_replace("[", "", $get_user_info_array->permission[0]);
				$reg_user_permission = str_replace("]", "", $world);
			}

			if ($reg_email) {

         $user = user_load_by_mail($reg_email);
         $logouturl=$config->get('gluu_custom_logout');
         $gluu_users_can_register=$config->get('gluu_users_can_register');

         $gluu_user_role=$config->get('gluu_user_role');
          if(!$user)
          {
            if($gluu_users_can_register=='1')
            {
                if($gluu_user_role==1)
                {
                  $role =='';
                }
                else
                {
                  $role =='administrator';
                }
            }

          if ($gluu_users_can_register == 3) {
            drupal_set_message('You are not authorized for an account on this application. If you think this is an error, please contact your OpenID Connect Provider (OP) admin.');
      				$response = new TrustedRedirectResponse($base_url);
      				return $response;
						}
      		$user = User::create();
      		$userinfo = array(
      			'name' => $username,
      			'pass' => user_password(),
      			'mail' => $reg_email,
      			'role' =>'administrator',
      		);
      		$user->setPassword($userinfo['pass']);
      		$user->enforceIsNew();
      		$user->setEmail($userinfo['mail']);
      		$user->setUsername($userinfo['name']);
      		$user->addRole($userinfo['role']);
      		$user->activate();
      		$user->set('init', $userinfo['mail']);
      		$user->save();
          $uid=$user->id();
          $user = User::load($uid);
      		user_login_finalize($user);
      		$response = new TrustedRedirectResponse($logouturl);
      		return $response;
	}
	else
	   {

		   $uid=$user->get('uid')->value;
		   $user = User::load($uid);
			 user_login_finalize($user);
			 $response = new TrustedRedirectResponse($logouturl);
			 return $response;

	   }
	  }
	else
	  {
				drupal_set_message('Missing claim : (email). Please talk to your organizational system administrator.');
				$response = new TrustedRedirectResponse($base_url);
				return $response;

	  }
	}
}
