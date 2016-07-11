<?php
/**
 * @file
 * Contains form for customer setup.
 */


/**
 * Form for setting up Customer Account.
 */
function gluu_sso_general_setup($form, &$form_state){
  global $base_url;
  $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files/images';
  $current_status = variable_get('oxd_id');
  $gluu_config = variable_get('gluu_config');
  if ($current_status) {
    $img_path = drupal_get_path('module', 'gluu_sso');
    $form['markup_2'] = array(
        '#markup' => '<fieldset style="border: 2px solid #53cc6b;">
                        <legend>
                            <div class="about">
                                <img style="width: 45px; height: 45px" src="' . url($bu . '/icons/ox.png') . '">server config
                            </div>
                        </legend>
                        <table class="form-table" style="margin-left: 30px; margin-right: 30px; width: 95% !important;">
                            <tbody>
                            <tr>
                                <th scope="row" style="width: 20% !important;">
                                    OXD id
                                </th>
                                <td>',
    );
    $form['gluu_sso_oxd_id'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#disabled' => true,
        '#value' => variable_get('oxd_id')
    );
    $form['markup_3'] = array(
        '#markup' => '</td>
                    </tr>
                </tbody>
            </table>
        </fieldset>',
    );
    $form['gluu_sso_general_setup_back'] = array(
        '#type' => 'submit',
        '#value' => t('Reset configurations'),
        '#submit' => array('gluu_sso_back'),
    );
    return $form;
  } else {
    $form['markup_14'] = array(
        '#markup' => '<div class="mess_red">Please enter OXD configuration to continue.</div><h3>Register your site with an OpenID Connect Provider.</h3>',
    );
    $form['markup_15'] = array(
        '#markup' => '<div class="mess_red">If you do not have an OpenID Connect provider, you may want to look at the Gluu Server (
                        <a target="_blank" href="http://www.gluu.org/docs">Like Drupal, there is a free open source Community Edition. For more information about Gluu Server support please visit <a target="_blank" href="http://www.gluu.org">our website.</a></a>)
            </div>
            <div class="mess_red">
                <h3>Instructions to Install oxd server</h3>
                <br><b>NOTE:</b> The oxd server should be installed on the same server as your Drupal site. It is recommended that the oxd server listen only on the localhost interface, so only your local applications can reach its API\'s.
                <ol style="list-style:decimal !important; margin: 30px">
                    <li>Extract and copy in your DMZ Server.</li>
                    <li>Download the latest oxd-server package for Centos or Ubuntu. See
                    <a target="_blank" href="http://gluu.org/docs-oxd">oxd docs</a> for more info.
                    </li><li>If you are installing an .rpm or .deb, make sure you have Java in your server.
                    </li><li>Edit <b>oxd-conf.json</b> in the <b>conf</b> directory to specify the port on which
                    it will run, and specify the hostname of the OpenID Connect provider.</li>
                    <li>Open the command line and navigate to the extracted folder in the <b>bin</b> directory.</li>
                    <li>For Linux environment, run <b>sh oxd-start.sh &amp;</b></li>
                    <li>For Windows environment, run <b>oxd-start.bat</b></li>
                    <li>After the server starts, set the port number and your email in this page and click Next.</li>
                </ol>
            </div>
            <hr>',
    );
    $form['gluu_sso_admin_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Admin Email'),
        '#required' => TRUE,
        '#value' => variable_get('site_mail', ini_get('sendmail_from'))
    );
    $form['gluu_sso_oxd_port'] = array(
        '#type' => 'textfield',
        '#title' => t('Oxd port in your server'),
        '#required' => TRUE,
        '#value' => $gluu_config['oxd_host_port']
    );
    $form['markup_17'] = array(
        '#markup' => '<b style="color: red">NOTE:</b> It must be free port in your server.<br/><br/>',
    );
    $form['gluu_sso_general_setup_button'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
        '#submit' => array('gluu_sso_general_setup_submit'),
    );
    return $form;
  }
}

