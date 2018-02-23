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
use Drupal\gluu_sso\Plugin\oxds\Gluu_sso_logout;
use Drupal\user\Entity\User;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Config;
use Drupal\gluu_sso\Utils\Utilities;
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
    public function gluu_sso_is_port_working_module() {
        $config = $this->config('gluu_sso.default');
        $oxd_port = $config->get('oxd_port');
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
    public function gluu_sso_getbaseurl() {
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
     * Getting logout url;
     */
    public function gluu_sso_getlogouturl() {
        $config = \Drupal::config('gluu_sso.default');
        $oxd_id = $config->get('gluu_oxd_id');
        $gluu_config = $config->get('gluu_config');
        $logout = new Gluu_sso_logout();
        $logout->setRequestOxdId($oxd_id);
        $logout->setRequestIdToken($_SESSION['user_oxd_id_token']);
        $logout->setRequestPostLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
        $logout->setRequestSessionState($_SESSION['session_states']);
        $logout->setRequestState($_SESSION['state']);
        $utilities = new Utilities();
        if($gluu_config["has_registration_endpoint"]){
            $logout->setRequest_protection_access_token($utilities->get_protection_access_token($config));
        }
        if($gluu_config['connection_type'] == 1 || is_null($gluu_config['connection_type'])){
            $logout->request();
        }else{
            $logout->request(rtrim($gluu_config['oxd_web_host'],'/').'/get-logout-uri');
        }
        unset($_SESSION['user_oxd_access_token']);
        unset($_SESSION['user_oxd_id_token']);
        unset($_SESSION['session_states']);
        unset($_SESSION['state']);
        unset($_SESSION['session_in_op']);
        return $logout->getResponseObject()->data->uri;
    }

    /**
     * Getting authorization url for gluu_sso module;
     */
    public function gluu_sso_login_url() {
        $config = $this->config('gluu_sso.default');
        $openidurl = $config->get('openidurl');
        $oxd_port = $config->get('oxd_port');
        $user_acr = $config->get('user_acr');
        $oxd_id = $config->get('gluu_oxd_id');
        $scopes = $config->get('gluu_scopes');
        $gluu_config = $config->get("gluu_config");
        $base_url = self::gluu_sso_getbaseurl();
        if ($oxd_id == '') {
            drupal_set_message('Please check your admin settings');
            $response = new RedirectResponse($base_url);
            $response->send();
            return;
        } else {

            $get_authorization_url = new Get_authorization_url();
            $get_authorization_url->setRequestOxdId($oxd_id);
            $get_authorization_url->setRequestScope($gluu_config['scope']);
            $get_authorization_url->setRequestAcrValues(array($user_acr));
            $utilities = new Utilities();
            if($gluu_config["has_registration_endpoint"] == 1){
                $get_authorization_url->setRequest_protection_access_token($utilities->get_protection_access_token($config));
            }
            if($gluu_config['connection_type'] == 2){
                $get_authorization_url->request(rtrim($gluu_config['oxd_web_host'],'/')."/get-authorization-url");
            }else{
                $get_authorization_url->request();
            }
            header("Location: " . $get_authorization_url->getResponseAuthorizationUrl());
            exit;
        }
    }

    /**
     * Implements hook_user_login_validate().
     */
    public function login() {

        $user = \Drupal::currentUser()->id();
        $base_url = self::gluu_sso_getbaseurl();
        if ($user == 0) {
            if (self::gluu_sso_is_port_working_module()) {
                self::gluu_sso_login_url();
            } else {
                drupal_set_message('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                $response = new TrustedRedirectResponse($base_url);
                return $response;
            }
        } else {
            return new \Symfony\Component\HttpFoundation\RedirectResponse(\Drupal::url('user.page'));
        }
    }
    
    
    
    public function gluu_sso_login_redirect() {
        $config = $this->config('gluu_sso.default');
        $code = \Drupal::request()->query->get('code');
        $state = \Drupal::request()->query->get('state');
        $config = $this->config('gluu_sso.default');
        $gluu_oxd_id = $config->get('gluu_oxd_id');
        $gluu_config = $config->get('gluu_config');
        
        $get_tokens_by_code = new Get_tokens_by_code();
        $get_tokens_by_code->setRequestOxdId($gluu_oxd_id);
        $get_tokens_by_code->setRequestCode($code);
        $get_tokens_by_code->setRequestState($state);
        $utilities = new Utilities();
        if($gluu_config["has_registration_endpoint"] == 1){
            $get_tokens_by_code->setRequest_protection_access_token($utilities->get_protection_access_token($config));
        }
        if($gluu_config['connection_type'] == 2){
            $get_tokens_by_code->request(rtrim($gluu_config['oxd_web_host'],'/')."/get-tokens-by-code");
        }else{
            $get_tokens_by_code->request();
        }
        
        $_SESSION['session_in_op'] = $get_tokens_by_code->getResponseIdTokenClaims()->exp[0];
        $_SESSION['user_oxd_id_token'] = $get_tokens_by_code->getResponseIdToken();
        $_SESSION['user_oxd_access_token'] = $get_tokens_by_code->getResponseAccessToken();
        $_SESSION['session_states'] = $_REQUEST['session_state'];
        $_SESSION['state'] = $_REQUEST['state'];
        
        $get_tokens_by_code->getResponseAccessToken();
        $get_tokens_by_code_array = array();
        $base_url = self::gluu_sso_getbaseurl();
        $get_tokens_by_code_array = array();
        if (!empty($get_tokens_by_code->getResponseAccessToken())) {
            $get_tokens_by_code_array = $get_tokens_by_code->getResponseObject()->data->id_token_claims;
        } else {

            drupal_set_message('Missing claims : Please talk to your organizational system administrator or try again.');
            $response = new TrustedRedirectResponse(self::gluu_sso_getlogouturl());
            return $response;
        }
        $get_user_info = new Get_user_info();
        $get_user_info->setRequestOxdId($gluu_oxd_id);
        $get_user_info->setRequestAccessToken($get_tokens_by_code->getResponseAccessToken());
        
        
        if($gluu_config["has_registration_endpoint"] == 1){
            $get_user_info->setRequest_protection_access_token($utilities->get_protection_access_token($config));
        }
        if($gluu_config['connection_type'] == 2){
            $get_user_info->request(rtrim($gluu_config['oxd_web_host'],'/')."/get-user-info");
        }else{
            $get_user_info->request();
        }
        $response = $get_user_info->getResponseObject();
        $get_user_info_array = $get_user_info->getResponseObject()->data->claims;
        
        $reg_email = '';
        $reg_user_permission = '';
        if (!empty($get_user_info_array->email[0])) {
            $reg_email = $get_user_info_array->email[0];
        } elseif (!empty($get_tokens_by_code_array->email[0])) {
            $reg_email = $get_tokens_by_code_array->email[0];
        } else {

            drupal_set_message('Missing claim : (email). Please talk to your organizational system administrator.');
            $response = new TrustedRedirectResponse(self::gluu_sso_getlogouturl());
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
        
        $bool = false;
        $gluu_new_roles = $config->get('gluu_new_roles');
        $gluu_users_can_register = $config->get('gluu_users_can_register');
        if ($gluu_users_can_register == 2 and !empty($gluu_new_roles)) {
            foreach ($gluu_new_roles as $gluu_new_role) {
                    if (strstr($reg_user_permission, $gluu_new_role)) {
                            $bool = true;
                    }
            }
            if(!$bool){
                drupal_set_message('You are not authorized for an account on this application. If you think this is an error, please contact your OpenID Connect Provider (OP) admin.');
                $response = new TrustedRedirectResponse(self::gluu_sso_getlogouturl());
                return $response;
            }
        }
        if ($reg_email) {
            $user = user_load_by_mail($reg_email);
            $logouturl = $base_url;
            $gluu_users_can_register = $config->get('gluu_users_can_register');

            $gluu_user_role = $config->get('gluu_user_role');
            if (!$user) {
                if ($gluu_users_can_register == '1') {
                    if ($gluu_user_role == 1) {
                        $role == '';
                    } else {
                        $role == 'administrator';
                    }
                }

                if ($gluu_users_can_register == 3) {
                    drupal_set_message('You are not authorized for an account on this application. If you think this is an error, please contact your Drupal admin.');
                    $response = new TrustedRedirectResponse(self::gluu_sso_getlogouturl());
                    return $response;
                }
                $user = User::create();
                if($config->get('gluu_user_role') == 3){
                    $role = 'administrator';
                } else {
                    $role = 'authenticated';
                }
                $userinfo = array(
                    'name' => $username,
                    'pass' => user_password(),
                    'mail' => $reg_email,
                    'role' => 'administrator',
                );
                $user->setPassword($userinfo['pass']);
                $user->enforceIsNew();
                $user->setEmail($userinfo['mail']);
                $user->setUsername($userinfo['name']);
                $user->addRole($userinfo['role']);
                $user->activate();
                $user->set('init', $userinfo['mail']);
                $user->save();
                $uid = $user->id();
                $user = User::load($uid);
                user_login_finalize($user);
                $response = new TrustedRedirectResponse($logouturl);
                return $response;
            } else {

                $uid = $user->get('uid')->value;
                $user = User::load($uid);
                user_login_finalize($user);
                $response = new TrustedRedirectResponse($logouturl);
                return $response;
            }
        } else {
            drupal_set_message('Missing claim : (email). Please talk to your organizational system administrator.');
            $response = new TrustedRedirectResponse(self::gluu_sso_getlogouturl());
            return $response;
        }
    }

}
