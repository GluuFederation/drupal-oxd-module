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

  public function login() {
      global $user;
      if ($user->uid == 0) {
            $gluu_config = array("op_host"=> "https://ce-dev2.gluu.org/","oxd_host_port"=>8099,"authorization_redirect_uri" => "https://drupal-oxd.com/gluu_sso/gluuslogout.php","post_logout_redirect_uri" => "https://drupal-oxd.com/gluu_sso/gluuslogin.php","scope" => [ "openid", "profile","email"],"application_type" => "web","response_types" => ["code"],"grant_types"=>["authorization_code"]);
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
            $connection = \Drupal\Core\Database\Database::getConnection();
            $connection->update('oxdid')
      					->fields(array(
                  'oxd_id' => $oxd_id,
            ))
      			->condition('id', 0, '=')
      			->execute();
            $get_authorization_url = new Get_authorization_url();
            $get_authorization_url->setRequestOxdId($oxd_id);
            $get_authorization_url->setRequestScope($gluu_config['scope']);
            $get_authorization_url->setRequestAcrValues(array('basic'));
            $get_authorization_url->request();
            header("Location: ".$get_authorization_url->getResponseAuthorizationUrl());
      exit;
    }else{
      return new \Symfony\Component\HttpFoundation\RedirectResponse(\Drupal::url('user.page'));
    }
  }
  public function gluu_sso_logout() {

    $code= \Drupal::request()->query->get('code');
    $state=\Drupal::request()->query->get('state');
    $connection = \Drupal\Core\Database\Database::getConnection();
  	$results = $connection->query('select  * from oxdid')->fetch();
    $get_tokens_by_code = new Get_tokens_by_code();
    $get_tokens_by_code->setRequestOxdId($results->oxd_id);
    $get_tokens_by_code->setRequestCode($code);
    $get_tokens_by_code->setRequestState($state);
    $get_tokens_by_code->request();
    $get_tokens_by_code->getResponseAccessToken();
    $get_user_info = new Get_user_info();
    $get_user_info->setRequestOxdId($results->oxd_id);
    $get_user_info->setRequestAccessToken($get_tokens_by_code->getResponseAccessToken());
    $get_user_info->request();
    $response=$get_user_info->getResponseObject();
    $username=$response->data->claims->email[0];
    $email=$response->data->claims->name[0];
    $user = User::create();
    $userinfo = array(
      'name' => $username,
      'pass' => '123456789',
      'mail' => $email,
      'role' =>'authenticated',
    );
    $user->setPassword($userinfo['pass']);
    $user->enforceIsNew();
    $user->setEmail($userinfo['mail']);
    $user->setUsername($userinfo['name']);
    $user->addRole('role_name');
    $user->activate();
    $user->set('init', $userinfo['mail']);
    $user->save();
    $uid=$user->id();
    $user = User::load($uid);
    user_login_finalize($user);
    return new \Symfony\Component\HttpFoundation\RedirectResponse(\Drupal::url('user.page'));
  }
}