function oxd_openid_check_empty_or_null( $value ) {
  if( ! isset( $value ) || empty( $value ) ) {
    return true;
  }
  return false;
}

/**
 * Handle submit for customer setup.
 */
function gluu_sso_general_setup_submit(&$form, $form_state) {
  global $base_url;
  $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files/images';
  $authorization_redirect_uri = $base_url.'?gluuOption=oxdOpenId';
  $logout_redirect_uri = $base_url.'/index.php?option=allLogout';
  $admin_email = $_POST['gluu_sso_admin_email'];
  $oxd_port = $_POST['gluu_sso_oxd_port'];

  $config_option = array(
      "oxd_host_ip" => '127.0.0.1',
      "reg_email" => $admin_email,
      "oxd_host_port" =>$oxd_port,
      "authorization_redirect_uri" => $authorization_redirect_uri,
      "logout_redirect_uri" => $logout_redirect_uri,
      "scope" => [ "openid", "profile","email","address", "clientinfo", "mobile_phone", "phone"],
      "application_type" => "web",
      "redirect_uris" => [ $authorization_redirect_uri ],
      "response_types" => ["code"],
      "grant_types" =>["authorization_code"],
      "acr_values" => [],
      "am_host" =>""
  );
  variable_set( 'gluu_config', $config_option );
  $custom_scripts = variable_get('oxd_openid_custom_scripts');
  if(empty($custom_scripts)) {
    $img_path = drupal_get_path('module', 'gluu_sso');
    $custom_scripts = array(
        array('name'=>'Google','image'=>url( $bu.'/icons/google.png'),'value'=>'gplus'),
        array('name'=>'Basic','image'=>url(  $bu.'/icons/basic.png'),'value'=>'basic'),
        array('name'=>'Duo','image'=>url(  $bu.'/icons/duo.png'),'value'=>'duo'),
        array('name'=>'U2F token','image'=>url(  $bu.'/icons/u2f.png'),'value'=>'u2f')
    );
    variable_set('oxd_openid_custom_scripts',$custom_scripts);
  }
  $scops = variable_get('gluu_scops');
  if(empty($scops)) {
    variable_set('gluu_scops',array("openid", "profile","email","address", "clientinfo", "mobile_phone", "phone"));
  }
  $register_site = new Register_site();
  $register_site->setRequestAcrValues($config_option['acr_values']);
  $register_site->setRequestAuthorizationRedirectUri($config_option['authorization_redirect_uri']);
  $register_site->setRequestRedirectUris($config_option['redirect_uris']);
  $register_site->setRequestGrantTypes($config_option['grant_types']);
  $register_site->setRequestResponseTypes(['code']);
  $register_site->setRequestLogoutRedirectUri($config_option['logout_redirect_uri']);
  $register_site->setRequestContacts([$admin_email]);
  $register_site->setRequestApplicationType('web');
  $register_site->setRequestClientLogoutUri($config_option['logout_redirect_uri']);
  $register_site->setRequestScope($config_option['scope']);
  $status = $register_site->request();
  if(!$status['status']){
    drupal_set_message( t( $status['message']), 'error');
    return;
  }
  if($register_site->getResponseOxdId()){
    if(variable_get('oxd_id')){
      variable_set( 'oxd_id', $register_site->getResponseOxdId() );
    }else{
      variable_set( 'oxd_id', $register_site->getResponseOxdId() );
    }
    drupal_set_message(t('Your settings are saved successfully. oxd_id= '.$register_site->getResponseOxdId()));
    return;
  }else{
    drupal_set_message(t('Gluu server url, oxd ip or oxd host is not a valid.'), 'error');
    return;
  }
}

/**
 * Handle back button submit for customer setup.
 */
