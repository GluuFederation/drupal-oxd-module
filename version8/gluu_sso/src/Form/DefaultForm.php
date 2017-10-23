<?php

namespace Drupal\gluu_sso\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Database\Connection;
use Drupal\gluu_sso\Plugin\oxds\Setup_client;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Drupal\user;

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
        $oxdprovider = $config->get('openidurl');
        $acr = $config->get('user_acr');
        $oxdid = $config->get('gluu_oxd_id');
        $client_id = $config->get('client_id', 'show');
        if ($client_id == 'show') {
            $type = 'textfield';
            $title_id = 'Client ID:';
        } else {
            $type = 'hidden';
            $title_id = '';
        }
        $client_secret = $config->get('client_secret', 'show');
        if ($client_secret == 'show') {
            $type = 'textfield';
            $title_secret = 'Client Secret:';
        } else {
            $type = 'hidden';
            $title_secret = '';
        }
        if ($oxdprovider == '') {
            $disabled = 'FALSE';
        } else {
            $disabled = 'TRUE';
        }
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
            '#title' => '<div style="padding-left: 30px;font-weight: 100;">
                                    <p>The oxd OpenID Connect single sign-on (SSO) plugin for Drupal enables you to use a standard OpenID Connect Provider (OP), like Google or the Gluu Server, to authenticate and enroll users for your Drupal site.</p>
                                    <p>This plugin relies on the oxd mediator service. For oxd deployment instructions and license information, please visit the <a href="https://oxd.gluu.org/">oxd website</a>.</p>
                                    <p>In addition, if you want to host your own OP you can deploy the <a href="https://www.gluu.org/">free open source Gluu Server</a>.</p>
                                </div>
                                <hr/>
                                <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%; font-weight: bold ">Server Settings</h3>
                                <p style="font-weight: 100;"><i>The below values are required to configure your Drupal site with your oxd server and OP. Upon successful registration of your Drupal site in the OP, a unique identifier will be issued and displayed below in a new field called: oxd ID.</i></p>'.
                                $this->t('URI of the OpenID Connect Provider'),
            '#default_value' => $config->get('openidurl'),
            '#disabled' => $config->get('disabledopenurl'),
        );
        $form['general']['gluu_client_id'] = array(
            '#type' => $type,
            '#title' => $this->t($title_id),
            '#default_value' => $config->get('gluu_client_id'),
        );
        $form['general']['gluu_client_secret'] = array(
            '#type' => $type,
            '#title' => $this->t($title_secret),
            '#default_value' => $config->get('gluu_client_secret'),
        );
        $form['general']['customurl'] = array(
            '#type' => 'textfield',
            '#title' => $this->t('Custom url after logout'),
            '#default_value' => $config->get('gluu_custom_logout'),
        );
        $base_url = self::gluu_sso_get_base_url_workflow();
        if($config->get('gluu_oxd_id')){
            $form['general']['redirecturl'] = array(
                '#type' => 'textfield',
                '#title' => $this->t('Redirect URL'),
                '#default_value' => $base_url . "gluu_sso/gluusloginredirect.php",
                '#disabled' => TRUE,
            );
        }
        $form['general']['connection_type'] = array(
            '#type' => 'radios',
            '#title' => $this->t('Select oxd server / oxd https extension').'<a data-toggle="tooltip" class="tooltipLink" title="If you are using localhost to connect your drupal 7 site to your oxd server, choose oxd server. If you are connecting via https, choose oxd https extension.">
                                                    <span class="glyphicon glyphicon-info-sign"></span>
                                                </a>',
            '#default_value' => $config->get('connection_type') ? $config->get('connection_type') : 1,
            '#options' => array(1 => $this->t('oxd server'), 2 => $this->t('oxd https extension')),
        );
        $form['general']['oxd-port'] = array(
            '#type' => 'textfield',
            '#title' => $this->t('oxd server port'),
            '#attributes' => ['class' => ['port']],
            '#default_value' => $config->get('oxd_port'),
        );
        $form['general']['oxd-Web-Host'] = array(
            '#type' => 'textfield',
            '#title' => $this->t('oxd https extension host'),
            '#attributes' => ['class' => ['host']],
            '#default_value' => $config->get('oxd_web_host'),
        );
        $form['general']['oxd-id'] = array(
            '#type' => 'textfield',
            '#title' => $this->t('oxd ID:'),
            '#default_value' => $config->get('gluu_oxd_id'),
            '#disabled' => TRUE,
        );
        $form['general']['enrollment'] = array(
            '#type' => 'radios',
            '#title' => $this->t('Enrollment and Access Management').'<a data-toggle="tooltip" class="tooltipLink" title="Choose whether to register new users when they login at an external identity provider. If you disable automatic registration, new users will need to be manually created">
                                        <span class="glyphicon glyphicon-info-sign"></span>
                                    </a>',
            '#default_value' => $config->get('gluu_users_can_register'),
            '#options' => array(1 => $this->t('Automatic Regsiter any user with an account in the openid provider'), 2=>$this->t('Only register and allow ongoing access to users with one or more of the following roles in the OpenID Provider'), 3 => $this->t('Disable Automatic Registration')),
            '#attributes' => array('data-gluu-roles' => $config->get('gluu_new_roles'))
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
            '#options' => array('openid' => $this->t('openid'), 'email' => $this->t('email'), 'profile' => $this->t('profile'), 'permission' => $this->t('permission'), 'IMapdata' => $this->t('IMapdata'), 'clientinfo' => $this->t('clientinfo'), 'address' => $this->t('address')),
            '#title' => $this->t('Requested Scopes'),
            '#default_value' => $config->get('gluu_scopes'),
        );
        $form['openid']['user_acr'] = array(
            '#type' => 'select',
            '#title' => $this->t('Select ACR:'),
            '#default_value' => $config->get('user_acr'),
            '#options' => array('default' => $this->t('none'), 'passport' => $this->t('passport'), 'auth_ldap_server' => $this->t('auth_ldap_server'), 'u2f' => $this->t('u2f'), 'super_gluu' => $this->t('super_gluu'), 'asimba' => $this->t('asimba'), 'otp' => $this->t('otp'), 'basic' => $this->t('basic'), 'duo' => $this->t('duo')),
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
        $form['#attached'] = array('library' => array('gluu_sso/gluu_ssojs'));//['library'][] = 'gluu_sso/gluu_ssojs';
        
        
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
    public function gluu_sso_get_base_url_workflow() {
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
    
    public function submitForm(array &$form, FormStateInterface $form_state) {
        
        $base_url = self::gluu_sso_get_base_url_workflow();
        $account = \Drupal::currentUser();
        /*         * ************************************form posted values****************************** */
        $config = $this->config('gluu_sso.default');
        $values = array(
            'gluu_provider' => $form_state->getValue('url'),
            'gluu_custom_logout' => $form_state->getValue('customurl'),
            'gluu_oxd_port' => $form_state->getValue('oxd-port'),
            'enrollment' => $form_state->getValue('enrollment'),
            'scopes' => $form_state->getValue('scopes'),
            'user_acr' => $form_state->getValue('user_acr'),
            'gluu_client_id' => $form_state->getValue('gluu_client_id'),
            'gluu_client_secret' => $form_state->getValue('gluu_client_secret'),
            'user_type' => $form_state->getValue('user_type'),
            'connection_type' => $form_state->getValue('connection_type'),
            'oxd_web_host' => $form_state->getValue('oxd-Web-Host'),
            'gluu_new_roles' => $_POST['gluu_new_role']
        );
        /*         * ***************************checking roles and ssl********************************** */
        if ($account->id() == 1) {

            if (isset($_REQUEST['form_key']) and strpos($_REQUEST['form_key'], 'general_register_page') !== false) {
                /*                 * ******************************validation phase start**************************** */
                //checking ssl activation
                if (!isset($_SERVER['HTTPS']) or $_SERVER['HTTPS'] != "on") {
                    drupal_set_message(t('OpenID Connect requires https. This plugin will not work if your website uses http only.'), 'error');
                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                    return $response;
                }
                if($values['connection_type'] == 1){
                    if (empty($values['gluu_oxd_port'])) {
                        drupal_set_message(t('Enter your oxd server port.'), 'error');
                        $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                        return $response;
                    }
                    
                    if (intval($values['gluu_oxd_port']) > 65535 && intval($values['gluu_oxd_port']) < 0) {
                        drupal_set_message(t('Enter your oxd server port (Min. number 1, Max. number 65535)'), 'error');
                        $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                        return $response;
                    }
                } else {
                    if (empty($values['oxd_web_host'])) {
                        drupal_set_message(t('Enter your oxd https extension host.'), 'error');
                        $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                        return $response;
                    }
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
                    $gluu_config = $config->get("gluu_config");
                    foreach ($values['scopes'] as $scope) {
                        if ($scope && !in_array($scope, $get_scopes)) {
                            array_push($gluu_config['scope'], $scope);
                        }
                    }
                    $config->set('gluu_config', $gluu_config);
                    $config->save();
                    $gluuconfig = $config->get('gluu_config');
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
                
                if(!$values['enrollment']){
                    drupal_set_message(t('Select Enrollment and Access Management Option.'), 'error');
                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                    return $response;
                }
                
                if ($values['enrollment'] == 3) {
                    $config->set('gluu_users_can_register', 3);
                    $config->clear('gluu_new_roles');
                    $config->save();
                } else if ($values['enrollment'] == 2) {
                    $config->set('gluu_users_can_register', 2);
                    $config->set('gluu_new_roles', $_POST['gluu_new_role']);
                    $config->save();
                } else {
                    $config->set('gluu_users_can_register', 1);
                    $config->clear('gluu_new_roles');
                    $config->save();
                }
                if (isset($values['user_acr'])) {
                    $config->set('user_acr', $values['user_acr']);
                    $config->save();
                }
                //checking gluu provider services
                $gluu_provider = $form_state->getValue('url');
                if (isset($gluu_provider) and ! empty($gluu_provider)) {
                    $arrContextOptions = array(
                        "ssl" => array(
                            "verify_peer" => false,
                            "verify_peer_name" => false,
                        ),
                    );
                    $json = file_get_contents($gluu_provider . '/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));
                    $obj = json_decode($json);
                    $config->set('openidurl', $gluu_provider);
                    $config->set('oxd_port', $values['gluu_oxd_port']);
                    $config->set('connection_type', $values['connection_type']);
                    $config->set('oxd_web_host', $values['oxd_web_host']);
                    $config->save();
                    $gluu_provider = $config->get('openidurl');
                    if (!empty($obj->userinfo_endpoint)) {
                        if (empty($obj->registration_endpoint)) {
                            drupal_set_message(t('Please enter your client_id and client_secret.'), 'status');
                            $config->set('client_id', 'show');
                            $config->set('client_secret', 'show');
                            $config->save();
                            //code saving client id and secret key
                            $gluu_config = array(
                                "gluu_oxd_port" => $_POST['gluu_oxd_port'],
//                                "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                                "authorization_redirect_uri" => $base_url . 'index.php?gluuOption=oxdOpenId',
                                "post_logout_redirect_uri" => $base_url . 'index.php?option=allLogout',
                                "config_scopes" => ["openid", "profile", "email"],
                                "gluu_client_id" => "",
                                "gluu_client_secret" => "",
                                "config_acr" => [],
                                "has_registration_endpoint" => false,
                                "connection_type" => $values['connection_type'],
                                "oxd_web_host" => $values['oxd_web_host']
                            );
                            
                            if (isset($values['gluu_client_id']) && !empty($values['gluu_client_id']) &&
                                    isset($values['gluu_client_secret']) && !empty($values['gluu_client_secret'])
                            ) {
                                $gluu_config = array(
                                    "op_host" => $gluu_provider, 
                                    "oxd_host_port" => $values['gluu_oxd_port'], 
                                    "authorization_redirect_uri" => $base_url . "gluu_sso/gluusloginredirect.php", 
                                    "post_logout_redirect_uri" => $base_url . "admin/config/gluu_sso/default", 
                                    "scope" => [ "openid", "profile", "email"], 
                                    "application_type" => "web", "response_types" => ["code"], 
                                    "grant_types" => ["authorization_code"], "config_acr" => [], 
                                    "gluu_client_id" => $values['gluu_client_id'], 
                                    "gluu_client_secret" => $values['gluu_client_secret'],
                                    "has_registration_endpoint" => false,
                                    "connection_type" => $values['connection_type'],
                                    "oxd_web_host" => $values['oxd_web_host']
                                );
                                $config->set('gluu_config', $gluu_config);
                                $config->save();
//                                if (!self::gluu_sso_is_port_working_workflow()) {
//                                    drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'), 'error');
//                                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
//                                    return $response;
//                                }
                                $register_site = new Setup_client();
                                $register_site->setRequestOpHost($gluu_config['op_host']);
                                $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                                $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                                $register_site->setRequestContacts(array(\Drupal::config('system.site')->get('mail')));
                                $register_site->setRequestGrantTypes($gluu_config['grant_types']);
                                $register_site->setRequestResponseTypes($gluu_config['response_types']);
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
                                if($gluu_config['connection_type'] == 2){
                                    $status = $register_site->request(rtrim($gluu_config['oxd_web_host'],'/')."/setup-client");
                                }else{
                                    $status = $register_site->request();
                                }
                                if ($status['message'] == 'invalid_op_host') {
                                    drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"), 'error');
                                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                    return $response;
                                }
                                if ($status['message'] == 'internal_error') {
                                    drupal_set_message(t('message_error', 'ERROR: ' . $status['error_message']), 'error');
                                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                    return $response;
                                }
                                $gluu_oxd_id = $register_site->getResponseOxdId();
                                //var_dump($register_site->getResponseObject());exit;
                                if ($gluu_oxd_id) {
                                    $config->set('gluu_oxd_id', $gluu_oxd_id);
                                    $config->save();
                                    $gluu_provider = $register_site->getResponseOpHost();
                                    $gluu_config['gluu_client_id'] = $register_site->getResponse_client_id();
                                    $gluu_config['gluu_client_secret'] = $register_site->getResponse_client_secret();
                                    $config->set('gluu_config', $gluu_config);
                                    $config->set('gluu_client_id', $register_site->getResponse_client_id());
                                    $config->set('gluu_client_secret', $register_site->getResponse_client_secret());
                                    $config->save();
                                    $config->set('gluu_provider', $gluu_provider);
                                    $config->save();
                                    drupal_set_message(t('Your settings are saved successfully.'), 'status');
                                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                    return $response;
                                } else {
                                    drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"), 'error');
                                    $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                    return $response;
                                }
                            } else {
//                                drupal_set_message(t('openid_error'), 'error');
                                $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                return $response;
                            }
                        } else {  // without client id and secret key code
                            $gluu_provider = $config->get('openidurl');
                            $gluu_config = array(
                                    "op_host" => $gluu_provider, 
                                    "oxd_host_port" => $values['gluu_oxd_port'], 
                                    "authorization_redirect_uri" => $base_url . "gluu_sso/gluusloginredirect.php", 
                                    "post_logout_redirect_uri" => $base_url . "admin/config/gluu_sso/default",
                                    "scope" => [ "openid", "profile", "email"], 
                                    "application_type" => "web", 
                                    "response_types" => ["code"], 
                                    "grant_types" => ["authorization_code"], 
                                    "config_acr" => [],
                                    "has_registration_endpoint" => true,
                                    "connection_type" => $values['connection_type'],
                                    "oxd_web_host" => $values['oxd_web_host']
                                );
                            $config->set('gluu_config', $gluu_config);
                            $config->save();
//                            if (!self::gluu_sso_is_port_working_workflow()) {
//                                drupal_set_message(t('Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.'), 'error');
//                                $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
//                                return $response;
//                            }
                            $register_site = new Setup_client();
                            $register_site->setRequestOpHost($gluu_config['op_host']);
                            $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                            $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                            $register_site->setRequestContacts(array(\Drupal::config('system.site')->get('mail')));
                            $register_site->setRequestGrantTypes($gluu_config['grant_types']);
                            $register_site->setRequestResponseTypes($gluu_config['response_types']);
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
                            if($gluu_config['connection_type'] == 2){
                                $status = $register_site->request(rtrim($gluu_config['oxd_web_host'],'/')."/setup-client");
                            }else{
                                $status = $register_site->request();
                            }
                            $gluu_oxd_id = $register_site->getResponseOxdId();
                            if ($gluu_oxd_id) {
                                $config = $this->config('gluu_sso.default');
                                $config->set('gluu_oxd_id', $gluu_oxd_id);
                                $config->save();
                                $gluu_config['gluu_client_id'] = $register_site->getResponse_client_id();
                                $gluu_config['gluu_client_secret'] = $register_site->getResponse_client_secret();
                                $config->set('gluu_config', $gluu_config);
                                $config->set('gluu_client_id', $register_site->getResponse_client_id());
                                $config->set('gluu_client_secret', $register_site->getResponse_client_secret());
                                $config->save();
                                $register_site->getResponseOpHost();
                                $config->set('gluu_provider', $gluu_provider);
                                $config->save();
                                drupal_set_message(t('Your settings are saved successfully.'), 'status');
                                $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                return $response;
                            } else {
                                drupal_set_message(t("ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json"), 'error');
                                $response = new TrustedRedirectResponse($base_url . "admin/config/gluu_sso/default");
                                return $response;
                            }
                        }
                    }
                }//checking gluu server end points
            }
            /*             * ******************************************validation phase end************************************* */
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
        $config->clear('oxd_web_host');
        $config->clear('connection_type');
        $config->clear('gluu_config');
        $config->clear('gluu_client_id');
        $config->clear('gluu_client_secret');
        $config->clear('client_id');
        $config->clear('client_secret');
        $config->clear('gluu_new_roles');
        $config->save();
        drupal_set_message("Reset successfully");
    }

}
