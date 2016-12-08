<?php
/**
 * @file
 * Contains workflow of the miniOrange Active Directory/sso Module.
 */
$currentPath = $_SERVER['PHP_SELF'];
$pathInfo = pathinfo($currentPath);
$hostName = $_SERVER['HTTP_HOST'];
$protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";
$base_url =  $protocol.$hostName.$pathInfo['dirname']."/";
global $user; 
if (is_array($user->roles) && in_array('administrator', $user->roles)) {
    if( isset( $_REQUEST['form_key'] ) and strpos( $_REQUEST['form_key'], 'general_register_page' ) !== false ) {
        if(!isset($_SERVER['HTTPS']) or $_SERVER['HTTPS'] != "on") {
            variable_set('message_error', 'OpenID Connect requires https. This plugin will not work if your website uses http only.');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
        if($_POST['gluu_user_role']==3){
            variable_set('gluu_user_role', 3);
        }else{
            variable_set('gluu_user_role', 1);
        }
        if($_POST['gluu_users_can_register']==1){
            variable_set('gluu_users_can_register', $_POST['gluu_users_can_register']);
            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if($_POST['gluu_users_can_register']==2){
            variable_set('gluu_users_can_register', 2);

            variable_set('gluu_users_can_register', $_POST['gluu_users_can_register']);
            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if($_POST['gluu_users_can_register']==3){
            variable_set('gluu_users_can_register', 3);
            variable_set('gluu_users_can_register', $_POST['gluu_users_can_register']);
            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if (empty($_POST['gluu_oxd_port'])) {
            variable_set('message_error', 'All the fields are required. Please enter valid entries.');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
        if (intval($_POST['gluu_oxd_port']) > 65535 && intval($_POST['gluu_oxd_port']) < 0) {
            variable_set('message_error', 'Enter your oxd host port (Min. number 1, Max. number 65535)');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
        if  (!empty($_POST['gluu_provider'])) {
            if (filter_var($_POST['gluu_provider'], FILTER_VALIDATE_URL) === false) {
                variable_set('message_error', 'Please enter valid OpenID Provider URI.');
                header("Location: ".$base_url."admin/config/people/gluu_sso");return;
            }
        }
        if  (!empty($_POST['gluu_custom_logout'])) {
            if (filter_var($_POST['gluu_custom_logout'], FILTER_VALIDATE_URL) === false) {
                variable_set('message_error', 'Please enter valid Custom URI.');
            }else{
                variable_set('gluu_custom_logout', $_POST['gluu_custom_logout']);
            }
        }
        else{
            variable_set('gluu_custom_logout', '');
        }

        if (isset($_POST['gluu_provider']) and !empty($_POST['gluu_provider'])) {

            $gluu_provider = $_POST['gluu_provider'];
            variable_set('gluu_provider', $gluu_provider);
            $arrContextOptions=array(
                "ssl"=>array(
                    "verify_peer"=>false,
                    "verify_peer_name"=>false,
                ),
            );
            $json = file_get_contents($gluu_provider.'/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));

            $obj = json_decode($json);
            if(!empty($obj->userinfo_endpoint)){

                if(empty($obj->registration_endpoint)){
                    variable_set('message_success', "Please enter your client_id and client_secret.");
                    $gluu_config = array(
                        "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                        "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                        "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                        "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                        "config_scopes" => ["openid","profile","email"],
                        "gluu_client_id" => "",
                        "gluu_client_secret" => "",
                        "config_acr" => []
                    );
                    if($_POST['gluu_users_can_register']==2){
                        $config = variable_get('gluu_config');
                        array_push($config['config_scopes'],'permission');
                        variable_set('gluu_config', $config);
                    }
                    variable_set('gluu_config', $gluu_config);
                    if(isset($_POST['gluu_client_id']) and !empty($_POST['gluu_client_id']) and
                        isset($_POST['gluu_client_secret']) and !empty($_POST['gluu_client_secret'])){
                        $gluu_config = array(
                            "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                            "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                            "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                            "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                            "config_scopes" => ["openid","profile","email"],
                            "gluu_client_id" => $_POST['gluu_client_id'],
                            "gluu_client_secret" => $_POST['gluu_client_secret'],
                            "config_acr" => []
                        );
                        variable_set('gluu_config', $gluu_config);
                        if($_POST['gluu_users_can_register']==2){
                            $config = variable_get('gluu_config');
                            array_push($config['config_scopes'],'permission');
                            variable_set('gluu_config',$config);
                        }
                        if(!gluu_is_port_working()){
                            variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        }
                        $register_site = new Register_site();
                        $register_site->setRequestOpHost($gluu_provider);
                        $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                        $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                        $register_site->setRequestContacts([$gluu_config['admin_email']]);
                        $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
                        $get_scopes = $obj->scopes_supported;
                        if(!empty($obj->acr_values_supported)){
                            $get_acr = $obj->acr_values_supported;
                            variable_set('gluu_acr', $get_acr);
                            $register_site->setRequestAcrValues($gluu_config['config_acr']);
                        }
                        else{
                            $register_site->setRequestAcrValues($gluu_config['config_acr']);
                        }
                        if(!empty($obj->scopes_supported)){
                            $get_scopes = $obj->scopes_supported;
                            variable_set('gluu_scopes', $get_scopes);
                            $register_site->setRequestScope($obj->scopes_supported);
                        }
                        else{
                            $register_site->setRequestScope($gluu_config['config_scopes']);
                        }
                        $register_site->setRequestClientId($gluu_config['gluu_client_id']);
                        $register_site->setRequestClientSecret($gluu_config['gluu_client_secret']);
                        $status = $register_site->request();
                        if ($status['message'] == 'invalid_op_host') {
                            variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        }
                        if (!$status['status']) {
                            variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        }
                        if ($status['message'] == 'internal_error') {
                            variable_set('message_error', 'ERROR: '.$status['error_message']);
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        }
                        $gluu_oxd_id = $register_site->getResponseOxdId();
                        //var_dump($register_site->getResponseObject());exit;
                        if ($gluu_oxd_id) {
                            variable_set('gluu_oxd_id', $gluu_oxd_id);
                            $gluu_provider = $register_site->getResponseOpHost();
                            variable_set('gluu_provider', $gluu_provider);
                            variable_set('message_success', 'Your settings are saved successfully.');
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        } else {
                            variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                        }
                    }
                    else{
                        variable_set('openid_error', 'Error505.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                }
                else{

                    $gluu_config = array(
                        "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                        "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                        "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                        "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                        "config_scopes" => ["openid","profile","email"],
                        "gluu_client_id" => "",
                        "gluu_client_secret" => "",
                        "config_acr" => []
                    );
                    variable_set('gluu_config', $gluu_config);
                    if($_POST['gluu_users_can_register']==2){
                        $config = variable_get('gluu_config');
                        array_push($config['config_scopes'],'permission');
                        variable_set('gluu_config', $config);
                    }

                    if(!gluu_is_port_working()){
                        variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }

                    $register_site = new Register_site();
                    $register_site->setRequestOpHost($gluu_provider);
                    $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                    $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                    $register_site->setRequestContacts([$gluu_config['admin_email']]);
                    $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
                    $get_scopes = $obj->scopes_supported;
                    if(!empty($obj->acr_values_supported)){
                        $get_acr = $obj->acr_values_supported;
                        variable_set('gluu_acr', $get_acr);
                        $register_site->setRequestAcrValues($gluu_config['config_acr']);
                    }
                    else{
                        $register_site->setRequestAcrValues($gluu_config['config_acr']);
                    }
                    if(!empty($obj->scopes_supported)){
                        $get_scopes = $obj->scopes_supported;
                        variable_set('gluu_scopes', $get_scopes);
                        $register_site->setRequestScope($obj->scopes_supported);
                    }
                    else{
                        $register_site->setRequestScope($gluu_config['config_scopes']);
                    }
                    $status = $register_site->request();
                    //var_dump($status);exit;
                    if ($status['message'] == 'invalid_op_host') {
                        variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    if (!$status['status']) {
                        variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    if ($status['message'] == 'internal_error') {
                        variable_set('message_error', 'ERROR: '.$status['error_message']);
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    $gluu_oxd_id = $register_site->getResponseOxdId();
                    if ($gluu_oxd_id) {
                        variable_set('gluu_oxd_id', $gluu_oxd_id);
                        $register_site->getResponseOpHost();
                        variable_set('gluu_provider', $gluu_provider);
                        variable_set('message_success', 'Your settings are saved successfully.');

                        header("Location: ".$base_url."admin/config/people/gluu_sso");
                        return;
                    }
                    else {
                        variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                }
            }
            else{
                variable_set('message_error', 'Please enter correct URI of the OpenID Provider');
                header("Location: ".$base_url."admin/config/people/gluu_sso");return;
            }
        }
        else{
            $gluu_config = array(
                "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                "config_scopes" => ["openid","profile","email"],
                "gluu_client_id" => "",
                "gluu_client_secret" => "",
                "config_acr" => []
            );
            variable_set('gluu_config', $gluu_config);
            if($_POST['gluu_users_can_register']==2){
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }
            if(!gluu_is_port_working()){
                variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');

                header("Location: ".$base_url."admin/config/people/gluu_sso");return;
            }
            $register_site = new Register_site();
            $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
            $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
            $register_site->setRequestContacts([$gluu_config['admin_email']]);
            $register_site->setRequestAcrValues($gluu_config['config_acr']);
            $register_site->setRequestScope($gluu_config['config_scopes']);
            $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
            $status = $register_site->request();

            if ($status['message'] == 'invalid_op_host') {
                variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                header("Location: ".$base_url."admin/config/people/gluu_sso");
                return;
            }
            if (!$status['status']) {
                variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                header("Location: ".$base_url."admin/config/people/gluu_sso");
                return;
            }
            if ($status['message'] == 'internal_error') {
                variable_set('message_error', 'ERROR: '.$status['error_message']);
                header("Location: ".$base_url."admin/config/people/gluu_sso");
                return;
            }
            $gluu_oxd_id = $register_site->getResponseOxdId();
            if ($gluu_oxd_id) {
                variable_set('gluu_oxd_id', $gluu_oxd_id);
                $gluu_provider = $register_site->getResponseOpHost();
                variable_set('gluu_provider', $gluu_provider);
                $arrContextOptions=array(
                    "ssl"=>array(
                        "verify_peer"=>false,
                        "verify_peer_name"=>false,
                    ),
                );
                $json = file_get_contents($gluu_provider.'/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));
                $obj = json_decode($json);
                if(!gluu_is_port_working()){
                    variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                $register_site = new Register_site();
                $register_site->setRequestOpHost($gluu_provider);
                $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                $register_site->setRequestContacts([$gluu_config['admin_email']]);
                $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);

                $get_scopes = $obj->scopes_supported;
                if(!empty($obj->acr_values_supported)){
                    $get_acr = $obj->acr_values_supported;
                    variable_set('gluu_acr', $get_acr);
                    $register_site->setRequestAcrValues($gluu_config['config_acr']);
                }
                else{
                    $register_site->setRequestAcrValues($gluu_config['config_acr']);
                }
                if(!empty($obj->scopes_supported)){
                    $get_scopes = $obj->scopes_supported;
                    variable_set('gluu_scopes', $get_scopes);
                    $register_site->setRequestScope($obj->scopes_supported);
                }
                else{
                    $register_site->setRequestScope($gluu_config['config_scopes']);
                }
                $status = $register_site->request();
                if ($status['message'] == 'invalid_op_host') {
                    variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                if (!$status['status']) {
                    variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                if ($status['message'] == 'internal_error') {
                    variable_set('message_error', 'ERROR: '.$status['error_message']);
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                $gluu_oxd_id = $register_site->getResponseOxdId();
                if ($gluu_oxd_id) {
                    variable_set('gluu_oxd_id', $gluu_oxd_id);
                    variable_set('message_success', 'Your settings are saved successfully.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                else {
                    variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
            }
            else {
                variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                header("Location: ".$base_url."admin/config/people/gluu_sso");return;
            }
        }
    }
    else if( isset( $_REQUEST['form_key'] ) and strpos( $_REQUEST['form_key'], 'general_oxd_edit' ) !== false ) {
        if(!isset($_SERVER['HTTPS']) or $_SERVER['HTTPS'] != "on") {
            variable_set('message_error', 'OpenID Connect requires https. This plugin will not work if your website uses http only.');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }

        if($_POST['gluu_user_role']==3){
            variable_set('gluu_user_role', 3);
        }else{
            variable_set('gluu_user_role', 1);
        }
        if($_POST['gluu_users_can_register']==1){
            variable_set('gluu_users_can_register', $_POST['gluu_users_can_register']);
            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if($_POST['gluu_users_can_register']==2){
            variable_set('gluu_users_can_register', 2);

            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if($_POST['gluu_users_can_register']==3){
            variable_set('gluu_users_can_register', 3);
            if(!empty(array_values(array_filter($_POST['gluu_new_role'])))){
                variable_set('gluu_new_role', array_values(array_filter($_POST['gluu_new_role'])));
                $config = variable_get('gluu_config');
                array_push($config['config_scopes'],'permission');
                variable_set('gluu_config', $config);
            }else{
                variable_set('gluu_new_role', array());
            }
        }
        if (empty($_POST['gluu_oxd_port'])) {
            variable_set('message_error', 'All the fields are required. Please enter valid entries.');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
        if (intval($_POST['gluu_oxd_port']) > 65535 && intval($_POST['gluu_oxd_port']) < 0) {
            variable_set('message_error', 'Enter your oxd host port (Min. number 1, Max. number 65535)');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }

        if  (!empty($_POST['gluu_custom_logout'])) {
            if (filter_var($_POST['gluu_custom_logout'], FILTER_VALIDATE_URL) === false) {
                variable_set('message_error', 'Please enter valid Custom URI.');
            }else{
                variable_set('gluu_custom_logout', $_POST['gluu_custom_logout']);
            }
        }
        else{
            variable_set('gluu_custom_logout', '');
        }
        $gluu_provider = variable_get('gluu_provider');
        $arrContextOptions=array(
            "ssl"=>array(
                "verify_peer"=>false,
                "verify_peer_name"=>false,
            ),
        );
        $json = file_get_contents($gluu_provider.'/.well-known/openid-configuration', false, stream_context_create($arrContextOptions));

        $obj = json_decode($json);
        if(!empty($obj->userinfo_endpoint)){

            if(empty($obj->registration_endpoint)){
                variable_set('message_success', "Please enter your client_id and client_secret.");
                $gluu_config = array(
                    "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                    "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                    "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                    "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                    "config_scopes" => ["openid","profile","email"],
                    "gluu_client_id" => "",
                    "gluu_client_secret" => "",
                    "config_acr" => []
                );
                if($_POST['gluu_users_can_register']==2){
                    $config = variable_get('gluu_config');
                    array_push($config['config_scopes'],'permission');
                    variable_set('gluu_config', $config);
                }
                variable_set('gluu_config', $gluu_config);
                if(isset($_POST['gluu_client_id']) and !empty($_POST['gluu_client_id']) and
                    isset($_POST['gluu_client_secret']) and !empty($_POST['gluu_client_secret'])){
                    $gluu_config = array(
                        "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                        "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                        "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                        "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                        "config_scopes" => ["openid","profile","email"],
                        "gluu_client_id" => $_POST['gluu_client_id'],
                        "gluu_client_secret" => $_POST['gluu_client_secret'],
                        "config_acr" => []
                    );
                    variable_set('gluu_config', $gluu_config);
                    if($_POST['gluu_users_can_register']==2){
                        $config = variable_get('gluu_config');
                        array_push($config['config_scopes'],'permission');
                        variable_set('gluu_config', $config);
                    }
                    if(!gluu_is_port_working()){
                        variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    $register_site = new Register_site();
                    $register_site->setRequestOpHost($gluu_provider);
                    $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                    $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                    $register_site->setRequestContacts([$gluu_config['admin_email']]);
                    $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
                    $get_scopes = $obj->scopes_supported;
                    if(!empty($obj->acr_values_supported)){
                        $get_acr = $obj->acr_values_supported;
                        variable_set('gluu_acr', $get_acr);
                        $register_site->setRequestAcrValues($gluu_config['config_acr']);
                    }
                    else{
                        $register_site->setRequestAcrValues($gluu_config['config_acr']);
                    }
                    if(!empty($obj->scopes_supported)){
                        $get_scopes = $obj->scopes_supported;
                        variable_set('gluu_scopes', $get_scopes);
                        $register_site->setRequestScope($obj->scopes_supported);
                    }
                    else{
                        $register_site->setRequestScope($gluu_config['config_scopes']);
                    }
                    $register_site->setRequestClientId($gluu_config['gluu_client_id']);
                    $register_site->setRequestClientSecret($gluu_config['gluu_client_secret']);
                    $status = $register_site->request();
                    if ($status['message'] == 'invalid_op_host') {
                        variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    if (!$status['status']) {
                        variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    if ($status['message'] == 'internal_error') {
                        variable_set('message_error', 'ERROR: '.$status['error_message']);
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                    $gluu_oxd_id = $register_site->getResponseOxdId();
                    //var_dump($register_site->getResponseObject());exit;
                    if ($gluu_oxd_id) {
                        variable_set('gluu_oxd_id', $gluu_oxd_id);
                        $gluu_provider = $register_site->getResponseOpHost();
                        variable_set('gluu_provider', $gluu_provider);
                        variable_set('message_success', 'Your settings are saved successfully.');
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    } else {
                        variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                        header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                    }
                }
                else{
                    variable_set('openid_error', 'Error505.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
            }
            else{

                $gluu_config = array(
                    "gluu_oxd_port" =>$_POST['gluu_oxd_port'],
                    "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                    "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                    "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                    "config_scopes" => ["openid","profile","email"],
                    "gluu_client_id" => "",
                    "gluu_client_secret" => "",
                    "config_acr" => []
                );
                variable_set('gluu_config', $gluu_config);
                if($_POST['gluu_users_can_register']==2){
                    $config = variable_get('gluu_config');
                    array_push($config['config_scopes'],'permission');
                    variable_set('gluu_config', $config);
                }

                if(!gluu_is_port_working()){
                    variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }

                $register_site = new Register_site();
                $register_site->setRequestOpHost($gluu_provider);
                $register_site->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
                $register_site->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
                $register_site->setRequestContacts([$gluu_config['admin_email']]);
                $register_site->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
                $get_scopes = $obj->scopes_supported;
                if(!empty($obj->acr_values_supported)){
                    $get_acr = $obj->acr_values_supported;
                    variable_set('gluu_acr', $get_acr);
                    $register_site->setRequestAcrValues($gluu_config['config_acr']);
                }
                else{
                    $register_site->setRequestAcrValues($gluu_config['config_acr']);
                }
                if(!empty($obj->scopes_supported)){
                    $get_scopes = $obj->scopes_supported;
                    variable_set('gluu_scopes', $get_scopes);
                    $register_site->setRequestScope($obj->scopes_supported);
                }
                else{
                    $register_site->setRequestScope($gluu_config['config_scopes']);
                }
                $status = $register_site->request();
                //var_dump($status);exit;
                if ($status['message'] == 'invalid_op_host') {
                    variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                if (!$status['status']) {
                    variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                if ($status['message'] == 'internal_error') {
                    variable_set('message_error', 'ERROR: '.$status['error_message']);
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
                $gluu_oxd_id = $register_site->getResponseOxdId();
                if ($gluu_oxd_id) {
                    variable_set('gluu_oxd_id', $gluu_oxd_id);
                    $register_site->getResponseOpHost();
                    variable_set('gluu_provider', $gluu_provider);
                    variable_set('message_success', 'Your settings are saved successfully.');

                    header("Location: ".$base_url."admin/config/people/gluu_sso");
                    return;
                }
                else {
                    variable_set('message_error', "ERROR: OpenID Provider host is required if you don\'t provide it in oxd-default-site-config.json");
                    header("Location: ".$base_url."admin/config/people/gluu_sso");return;
                }
            }
        }
        else{
            variable_set('message_error', 'Please enter correct URI of the OpenID Provider');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
    }
    else if( isset( $_REQUEST['submit'] ) and strpos( $_REQUEST['submit'], 'delete' )  !== false and !empty($_REQUEST['submit'])) {
        if (variable_get('gluu_scopes')) {
            variable_del('gluu_scopes');
        }
        if (variable_get('gluu_acr')) {
            variable_del('gluu_acr');
        }
        if (variable_get('gluu_config')) {
            variable_del('gluu_config');
        }
        if (variable_get('gluu_auth_type')) {
            variable_del('gluu_auth_type');
        }
        if (variable_get('gluu_custom_logout')) {
            variable_del('gluu_custom_logout');
        }
        if (variable_get('gluu_provider')) {
            variable_del('gluu_provider');
        }
        if (variable_get('gluu_send_user_check')) {
            variable_del('gluu_send_user_check');
        }
        if (variable_get('gluu_oxd_id')) {
            variable_del('gluu_oxd_id');
        }
        if (variable_get('gluu_user_role')) {
            variable_del('gluu_user_role');
        }
        if (variable_get('gluu_users_can_register')) {
            variable_del('gluu_users_can_register');
        }
        if (variable_get('gluu_new_role')) {
            variable_del('gluu_new_role');
        }
        if (variable_get('openid_error')) {
            variable_del('openid_error');
        }


        if (!variable_get('gluu_scopes')) {
            $get_scopes = array("openid", "profile", "email");
            variable_set('gluu_scopes', $get_scopes);
        }
        if (!variable_get('gluu_acr')) {
            $custom_scripts = array('none');
            variable_set('gluu_acr', $custom_scripts);
        }
        if (!variable_get('gluu_config')) {
            $gluu_config = array(
                "gluu_oxd_port" => 8099,
                "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                "config_scopes" => ["openid","profile","email"],
                "gluu_client_id" => "",
                "gluu_client_secret" => "",
                "config_acr" => []
            );
            variable_set('gluu_config', $gluu_config);
        }
        if (!variable_get('gluu_auth_type')) {
            $gluu_auth_type = 'default';
            variable_set('gluu_auth_type', $gluu_auth_type);
        }
        if (!variable_get('gluu_custom_logout')) {
            $gluu_custom_logout = '';
            variable_set('gluu_custom_logout', $gluu_custom_logout);
        }
        if (!variable_get('gluu_provider')) {
            $gluu_provider = '';
            variable_set('gluu_provider', $gluu_provider);
        }
        if (!variable_get('gluu_send_user_check')) {
            $gluu_send_user_check = 0;
            variable_set('gluu_send_user_check', $gluu_send_user_check);
        }
        if (!variable_get('gluu_oxd_id')) {
            $gluu_oxd_id = '';
            variable_set('gluu_oxd_id', $gluu_oxd_id);
        }
        if (!variable_get('gluu_user_role')) {
            $gluu_user_role = 1;
            variable_set('gluu_user_role', $gluu_user_role);
        }
        if (!variable_get('gluu_users_can_register')) {
            $gluu_users_can_register = 1;
            variable_set('gluu_users_can_register', $gluu_users_can_register);
        }
        if (!variable_get('gluu_new_role')) {
            variable_set('gluu_new_role', array());
        }
        if (!variable_get('openid_error')) {
            $openid_error = '';
            variable_set('openid_error', $openid_error);
        }
        variable_set('message_success', 'Configurations deleted Successfully.');;
        header("Location: ".$base_url."admin/config/people/gluu_sso");
        return;
    }
    else if( isset( $_REQUEST['form_key'] ) and strpos( $_REQUEST['form_key'], 'general_oxd_id_reset' )!== false and !empty($_REQUEST['resetButton'])) {
        if (variable_get('gluu_scopes')) {
            variable_del('gluu_scopes');
        }
        if (variable_get('gluu_acr')) {
            variable_del('gluu_acr');
        }
        if (variable_get('gluu_config')) {
            variable_del('gluu_config');
        }
        if (variable_get('gluu_auth_type')) {
            variable_del('gluu_auth_type');
        }
        if (variable_get('gluu_custom_logout')) {
            variable_del('gluu_custom_logout');
        }
        if (variable_get('gluu_provider')) {
            variable_del('gluu_provider');
        }
        if (variable_get('gluu_send_user_check')) {
            variable_del('gluu_send_user_check');
        }
        if (variable_get('gluu_oxd_id')) {
            variable_del('gluu_oxd_id');
        }
        if (variable_get('gluu_user_role')) {
            variable_del('gluu_user_role');
        }
        if (variable_get('gluu_users_can_register')) {
            variable_del('gluu_users_can_register');
        }
        if (variable_get('gluu_new_role')) {
            variable_del('gluu_new_role');
        }
        if (variable_get('openid_error')) {
            variable_del('openid_error');
        }

        if (!variable_get('gluu_scopes')) {
            $get_scopes = array("openid", "profile", "email");
            variable_set('gluu_scopes', $get_scopes);
        }
        if (!variable_get('gluu_acr')) {
            $custom_scripts = array('none');
            variable_set('gluu_acr', $custom_scripts);
        }
        if (!variable_get('gluu_config')) {
            $gluu_config = array(
                "gluu_oxd_port" => 8099,
                "admin_email" => variable_get('site_mail', ini_get('sendmail_from')),
                "authorization_redirect_uri" => $base_url.'?gluuOption=oxdOpenId',
                "post_logout_redirect_uri" => $base_url.'index.php?option=allLogout',
                "config_scopes" => ["openid","profile","email"],
                "gluu_client_id" => "",
                "gluu_client_secret" => "",
                "config_acr" => []
            );
            variable_set('gluu_config', $gluu_config);
        }
        if (!variable_get('gluu_auth_type')) {
            $gluu_auth_type = 'default';
            variable_set('gluu_auth_type', $gluu_auth_type);
        }
        if (!variable_get('gluu_custom_logout')) {
            $gluu_custom_logout = '';
            variable_set('gluu_custom_logout', $gluu_custom_logout);
        }
        if (!variable_get('gluu_provider')) {
            $gluu_provider = '';
            variable_set('gluu_provider', $gluu_provider);
        }
        if (!variable_get('gluu_send_user_check')) {
            $gluu_send_user_check = 0;
            variable_set('gluu_send_user_check', $gluu_send_user_check);
        }
        if (!variable_get('gluu_oxd_id')) {
            $gluu_oxd_id = '';
            variable_set('gluu_oxd_id', $gluu_oxd_id);
        }
        if (!variable_get('gluu_user_role')) {
            $gluu_user_role = 1;
            variable_set('gluu_user_role', $gluu_user_role);
        }
        if (!variable_get('gluu_users_can_register')) {
            $gluu_users_can_register = 1;
            variable_set('gluu_users_can_register', $gluu_users_can_register);
        }
        if (!variable_get('gluu_new_role')) {
            variable_set('gluu_new_role', array());
        }
        if (!variable_get('openid_error')) {
            $openid_error = '';
            variable_set('openid_error', $openid_error);
        }
        drupal_set_message(t('Configurations deleted Successfully.'));
        header("Location: ".$base_url."admin/config/people/gluu_sso");
        return;
    }
    else if( isset( $_REQUEST['form_key'] ) and strpos( $_REQUEST['form_key'], 'openid_config_page' ) !== false ) {
        $params = $_REQUEST;
        $message_success = '';
        $message_error = '';

        if($_POST['send_user_type']){
            $gluu_auth_type = $_POST['send_user_type'];
            variable_set('gluu_auth_type', $gluu_auth_type);
        }else{
            variable_set('gluu_auth_type', 'default');
        }
        $gluu_send_user_check = $_POST['send_user_check'];
        variable_set('gluu_send_user_check', $gluu_send_user_check);

        if(!empty($params['scope']) && isset($params['scope'])){
            $gluu_config =   variable_get("gluu_config");
            $gluu_config['config_scopes'] = $params['scope'];
            variable_set('gluu_config', $gluu_config);
        }
        if(!empty($params['scope_name']) && isset($params['scope_name'])){
            $get_scopes =   variable_get("gluu_scopes");
            foreach($params['scope_name'] as $scope){
                if($scope && !in_array($scope,$get_scopes)){
                    array_push($get_scopes, $scope);
                }
            }
            variable_set('gluu_scopes', $get_scopes);
        }
        $gluu_acr              = variable_get('gluu_acr');

        if(!empty($params['acr_name']) && isset($params['acr_name'])){
            $get_acr =   variable_get("gluu_acr");
            foreach($params['acr_name'] as $scope){
                if($scope && !in_array($scope,$get_acr)){
                    array_push($get_acr, $scope);
                }
            }
            variable_set('gluu_acr', $get_acr);
        }
        $gluu_config =   variable_get("gluu_config");
        $gluu_oxd_id =   variable_get("gluu_oxd_id");
        if(!gluu_is_port_working()){
            variable_set('message_error', 'Can not connect to the oxd server. Please check the oxd-config.json file to make sure you have entered the correct port and the oxd server is operational.');
            header("Location: ".$base_url."admin/config/people/gluu_sso");return;
        }
        $update_site_registration = new Update_site_registration();
        $update_site_registration->setRequestOxdId($gluu_oxd_id);
        $update_site_registration->setRequestAcrValues($gluu_config['acr_values']);
        $update_site_registration->setRequestAuthorizationRedirectUri($gluu_config['authorization_redirect_uri']);
        $update_site_registration->setRequestLogoutRedirectUri($gluu_config['post_logout_redirect_uri']);
        $update_site_registration->setRequestContacts([$gluu_config['admin_email']]);
        $update_site_registration->setRequestClientLogoutUri($gluu_config['post_logout_redirect_uri']);
        $update_site_registration->setRequestScope($gluu_config['config_scopes']);
        $status = $update_site_registration->request();
        $new_oxd_id = $update_site_registration->getResponseOxdId();
        if($new_oxd_id){
            variable_set('gluu_oxd_id', $new_oxd_id);
        }

        variable_set('message_success', 'Your OpenID connect configuration has been saved.');
        header("Location: ".$base_url."admin/config/people/gluu_sso/openidconnect");return;
    }
    else if( isset( $_REQUEST['form_key_scope_delete'] ) and strpos( $_REQUEST['form_key_scope_delete'], 'form_key_scope_delete' ) !== false ) {
        $get_scopes =   variable_get('gluu_scopes');
        $up_cust_sc =  array();
        foreach($get_scopes as $custom_scop){
            if($custom_scop !=$_POST['delete_scope']){
                array_push($up_cust_sc,$custom_scop);
            }
        }
        $get_scopes = $up_cust_sc;
        variable_set('gluu_scopes', $get_scopes);


        $gluu_config =   variable_get("gluu_config");
        $up_cust_scope =  array();
        foreach($gluu_config['config_scopes'] as $custom_scop){
            if($custom_scop !=$_POST['delete_scope']){
                array_push($up_cust_scope,$custom_scop);
            }
        }
        $gluu_config['config_scopes'] = $up_cust_scope;
        variable_set('gluu_config', $gluu_config);
        return true;
    }
    else if (isset($_REQUEST['form_key_scope']) and strpos( $_REQUEST['form_key_scope'], 'oxd_openid_config_new_scope' ) !== false) {
        if (gluu_is_oxd_registered()) {
            if (!empty($_REQUEST['new_value_scope']) && isset($_REQUEST['new_value_scope'])) {

                $get_scopes =   variable_get("gluu_scopes");
                if($_REQUEST['new_value_scope'] && !in_array($_REQUEST['new_value_scope'],$get_scopes)){
                    array_push($get_scopes, $_REQUEST['new_value_scope']);
                }
                variable_set('gluu_scopes', $get_scopes);
                return true;
            }

        }
    }

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
function gluu_is_port_working(){
    $config_option = variable_get('gluu_config');
    $connection = @fsockopen('127.0.0.1', $config_option['gluu_oxd_port']);
    if (is_resource($connection))
    {
        fclose($connection);
        return true;
    }
    else
    {
        return false;
    }
}
function oxd_openid_check_empty_or_null( $value ) {
    if( ! isset( $value ) || empty( $value ) ) {
        return true;
    }
    return false;
}