function gluu_sso_back(&$form, $form_state) {
  global $base_url;
  $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files/images';
  $custom_scripts = variable_get('oxd_openid_custom_scripts');
  if($custom_scripts){
    foreach($custom_scripts as $custom_script){
      if(variable_get('oxd_openid_'.$custom_script['value'].'_enable')){
        variable_del('oxd_openid_'.$custom_script['value'].'_enable');
      }
    }
    variable_del('oxd_openid_custom_scripts');
  }
  if(variable_get('oxd_id')){
    variable_del('oxd_id');
  }
  if(variable_get('gluu_scops')){
    variable_del('gluu_scops');
  }
  if(variable_get('gluu_config')){
    variable_del('gluu_config');
  }
  if(variable_get('oxd_openid_login_theme')){
    variable_del('oxd_openid_login_theme');
  }
  if(variable_get('oxd_openid_login_redirect')){
    variable_del('oxd_openid_login_redirect');
  }
  if(variable_get('oxd_openid_login_theme')){
    variable_del('oxd_openid_login_theme');
  }
  if(variable_get('oxd_openid_default_login_enable')){
    variable_del('oxd_openid_default_login_enable');
  }
  if(variable_get('oxd_openid_login_widget_customize_text')){
    variable_del('oxd_openid_login_widget_customize_text');
  }
  if(variable_get('oxd_openid_login_button_customize_text')){
    variable_del('oxd_openid_login_button_customize_text');
  }
  if(variable_get('oxd_login_icon_custom_size')){
    variable_del('oxd_login_icon_custom_size');
  }
  if(variable_get('oxd_login_icon_space')){
    variable_del('oxd_login_icon_space');
  }
  if(variable_get('oxd_login_icon_custom_width')){
    variable_del('oxd_login_icon_custom_width');
  }
  if(variable_get('oxd_login_icon_custom_height')){
    variable_del('oxd_login_icon_custom_height');
  }
  if(variable_get('oxd_openid_login_custom_theme')){
    variable_del('oxd_openid_login_custom_theme');
  }
  if(variable_get('oxd_login_icon_custom_color')){
    variable_del('oxd_login_icon_custom_color');
  }
  global $base_url;
  $authorization_redirect_uri = $base_url.'?gluuOption=oxdOpenId';
  $logout_redirect_uri = $base_url.'/index.php?option=allLogout';
  $img_path = drupal_get_path('module', 'gluu_sso');
  variable_set('gluu_scops',array("openid", "profile","email","address", "clientinfo", "mobile_phone", "phone"));
  $custom_scripts = array(
      array('name'=>'Google','image'=>url( $bu.'/icons/google.png'),'value'=>'gplus'),
      array('name'=>'Basic','image'=>url( $bu.'/icons/basic.png'),'value'=>'basic'),
      array('name'=>'Duo','image'=>url( $bu.'/icons/duo.png'),'value'=>'duo'),
      array('name'=>'U2F token','image'=>url( $bu.'/icons/u2f.png'),'value'=>'u2f')
  );
  variable_set('oxd_openid_custom_scripts',$custom_scripts);
  $config_option = array(
      "oxd_host_ip" => '127.0.0.1',
      "oxd_host_port" =>8099,
      "authorization_redirect_uri" => $authorization_redirect_uri,
      "logout_redirect_uri" => $logout_redirect_uri,
      "scope" => [ "openid", "profile","email","address", "clientinfo", "mobile_phone", "phone"],
      "application_type" => "web",
      "redirect_uris" => [ $authorization_redirect_uri ],
      "response_types" => ["code"],
      "gluu_url" => '',
      "grant_types" =>["authorization_code"],
      "acr_values" => [],
      "admin_email"=>variable_get('site_mail', ini_get('sendmail_from')),
      "am_host" =>""
  );
  variable_set('gluu_config', $config_option);
  variable_set('oxd_openid_login_redirect', 'same' );
  variable_set('oxd_openid_login_theme', 'oval' );
  variable_set('oxd_login_icon_custom_size','40');
  variable_set('oxd_login_icon_space','5');
  variable_set('oxd_login_icon_custom_width','200');
  variable_set('oxd_login_icon_custom_height','40');
  variable_set('oxd_openid_login_custom_theme', 'default' );
  variable_set('oxd_login_icon_custom_color', '2B41FF' );
  drupal_set_message(t('All configurations deleted successfully!'));
}
