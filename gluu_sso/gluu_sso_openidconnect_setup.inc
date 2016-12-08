<?php
/**
 * @file
 * Contains form for customer setup.
 */
//var_dump(variable_get('gluu_config'));exit;
function add_js_css_fonts() {
    drupal_add_js('https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js', 'external');
    drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/gluu-oxd-css.css');
    drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/gluu-oxd-css1.css');
    drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/bootstrap.css');
    drupal_add_js('https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js', 'external');
    drupal_add_js(drupal_get_path('module', 'gluu_sso') . '/files/js/scope-custom-script.js' );

}
add_js_css_fonts() ;
/**
 * Form for setting up Customer Account.
 */
function gluu_sso_openidconnect_setup($form, &$form_state){
    global $base_url;
    $form['#action'] = url('/admin/config/people/gluu_sso/savedata');
    $form['markup_2'] = array(
        '#markup' => admin_openidconnect_html()
    );
    return $form;
}

function admin_openidconnect_html()
{
    $base_url = @($_SERVER["HTTPS"] != 'on') ? 'http://' : 'https://';
    $url = $_SERVER['REQUEST_URI']; //returns the current URL
    $parts = explode('/', $url);
    $base_url .= $_SERVER['SERVER_NAME'];
    for ($i = 0; $i < count($parts) - 1; $i++) {
        $base_url .= $parts[$i] . "/";
    }
    $img_path = drupal_get_path('module', 'gluu_sso');
    $get_scopes = variable_get('gluu_scopes');
    $gluu_config = variable_get('gluu_config');
    $gluu_acr = variable_get('gluu_acr');
    $gluu_auth_type = variable_get('gluu_auth_type');
    $gluu_send_user_check = variable_get('gluu_send_user_check');
    $gluu_provider = variable_get('gluu_provider');
    $gluu_user_role = variable_get('gluu_user_role');
    $gluu_custom_logout = variable_get('gluu_custom_logout');
    $gluu_new_roles = variable_get('gluu_new_role');
    $gluu_users_can_register = variable_get('gluu_users_can_register');
    $message_error = variable_get('message_error');
    $message_success = variable_get('message_success');
    $openid_error = variable_get('openid_error');
    $html="
<script type='application/javascript'>
    jQuery(document).ready(function() {

        jQuery('[data-toggle=\"tooltip\"]').tooltip();
        jQuery('#p_role').on('click', 'a.remrole', function() {
            jQuery(this).parents('.role_p').remove();
        });

    });
    jQuery(document ).ready(function() {

        jQuery(\"input[name='scope[]']\").change(function(){
            var form=jQuery(\"#gluu-sso-openidconnect-setup\");
            if (jQuery(this).is(':checked')) {
                jQuery.ajax({
                    url: '/admin/config/people/gluu_sso/savedata',
                    type: 'POST',
                    data:form.serialize(),
                    success: function(result){
                        if(result){
                            return false;
                        }
                    }});
            }else{
                jQuery.ajax({
                    url: '/admin/config/people/gluu_sso/savedata',
                    type: 'POST',
                    data:form.serialize(),
                    success: function(result){
                        if(result){
                            return false;
                        }
                    }});
            }
        });

    });
    function delete_scopes(val){
        if (confirm(\"Are you sure that you want to delete this scope? You will no longer be able to request this user information from the OP.\")) {
            jQuery.ajax({
                url: '/admin/config/people/gluu_sso/savedata',
                type: 'POST',
                data:{form_key_scope_delete:'form_key_scope_delete', delete_scope:val},
                success: function(result){
                    location.reload();
                }});
        }
        else{
            return false;
        }

    }
    function add_scope_for_delete() {
        var striped = jQuery('#table-striped');
        var new_scope_field = jQuery('#new_scope_field').val();
        var m = true;
        if(new_scope_field){
            jQuery(\"input[name='scope[]']\").each(function(){
                // get name of input
                var value =  jQuery(this).attr(\"value\");
                if(value == new_scope_field){
                    m = false;
                }
            });
            if(m){
                jQuery('<tr >' +
                    '<td style=\"padding: 0px !important;\">' +
                    '   <p  id=\"'+new_scope_field+'\">' +
                    '     <input type=\"checkbox\" name=\"scope[]\" id=\"new_'+new_scope_field+'\" value=\"'+new_scope_field+'\"  />'+
                    '   </p>' +
                    '</td>' +
                    '<td style=\"padding: 0px !important;\">' +
                    '   <p  id=\"'+new_scope_field+'\">' +
                    new_scope_field+
                    '   </p>' +
                    '</td>' +
                    '<td style=\"padding: 0px !important; \">' +
                    '   <a href=\"#scop_section\" class=\"btn btn-danger btn-xs\" style=\"margin: 5px; float: right\" onclick=\"delete_scopes(\''+new_scope_field+'\')\" >' +
                    '<span class=\"glyphicon glyphicon-trash\"></span>' +
                    '</a>' +
                    '</td>' +
                    '</tr>').appendTo(striped);
                jQuery('#new_scope_field').val('');

                jQuery.ajax({
                    url: '/admin/config/people/gluu_sso/savedata',
                    type: 'POST',
                    data:{form_key_scope:'oxd_openid_config_new_scope', new_value_scope:new_scope_field},
                    success: function(result){
                        if(result){
                            return false;
                        }
                    }});
                jQuery(\"#new_\"+new_scope_field).change(
                    function(){
                        var form=jQuery(\"#gluu-sso-openidconnect-setup\");
                        if (jQuery(this).is(':checked')) {
                            jQuery.ajax({
                                url: '/admin/config/people/gluu_sso/savedata',
                                type: 'POST',
                                data:form.serialize(),
                                success: function(result){
                                    if(result){
                                        return false;
                                    }
                                }});
                        }else{
                            jQuery.ajax({
                                url: '/admin/config/people/gluu_sso/savedata',
                                type: 'POST',
                                data:form.serialize(),
                                success: function(result){
                                    if(result){
                                        return false;
                                    }
                                }});
                        }
                    });

                return false;
            }
            else{
                alert('The scope named '+new_scope_field+' is exist!');
                jQuery('#new_scope_field').val('');
                return false;
            }
        }else{
            alert('Please input scope name!');
            jQuery('#new_scope_field').val('');
            return false;
        }
    }
</script>";
    $html.=
        '
        <div id="messages">';
    if (!empty($message_error)) {
        $html .= '<div class="mess_red_error">' . $message_error . '</div>';
        variable_set('message_error', '');
    }
    if (!empty($message_success)) {
        $html .= '<div class="mess_green">' . $message_success . '</div>';
        variable_set('message_success', '');
    }
    $html .= '</div>
        <ul class="navbar navbar-tabs">
            <li id="account_setup"><a  style="cursor: pointer;" href="/admin/config/people/gluu_sso/general">General</a></li>';
    if (!gluu_is_oxd_registered()) {
        $html .= '<li  class="active" id="social-sharing-setup"><a  style="pointer-events: none; cursor: default;" >OpenID Connect Configuration</a></li>';
    } else {
        $html .= '<li  class="active" id="social-sharing-setup"><a  style="cursor: pointer;" href="/admin/config/people/gluu_sso/openidconnect">OpenID Connect Configuration</a></li>';
    }
    $html .= '<li id=""><a href="https://oxd.gluu.org/docs/plugin/drupal/" target="_blank">Documentation</a></li>';
    $html .= '</ul>
        <div class="container-page">';
    $html .= '            
            <div id="configopenid" style="padding: 20px !important;">
                    <input type="hidden" name="form_key" value="openid_config_page"/>
                    <fieldset style="border: 2px solid #53cc6b; padding: 20px">
                        <legend style="border-bottom:none; width: 110px !important;">
                            <img style=" height: 45px;" src="/'.$img_path.'/files/images/icons/gl.png"/>
                        </legend>
                        <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%; ">User Scopes</h3>
                        <div >
                            <table style="margin-left: 30px" >
                                <tr style="border-bottom: 1px solid green !important;">
                                    <th style="width: 230px; padding: 0px;text-transform: none !important;
     background: none !important;
     font-weight: normal;
     border:none !important; ">
                                        <h3 style="font-weight:bold; display: inline-flex" id="scop_section">
                                            Requested scopes
                                            <a data-toggle="tooltip" class="tooltipLink" data-original-title="Scopes are bundles of attributes that the OP stores about each user. It is recommended that you request the minimum set of scopes required">
                                                <span class="glyphicon glyphicon-info-sign"></span>
                                            </a>
                                        </h3>
                                    </th>
                                    <td style="width: 230px; padding-left: 10px !important">
                                            <table id="table-striped" class="form-table" >
                                                <tbody style="width: inherit !important;">
                                                <tr style="margin: 0px">
                                                    <td style="padding: 0px !important; width: 10%">
                                                        <p style="margin:0px !important;">
                                                            <input checked type="checkbox" name=""  id="openid" value="openid"  disabled />

                                                        </p>
                                                    </td>
                                                    <td style="padding: 0px !important; width: 75%">
                                                        <p style="margin:0px !important;">
                                                            <input type="hidden"  name="scope[]"  value="openid" />openid
                                                        </p>
                                                    </td>
                                                    <td style="padding: 0px !important;  width: 20%">
                                                        <a class="btn btn-danger btn-xs" style="margin: 5px; float: right" disabled><span class="glyphicon glyphicon-trash"></span></a>
                                                    </td>
                                                </tr>
                                                <tr style="padding: 0px">
                                                    <td style="padding: 0px !important; width: 10%">
                                                        <p style="margin:0px !important;">
                                                            <input checked type="checkbox" name=""  id="email" value="email"  disabled />

                                                        </p>
                                                    </td>
                                                    <td style="width: 70%">
                                                        <p style="margin:0px !important;">
                                                            <input type="hidden"  name="scope[]"  value="email" />email
                                                        </p>
                                                    </td>
                                                    <td style="padding: 0px !important;  width: 20%">
                                                        <a class="btn btn-danger btn-xs" style="margin: 5px; float: right" disabled><span class="glyphicon glyphicon-trash"></span></a>
                                                    </td>
                                                </tr>
                                                <tr style="padding: 0px">
                                                    <td style="padding: 0px !important; width: 10%">
                                                        <p style="margin:0px !important;">
                                                            <input checked type="checkbox" name=""  id="profile" value="profile"  disabled />

                                                        </p>
                                                    </td>
                                                    <td style="padding: 0px !important; width: 70%">
                                                        <p  style="margin:0px !important;">
                                                            <input type="hidden"  name="scope[]"  value="profile" />profile
                                                        </p>
                                                    </td>
                                                    <td style="padding: 0px !important;  width: 20%">
                                                        <a class="btn btn-danger btn-xs" style="margin: 5px; float: right" disabled><span class="glyphicon glyphicon-trash"></span></a>
                                                    </td>
                                                </tr>';
    foreach($get_scopes as $scop) {
        if ($scop == 'openid' or $scop == 'email' or $scop == 'profile'){
        } else{
            $html.='<tr style="padding: 0px">
                                                            <td>
                                                                <p  style="margin:0px !important;" id="'.$scop.'1">
                                                                    <input ';
            if($gluu_config && in_array($scop, $gluu_config['config_scopes'])){ $html.=" checked ";
            } $html.=' type="checkbox" name="scope[]"  id="'.$scop.'1" value="'.$scop.'" ';
            if (!gluu_is_oxd_registered() || $scop=='openid') $html.=' disabled ';
            $html.='/>
                                                                </p>
                                                            </td>
                                                            <td>
                                                                <p style="margin:0px !important;" id="'.$scop.'1">'.$scop.'
                                                                </p>
                                                            </td>
                                                            <td style="padding: 0px !important; ">
                                                                <button type="button" class="btn btn-danger btn-xs" style="margin: 5px; float: right" onclick="delete_scopes(\''.$scop.'\')" ><span class="glyphicon glyphicon-trash"></span></button>
                                                            </td>
                                                        </tr>';
        }
    }
    $html.='</tbody>
                                            </table>
                                    </td>
                                </tr>
                                <tr style="border-bottom: 1px solid green !important;padding: 10px 15px">
                                    <th style="border-bottom: 1px solid green !important;padding: 10px 15px;text-transform: none !important;
     background: none !important;
     font-weight: normal;
     border:none !important;">
                                        <h4 style="text-align: left;" id="scop_section1">
                                            Add scopes
                                        </h4>
                                    </th>
                                    <td style="border-bottom: 1px solid green !important;padding: 10px 15px"> 
                                        <div style="margin-left: 10px" id="p_scents">
                                            <p>
                                                <input class="form-control" style="
    height: 25px;" type="text" id="new_scope_field" name="new_scope[]" placeholder="Input scope name"';
    if(!gluu_is_oxd_registered()) $html.=' disabled ';
    $html.=' />
                                            </p>
                                            <br/>
                                            <p>
                                                <input type="button" style="width: 80px" class="btn btn-primary btn-large" onclick="add_scope_for_delete()" value="Add" id="add_new_scope"/>
                                            </p>
                                        </div>
                                    </td>
                                </tr>
                            </table>
                        </div>
                        <br/>
                        <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%; font-weight: bold ">Authentication</h3>
                        <br/>
                        <p style=" margin-left: 20px; font-weight:bold "><label style="display: inline !important; "><input type="checkbox" name="send_user_check" id="send_user" value="1" ';
    if(!gluu_is_oxd_registered()) $html.=' disabled ';
    if( $gluu_send_user_check) $html.=' checked ';
    $html.= '/> <span>Bypass the local Drupal login page and send users straight to the OP for authentication</span></label>
                        </p>
                        <br/>
                        <div>
                            <table style="margin-left: 30px" class="form-table">
                                <tbody>
                                <tr>
                                    <th style="width: 230px; padding: 0px; text-transform: none !important;
     background: none !important;
     font-weight: normal;
     border:none !important;">
                                        <h4 style="text-align: left; display: inline-flex" id="scop_section">
                                            Select ACR: <a data-toggle="tooltip" class="tooltipLink" data-original-title="The OpenID Provider may make available multiple authentication mechanisms. To signal which type of authentication should be used for access to this site you can request a specific ACR. To accept the OP\'s default authentication, set this field to none.">
                                                <span class="glyphicon glyphicon-info-sign"></span>
                                            </a>
                                        </h4>
                                    </th>
                                    <td >';
    $custom_scripts = $gluu_acr;
    if(!empty($custom_scripts)){
        $html.='<div class="form-group" style="margin-bottom: 0px !important;"><select style="margin-left: 5px; " class="form-control" name="send_user_type" id="send_user_type" ';
        if(!gluu_is_oxd_registered()) $html.= ' disabled ';
        $html.='>
                                                <option value="default">none</option>';
        if($custom_scripts){
            foreach($custom_scripts as $custom_script){
                if($custom_script != "default" and $custom_script != "none"){
                    $html.='<option ';
                    if($gluu_auth_type == $custom_script) $html.='selected';
                    $html.=' value="'.$custom_script.'">'.$custom_script.'</option>';

                }
            }
        }
        $html.='</select></div>';
    }
    $html.='</td>
                                </tr>
                                <tr>
                                    <th style="text-transform: none !important;
     background: none !important;
     font-weight: normal;
     border:none !important;">
                                        <input type="submit" class="btn btn-primary btn-large" ';
    if(!gluu_is_oxd_registered()) $html.=' disabled ';
    $html.=' value="Save Authentication Settings" name="set_oxd_config" />
                                    </th>
                                    <td>
                                    </td>
                                </tr>
                                </tbody>
                            </table>
                        </div>
                    </fieldset>
            </div>';
    return $html;


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
    $bu = $base_url.'/sites/all/modules/gluu_sso/files/images';
    $authorization_redirect_uri = $base_url.'?gluuOption=oxdOpenId';
    $logout_redirect_uri = $base_url.'/index.php?option=allLogout';
    $admin_email = $_POST['gluu_sso_admin_email'];
    $oxd_port = $_POST['gluu_sso_oxd_port'];
    $op_host = $_POST['gluu_sso_op_host'];

    $config_option = array(
        "op_host" => $op_host,
        "oxd_host_ip" => '127.0.0.1',
        "reg_email" => $admin_email,
        "oxd_host_port" =>$oxd_port,
        "authorization_redirect_uri" => $authorization_redirect_uri,
        "logout_redirect_uri" => $logout_redirect_uri,
        "scope" => [ "openid", "profile", "uma_protection","uma_authorization", "email","address", "clientinfo", "mobile_phone", "phone"],
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
        $custom_scripts = array(
            array('name'=>'Google','image'=>url( $bu.'/icons/google.png'),'value'=>'gplus'),
            array('name'=>'Basic','image'=>url(  $bu.'/icons/basic.png'),'value'=>'basic'),
            array('name'=>'Duo','image'=>url(  $bu.'/icons/duo.png'),'value'=>'duo'),
            array('name'=>'OxPush2','image'=>url(  $bu.'/icons/oxpush2.png'),'value'=>'oxpush2'),
            array('name'=>'U2F token','image'=>url(  $bu.'/icons/u2f.png'),'value'=>'u2f')
        );
        variable_set('oxd_openid_custom_scripts',$custom_scripts);
    }
    $scops = variable_get('gluu_scops');
    if(empty($scops)) {
        variable_set('gluu_scops',array("openid", "profile", "uma_protection","uma_authorization", "email","address", "clientinfo", "mobile_phone", "phone"));
    }
    $register_site = new Register_site();
    $register_site->setRequestOpHost($config_option['op_host']);
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
    $bu = $base_url.'/sites/all/modules/gluu_sso/files/images';
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
    variable_set('gluu_scops',array("openid", "profile", "uma_protection","uma_authorization", "email","address", "clientinfo", "mobile_phone", "phone"));
    $custom_scripts = array(
        array('name'=>'Google','image'=>url( $bu.'/icons/google.png'),'value'=>'gplus'),
        array('name'=>'Basic','image'=>url( $bu.'/icons/basic.png'),'value'=>'basic'),
        array('name'=>'Duo','image'=>url( $bu.'/icons/duo.png'),'value'=>'duo'),
        array('name'=>'OxPush2','image'=>url(  $bu.'/icons/oxpush2.png'),'value'=>'oxpush2'),
        array('name'=>'U2F token','image'=>url( $bu.'/icons/u2f.png'),'value'=>'u2f')
    );
    variable_set('oxd_openid_custom_scripts',$custom_scripts);
    $config_option = array(
        "oxd_host_ip" => '127.0.0.1',
        "oxd_host_port" =>8099,
        "authorization_redirect_uri" => $authorization_redirect_uri,
        "logout_redirect_uri" => $logout_redirect_uri,
        "scope" => [ "openid", "profile", "uma_protection","uma_authorization", "email","address", "clientinfo", "mobile_phone", "phone"],
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

function gluu_is_oxd_registered(){
    if(variable_get('gluu_oxd_id')){
        $oxd_id = variable_get('gluu_oxd_id');
        if(!$oxd_id ) {
            return 0;
        } else {
            return $oxd_id;
        }
    }
}