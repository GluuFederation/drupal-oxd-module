<?php

namespace Drupal\gluu_sso\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Database\Connection;
use Drupal\gluu_sso\Plugin\oxds\Register_site;

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
    // '#required' => TRUE,
	);
	$form['general']['customurl'] = array(
	  '#type' => 'textfield',
	  '#title' => $this->t('Custom url after logout'),
    '#default_value' => $config->get('logouturl'),
    // '#required' => TRUE,
	);
	$form['general']['oxd-port'] = array(
	  '#type' => 'textfield',
	  '#title' => $this->t('Oxd-Port'),
    '#default_value' => $config->get('oxd_port'),
    // '#required' => TRUE,
  );
  $form['general']['oxd-id'] = array(
	  '#type' => 'textfield',
	  '#title' => $this->t('Oxd-id:'),
    '#default_value' => $config->get('oxd_id'),
    '#disabled' => TRUE,
  );
	$form['general']['enrollment'] = array(
	    '#type' => 'radios',
		'#title' => $this->t('Enrollement'),
		'#default_value' => $config->get('enrollement'),
		'#options' => array(1 => $this->t('Automatic Regsiter any user with an account in the openid provider'), 0 => $this->t('Disable Automatic Registration')),
	);
  $form['general']['user_type'] = array(
	   '#type' => 'select',
		'#title' => $this->t('New User Default Role:'),
		'#default_value' => $config->get('enrollement'),
		'#options' => array(1 => $this->t('Regular User'), 2 => $this->t('System Administrator User')),
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
    '#default_value' => 'openid',
    'visible' => array(
      ':input[name="check_box"]' => array('checked' => TRUE),
    ),
	);
	$form['Documentation'] = array(
	  '#type' => 'details',
	  '#title' => $this->t('Documentation'),
	  '#group' => 'information',
	);
    return parent::buildForm($form, $form_state,$base_url);
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

    $oxd_port=$form_state->getValue('oxd-port');
    $oxdprovider=$form_state->getValue('url');
    $currentPath = $_SERVER['PHP_SELF'];
    $pathInfo = pathinfo($currentPath);
    $hostName = $_SERVER['HTTP_HOST'];
    $protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";
    if (strpos($pathInfo['dirname'], '\\') !== false) {
      $base_url=$protocol . $hostName . "/";
		} else {
      $base_url=$protocol . $hostName . $pathInfo['dirname'] . "/";
		}
    $base_url=$protocol . $hostName . "/";
    $account = \Drupal::currentUser();
     if ($account->id() == 1) {
       if (isset($_REQUEST['form_key']) and strpos($_REQUEST['form_key'], 'general_register_page') !== false) {
           if (!isset($_SERVER['HTTPS']) or $_SERVER['HTTPS'] != "on") {
               drupal_set_message(t('OpenID Connect requires https. This plugin will not work if your website uses http only.'), 'error');
               header("Location: " . $base_url . "admin/config/gluu_sso/default");
                return;
           }
          $config = $this->config('gluu_sso.default');
          $connection = @fsockopen('127.0.0.1', $config->get('oxd_port'));
          if (is_resource($connection)) {
      			fclose($connection);
          } else {
            drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'), 'error');
            header("Location: " . $base_url . "admin/config/gluu_sso/default");

      		}
          if (empty($form_state->getValue('oxd-port'))) {
            drupal_set_message(t('All the fields are required. Please enter valid entries.'), 'error');
    				header("Location: " . $base_url . "admin/config/gluu_sso/default");

    			}
    			if (intval($oxd_port) > 65535 && intval($oxd_port) < 0) {
            drupal_set_message(t('Enter your oxd host port (Min. number 1, Max. number 65535)'), 'error');
    				header("Location: " . $base_url . "admin/config/gluu_sso/default");
          }
          if (!empty($form_state->getValue('url'))) {
            if (filter_var($oxdprovider, FILTER_VALIDATE_URL) === false) {
              drupal_set_message(t('Please enter valid OpenID Provider URI.'), 'error');
    					header("Location: " . $base_url . "admin/config/gluu_sso/default");
              }
    		 }
         if (!empty($form_state->getValue('customurl'))) {
            $gluu_logout_url=$form_state->getValue('customurl');
     				if (filter_var($gluu_logout_url, FILTER_VALIDATE_URL) === false) {
                  drupal_set_message(t('Please enter valid Custom URI.)'), 'error');
     					}
          }
          if (isset($form_state->getValue('url')) and !empty($form_state->getValue('url'))) {
            $oxdprovider=$form_state->getValue('url');
            $config->set('gluu_provider', $oxdprovider);
            $arrContextOptions = array(
    					"ssl" => array(
    						"verify_peer" => false,
    						"verify_peer_name" => false,
    					),
    				);
    				$json = file_get_contents($oxdprovider . '/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));
            $obj = json_decode($json);
            if (!empty($obj->userinfo_endpoint)) {
              if (empty($obj->registration_endpoint)) {
                drupal_set_message(t('Please enter your client_id and client_secret.'), 'status');
                $gluu_config = array(
    								"gluu_oxd_port" => $form_state->getValue('oxd-port'),
    								"admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
    								"authorization_redirect_uri" => $base_url . 'index.php?gluuOption=oxdOpenId',
    								"post_logout_redirect_uri" => $base_url . 'index.php?option=allLogout',
    								"config_scopes" => ["openid", "profile", "email"],
    								// "gluu_client_id" => $_POST['gluu_client_id'],
    								// "gluu_client_secret" => $_POST['gluu_client_secret'],
    								"config_acr" => ["basic"]
    							);
                $register_site = new Register_site();
  							$register_site->setRequestOpHost($gluu_provider);
  							$register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
  							$register_site->setRequestPostLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
  							$register_site->setRequestContacts([$gluu_config['admin_email']]);
  							$get_scopes = $obj->scopes_supported;

              }
            }

          $values = array(
              'url' => $form_state->getValue('url'),
              'customurl'  => $form_state->getValue('customurl'),
              'oxd-port'   => $form_state->getValue('oxd-port'),
              'enrollment' => $form_state->getValue('enrollment'),
              'scopes'     => $form_state->getValue('scopes'),
          );
          $connection = \Drupal\Core\Database\Database::getConnection();
          $results = $connection->query('select  * from gluu_sso_settings')->fetchAll();
          if(empty($results)){

            db_insert('gluu_sso_settings')
              -> fields(array(
                'openidurl' => $values['url'],
                'logouturl' => $values['customurl'],
                'oxd_port' =>  $values['oxd-port'],
                'enrollement'=>$values['enrollment'],
                'requestedscopes'=>'openid',
                'newuserdefaultrole'=>'nn',
                'acr_values'=>'none',
              ))
              ->execute();
            }
            else
            {
              $connection->update('gluu_sso_settings')
                  ->fields(array(
                    'openidurl' => $values['url'],
                    'logouturl' => $values['customurl'],
                    'oxd_port' =>  $values['oxd-port'],
                    'enrollement'=>$values['enrollment'],
                    'requestedscopes'=>'openid',
                    'newuserdefaultrole'=>'nn',
                    'acr_values'=>'none',
                  ))
                  ->condition('id', 1, '=')
                  ->execute();
            }
            $config = $this->config('gluu_sso.default');
            $config->set('openidurl', $values['url']);
            $config->set('logouturl', $values['customurl']);
            $config->set('oxd_port', $values['oxd-port']);
            $config->set('enrollement', $values['enrollment']);
            $config->set('requestedscopes', $values['scopes']);
            $config->save();
          }
        }
      }
  }
}
