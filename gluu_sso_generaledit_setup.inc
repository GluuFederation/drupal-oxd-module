<?php
	/**
	 * @file
	 * Contains form for customer setup.
	 */
	
	/**
	 * Loading js and css .
	 */
	function gluu_sso_add_js_css_fonts_edit()
	{
		drupal_add_js('https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js', 'external');
		drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/gluu-oxd-css.css');
		drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/gluu-oxd-css1.css');
		drupal_add_css(drupal_get_path('module', 'gluu_sso') . '/files/css/bootstrap.css');
		drupal_add_js('https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js', 'external');
	}
	
	gluu_sso_add_js_css_fonts_edit();
	
	/**
	 * Menu generalEdit page callback;
	 */
	function gluu_sso_generaledit_setup($form, &$form_state)
	{
		$form['#action'] = url('admin/config/people/gluu_sso/savedata');
		$form['markup_2'] = array(
			'#markup' => gluu_sso_admin_edit_html()
		);
		
		return $form;
	}
	
	/**
	 * Getting base url;
	 */
	function gluu_sso_get_base_url_edit()
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
	
	/**
	 * Html for generalEdit page;
	 */
	function gluu_sso_admin_edit_html()
	{
		$base_url = gluu_sso_get_base_url_edit();
		$img_path = $base_url . '/sites/all/modules/gluu_sso';
		$gluu_config = variable_get('gluu_config');
		$gluu_provider = variable_get('gluu_provider');
		$gluu_user_role = variable_get('gluu_user_role');
		$gluu_custom_logout = variable_get('gluu_custom_logout');
		$gluu_new_roles = variable_get('gluu_new_role');
		$gluu_users_can_register = variable_get('gluu_users_can_register');
		$message_error = variable_get('message_error');
		$message_success = variable_get('message_success');
		$openid_error = variable_get('openid_error');
		$html = '
<script type="application/javascript">
    jQuery(document).ready(function() {

        jQuery(\'[data-toggle="tooltip"]\').tooltip();
        jQuery(\'#p_role\').on(\'click\', \'a.remrole\', function() {
            jQuery(this).parents(\'.role_p\').remove();
        });

    });';
		$html .= '
  function test_add() {
        var wrapper1 = \'<p class="role_p" style="padding-top: 10px">\' +
            \'<input class="form-control"  type="text" name="gluu_new_role[]" placeholder="Input role name" style="display: inline; width: 200px !important; margin-right: 5px"/>\' +
            \'<button type="button" class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button> \' +
            \'<button type="button" class="btn btn-xs remrole" ><span class="glyphicon glyphicon-minus"></span></button>\' +
            \'</p>\';
        jQuery(wrapper1).find(\'.remrole\').on(\'click\', function() {
            jQuery(this).parent(\'.role_p\').remove();
        });
        jQuery(wrapper1).appendTo(\'#p_role\');
    }
    jQuery(document ).ready(function() {
        jQuery(document).ready(function() {

            jQuery(\'[data-toggle="tooltip"]\').tooltip();
            jQuery(\'#p_role\').on(\'click\', \'a.remrole\', function() {
                jQuery(this).parents(\'.role_p\').remove();
            });

        });';
		
		if ($gluu_users_can_register == 2) {
			$html .= 'jQuery("#p_role").children().prop(\'disabled\',false);
        jQuery("#p_role *").prop(\'disabled\',false);';
		} else if ($gluu_users_can_register == 3) {
			$html .= 'jQuery("#p_role").children().prop(\'disabled\',true);
        jQuery("#p_role *").prop(\'disabled\',true);
        jQuery("input[name=\'gluu_new_role[]\']").each(function(){
            var striped = jQuery(\'#p_role\');
            var value =  jQuery(this).attr("value");
            jQuery(\'<p><input type="hidden" name="gluu_new_role[]"  value= "\'+value+\'"/></p>\').appendTo(striped);
        });
        jQuery("#UserType").prop(\'disabled\',true);';
			
		} else {
			$html .= 'jQuery("#p_role").children().prop(\'disabled\',true);
        jQuery("#p_role *").prop(\'disabled\',true);
        jQuery("input[name=\'gluu_new_role[]\']").each(function(){
            var striped = jQuery(\'#p_role\');
            var value =  jQuery(this).attr("value");
            jQuery(\'<p><input type="hidden" name="gluu_new_role[]"  value= "\'+value+\'"/></p>\').appendTo(striped);
        });';
		}
		$html .= 'jQuery(\'input:radio[name="gluu_users_can_register"]\').change(function(){
            if(jQuery(this).is(\':checked\') && jQuery(this).val() == \'2\'){
                jQuery("#p_role").children().prop(\'disabled\',false);
                jQuery("#p_role *").prop(\'disabled\',false);
                jQuery("input[type=\'hidden\'][name=\'gluu_new_role[]\']").remove();
                jQuery("#UserType").prop(\'disabled\',false);
            }else if(jQuery(this).is(\':checked\') && jQuery(this).val() == \'3\'){
                jQuery("#p_role").children().prop(\'disabled\',true);
                jQuery("#p_role *").prop(\'disabled\',true);
                jQuery("input[type=\'hidden\'][name=\'gluu_new_role[]\']").remove();
                jQuery("input[name=\'gluu_new_role[]\']").each(function(){
                    var striped = jQuery(\'#p_role\');
                    var value =  jQuery(this).attr("value");
                    jQuery(\'<p><input type="hidden" name="gluu_new_role[]"  value= "\'+value+\'"/></p>\').appendTo(striped);
                });
                jQuery("#UserType").prop(\'disabled\',true);
            }else{
                jQuery("#p_role").children().prop(\'disabled\',true);
                jQuery("#p_role *").prop(\'disabled\',true);
                jQuery("input[type=\'hidden\'][name=\'gluu_new_role[]\']").remove();
                jQuery("input[name=\'gluu_new_role[]\']").each(function(){
                    var striped = jQuery(\'#p_role\');
                    var value =  jQuery(this).attr("value");
                    jQuery(\'<p><input type="hidden" name="gluu_new_role[]"  value= "\'+value+\'"/></p>\').appendTo(striped);
                });
                jQuery("#UserType").prop(\'disabled\',false);
            }
        });
        jQuery("input[name=\'scope[]\']").change(function(){
            var form=$("#scpe_update");
            if (jQuery(this).is(\':checked\')) {
                jQuery.ajax({
                    url: window.location,
                    type: \'POST\',
                    data:form.serialize(),
                    success: function(result){
                        if(result){
                            return false;
                        }
                    }});
            }else{
                jQuery.ajax({
                    url: window.location,
                    type: \'POST\',
                    data:form.serialize(),
                    success: function(result){
                        if(result){
                            return false;
                        }
                    }});
            }
        });
        jQuery(\'#p_role\').on(\'click\', \'.remrole\', function() {
            jQuery(this).parents(\'.role_p\').remove();
        });
    });
</script>';
		
		$html .= '
<script type="application/javascript">
    var formSubmitting = false;
    var setFormSubmitting = function() { formSubmitting = true; };
    var edit_cancel_function = function() { formSubmitting = true; };
    window.onload = function() {
        window.addEventListener("beforeunload", function (e) {
            if (formSubmitting ) {
                return undefined;
            }

            var confirmationMessage = "You may have unsaved changes. Are you sure you want to leave this page?";

            (e || window.event).returnValue = confirmationMessage; //Gecko + IE
            return confirmationMessage; //Gecko + Webkit, Safari, Chrome etc.
        });
    };
</script>
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
            <li class="active" id="account_setup"><a  style="cursor: pointer;" href="' . $base_url . 'admin/config/people/gluu_sso/general">General</a></li>';
		if (!gluu_sso_is_oxd_registered_edit()) {
			$html .= '<li id="social-sharing-setup"><a style="pointer-events: none; cursor: default;" >OpenID Connect Configuration</a></li>';
		} else {
			$html .= '<li id="social-sharing-setup"><a  style="cursor: pointer;" href="' . $base_url . 'admin/config/people/gluu_sso/openidconnect">OpenID Connect Configuration</a></li>';
		}
		$html .= '<li id=""><a href="https://oxd.gluu.org/docs/plugin/drupal/" target="_blank">Documentation</a></li>';
		$html .= '</ul>
        <div class="container-page">';
		if (!gluu_sso_is_oxd_registered_edit()) {
			
			$html .= '<div class="page" id="accountsetup">
                    <div class="mo2f_table_layout">
                            <input type="hidden" name="form_key" value="general_register_page"/>
                            <div class="login_GluuOxd">
                                <br/>
                                <div  style="padding-left: 20px;">Register your site with any standard OpenID Provider (OP). If you need an OpenID Provider you can deploy the <a target="_blank" href="https://gluu.org/docs/deployment/"> free open source Gluu Server.</a></div>
                                <hr>
                                <br/>
                                <div style="padding-left: 20px;">This module relies on the oxd mediator service. For oxd deployment instructions and license information, please visit the <a target="_blank" href="http://oxd.gluu.org">oxd website.</a></div>
                                <hr>
                                <div style="padding-left: 20px;">
                                    <h3 style=" font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%; ">Server Settings</h3>
                                    <table class="table">

                                        <tr>
                                            <td style="width: 250px"><b>URI of the OpenID Provider:</b></td>
                                            <td><input class="" type="url" name="gluu_provider" id="gluu_provider"
                                                       autofocus="true"  placeholder="Enter URI of the OpenID Provider."
                                                       style="width:400px;"
                                                       value="' . $gluu_provider . '"/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="width: 250px"><b>Custom URI after logout:</b></td>
                                            <td><input class="" type="url" name="gluu_custom_logout" id="gluu_custom_logout"
                                                       autofocus="true"  placeholder="Enter custom URI after logout"
                                                       style="width:400px;"
                                                       value="' . $gluu_custom_logout . '"/>
                                            </td>
                                        </tr>';
			if (!empty($openid_error)) {
				$html .= '<tr>
                                                <td style="width: 250px"><b><font color="#FF0000">*</font>Redirect URL:</b></td>
                                                <td><p>' . $base_url . '/index.php?gluuOption=oxdOpenId</p>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="width: 250px"><b><font color="#FF0000">*</font>Client ID:</b></td>
                                                <td><input class="" type="text" name="gluu_client_id" id="gluu_client_id"
                                                           autofocus="true" placeholder="Enter OpenID Provider client ID."
                                                           style="width:400px;"
                                                           value="';
				if (!empty($gluu_config['gluu_client_id']))
					$html .= $gluu_config['gluu_client_id'];
				$html .= '"/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="width: 250px"><b><font color="#FF0000">*</font>Client Secret:</b></td>
                                                <td>
                                                    <input class="" type="text" name="gluu_client_secret" id="gluu_client_secret"
                                                           autofocus="true" placeholder="Enter OpenID Provider client secret."  style="width:400px;" 
                                                           value="';
				if (!empty($gluu_config['gluu_client_secret'])) $html .= $gluu_config['gluu_client_secret'];
				$html .= '"/>
                                                </td>
                                            </tr>';
			}
			$html .= '<tr>
                                            <td style="width: 250px"><b><font color="#FF0000">*</font>oxd port:</b></td>
                                            <td>
                                                <input class="" type="number" name="gluu_oxd_port" min="0" max="65535"
                                                       value="' . $gluu_config['gluu_oxd_port'] . '"
                                                       style="width:400px;" placeholder="Please enter free port (for example 8099). (Min. number 0, Max. number 65535)."/>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                                <div style="padding-left: 20px">
                                    <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%;">Enrollment
                                        <a data-toggle="tooltip" class="tooltipLink" data-original-title="Choose whether to register new users when they login at an external identity provider. If you disable automatic registration, new users will need to be manually created">
                                            <span class="glyphicon glyphicon-info-sign"></span>
                                        </a>
                                    </h3>
                                    <div>
                                        <p><label><input name="gluu_users_can_register" type="radio" id="gluu_users_can_register"';
			if ($gluu_users_can_register == 1) {
				$html .= " checked ";
			}
			$html .= 'value="1" style="margin-right: 3px"> Automatically register any user with an account in the OpenID Provider</label></p>
                                    </div>
                                    <div>
                                        <p><label ><input name="gluu_users_can_register" type="radio" id="gluu_users_can_register_1"';
			if ($gluu_users_can_register == 2) {
				$html .= " checked ";
			}
			$html .= 'value="2" style="margin-right: 3px"> Only register users with the following role(s) in the OpenID Provider</label></p>
                                        <div style="margin-left: 20px;">
                                            <div id="p_role" >';
			$k = 0;
			if (!empty($gluu_new_roles)) {
				foreach ($gluu_new_roles as $gluu_new_role) {
					if (!$k) {
						$k++;
						$html .= '<p class="role_p" style="padding-top: 10px">
                                                                <input  type="text" name="gluu_new_role[]" class="form-control" style="display: inline; width: 200px !important; "
                                                                        placeholder="Input role name"
                                                                        value="' . $gluu_new_role . '"/>
                                                                <button type="button" class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                            </p>';
					} else {
						$html .= '<p class="role_p" style="padding-top: 10px">
                                                                <input type="text" name="gluu_new_role[]" class="form-control" style="display: inline; width: 200px !important; "
                                                                       placeholder="Input role name"
                                                                       value="' . $gluu_new_role . '"/>
                                                                <button type="button"  class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                                <button type="button"  class="btn btn-xs remrole"><span class="glyphicon glyphicon-minus"></span></button>
                                                            </p>';
					}
				}
			} else {
				$html .= '<p class="role_p" style="padding-top: 10px">
                                                        <input type="text" name="gluu_new_role[]" class="form-control" placeholder="Input role name" style="display: inline; width: 200px !important; " value=""/>
                                                        <button  type="button" class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                    </p>';
			}
			$html .= '</div>
                                        </div>
                                    </div>
                                    <div>
                                        <p>
                                            <label >
                                                <input name="gluu_users_can_register" type="radio" id="gluu_users_can_register_2" ';
			if ($gluu_users_can_register == 3) {
				$html .= " checked ";
			}
			$html .= 'value="3" style="margin-right: 3px">
                                                Disable automatic registration
                                            </label>
                                        </p>
                                    </div>
                                    <table class="table">
                                        
                                    <tr>
                                        <td style="width: 250px"><b>New User Default Role:</b></td>
                                        <td>';
			$user_types = array(
				array('name' => 'Regular User', 'status' => '1'),
				array('name' => 'System Administrator User', 'status' => '3')
			);
			$html .= '<div class="form-group" style="margin-bottom: 0px !important;">
                                                <select id="UserType" class="form-control" name="gluu_user_role" >';
			
			foreach ($user_types as $user_type) {
				$html .= '<option ';
				if ($user_type['status'] == $gluu_user_role) $html .= ' selected ';
				$html .= 'value="' . $user_type['status'] . '">' . $user_type['name'] . '</option>';
				
			}
			$html .= '</select>
                                            </div>
                                        </td>
                                    </tr>';
			if (!empty($openid_error)) {
				$html .= '<tr>
                                                <td style="width: 250px">
                                                    <div><input class="button button-primary button-large" type="submit" name="register" value="Register" style=";width: 120px; float: right;"/></div>
                                                </td>
                                                <td>
                                                    <div><a class="button button-danger button-large" onclick="return confirm(\'Are you sure that you want to remove this OpenID Connect provider? Users will no longer be able to authenticate against this OP.\')" style="padding:2px 5px !important;background-color:red;text-decoration: none;text-align:center; float: left; width: 120px;" href="' . $base_url . 'admin/config/people/gluu_sso/savedata?submit=delete">Delete</a></div>
                                                </td>
                                            </tr>';
			} else {
				$html .= '<tr>';
				if (!empty($gluu_provider)) {
					$html .= '<td style="width: 250px">
                                                        <div><input type="submit" name="register" value="Register" style="width: 120px; float: right;" class="button button-primary button-large"/></div>
                                                    </td>
                                                    <td>
                                                        <a class="button button-primary button-large" onclick="return confirm(\'Are you sure that you want to remove this OpenID Connect provider? Users will no longer be able to authenticate against this OP.\')" style="padding:2px 5px !important;background-color:red;text-decoration: none;text-align:center; float: left; width: 120px;" href="' . $base_url . 'admin/config/people/gluu_sso/savedata?submit=delete">Delete</a>
                                                    </td>';
				} else {
					$html .= '<td style="width: 250px">
                                                        <div><input type="submit" name="submit" value="Register" style="width: 120px; float: left;" class="button button-primary button-large"/></div>
                                                    </td>
                                                    <td>
                                                    </td>';
				}
				$html .= '</tr>';
			}
			$html .= '</table>
                                </div>
                            </div>
                    </div>
                </div>';
		} else {
			$html .= '<div style="padding: 20px !important;" id="accountsetup">
                    <input type="hidden" name="form_key" value="general_oxd_edit"/>
                    <fieldset style="border: 2px solid #53cc6b; padding: 20px">
                        <legend style="border-bottom:none; width: 110px !important;">
                            <img style=" height: 45px;" src="
                            ' . $img_path . '/files/images/icons/gl.png"/>
                        </legend>
                        <div style="padding-left: 10px;margin-top: -20px">
                            <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60% ">Server Settings</h3>
                            <table class="table">
                                <tr>
                                    <td  style="width: 250px"><b>URI of the OpenID Connect Provider:</b></td>
                                    <td><input class="" type="url" name="gluu_provider" id="gluu_provider"
                                               autofocus="true" disabled placeholder="Enter URI of the OpenID Connect Provider."
                                               style="width:400px;"
                                               value="' . $gluu_provider . '"/>
                                    </td>
                                </tr>
                                <tr>
                                    <td  style="width: 250px"><b>Custom URI after logout:</b></td>
                                    <td><input class="" type="url" name="gluu_custom_logout" id="gluu_custom_logout"
                                               autofocus="true"  placeholder="Enter custom URI after logout"
                                               style="width:400px;"
                                               value="' . $gluu_custom_logout . '"/>
                                    </td>
                                </tr>';
			if (!empty($gluu_config['gluu_client_id']) and !empty($gluu_config['gluu_client_secret'])) {
				$html .= '<tr>
                                        <td  style="width: 250px"><b><font color="#FF0000">*</font>Client ID:</b></td>
                                        <td><input class="" type="text" name="gluu_client_id" id="gluu_client_id"
                                                   autofocus="true" placeholder="Enter OpenID Provider client ID."
                                                   style="width:400px; "
                                                   value="';
				if (!empty($gluu_config['gluu_client_id'])) $html .= $gluu_config['gluu_client_id'];
				$html .= '"/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="width: 250px" ><b><font color="#FF0000">*</font>Client Secret:</b></td>
                                        <td>
                                            <input class="" type="text" name="gluu_client_secret" id="gluu_client_secret"
                                                   autofocus="true" placeholder="Enter OpenID Provider client secret."  style="width:400px; " value="';
				if (!empty($gluu_config['gluu_client_secret'])) $html .= $gluu_config['gluu_client_secret'];
				$html .= '"/>
                                        </td>
                                    </tr>';
			}
			$html .= '<tr>
                                    <td  style="width: 250px"><b><font color="#FF0000">*</font>oxd port:</b></td>
                                    <td>
                                        <input class="" type="number"  name="gluu_oxd_port" min="0" max="65535"
                                               value="' . $gluu_config['gluu_oxd_port'] . '"
                                               style="width:400px;" placeholder="Please enter free port (for example 8099). (Min. number 0, Max. number 65535)."/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="width: 250px" ><b>oxd ID:</b></td>
                                    <td>
                                        <input class="" type="text" disabled name="oxd_id"
                                               value="' . gluu_sso_is_oxd_registered_edit() . '"
                                               style="width:400px;background-color: rgb(235, 235, 228);" placeholder="Your oxd ID"/>
                                    </td>
                                </tr>
                            </table>
                        </div>
                             <div style="padding-left: 20px">
                                    <h3 style="font-weight:bold;padding-left: 10px;padding-bottom: 20px; border-bottom: 2px solid black; width: 60%;">Enrollment
                                        <a data-toggle="tooltip" class="tooltipLink" data-original-title="Choose whether to register new users when they login at an external identity provider. If you disable automatic registration, new users will need to be manually created">
                                            <span class="glyphicon glyphicon-info-sign"></span>
                                        </a>
                                    </h3>
                                    <div>
                                        <p><label><input name="gluu_users_can_register" type="radio" id="gluu_users_can_register"';
			if ($gluu_users_can_register == 1) {
				$html .= " checked ";
			}
			$html .= 'value="1" style="margin-right: 3px"> Automatically register any user with an account in the OpenID Provider</label></p>
                                    </div>
                                    <div>
                                        <p><label ><input name="gluu_users_can_register" type="radio" id="gluu_users_can_register_1"';
			if ($gluu_users_can_register == 2) {
				$html .= " checked ";
			}
			$html .= 'value="2" style="margin-right: 3px"> Only register users with the following role(s) in the OpenID Provider</label></p>
                                        <div style="margin-left: 20px;">
                                            <div id="p_role" >';
			$k = 0;
			if (!empty($gluu_new_roles)) {
				foreach ($gluu_new_roles as $gluu_new_role) {
					if (!$k) {
						$k++;
						$html .= '<p class="role_p" style="padding-top: 10px">
                                                                <input  type="text" name="gluu_new_role[]" class="form-control" style="display: inline; width: 200px !important; "
                                                                        placeholder="Input role name"
                                                                        value="' . $gluu_new_role . '"/>
                                                                <button type="button" class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                            </p>';
					} else {
						$html .= '<p class="role_p" style="padding-top: 10px">
                                                                <input type="text" name="gluu_new_role[]" class="form-control" style="display: inline; width: 200px !important; "
                                                                       placeholder="Input role name"
                                                                       value="' . $gluu_new_role . '"/>
                                                                <button type="button"  class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                                <button type="button"  class="btn btn-xs remrole"><span class="glyphicon glyphicon-minus"></span></button>
                                                            </p>';
					}
				}
			} else {
				$html .= '<p class="role_p" style="padding-top: 10px">
                                                        <input type="text" name="gluu_new_role[]" class="form-control" placeholder="Input role name" style="display: inline; width: 200px !important; " value=""/>
                                                        <button  type="button" class="btn btn-xs add_new_role" onclick="test_add()"><span class="glyphicon glyphicon-plus"></span></button>
                                                    </p>';
			}
			$html .= '</div>
                                        </div>
                                    </div>
                                    <div>
                                        <p>
                                            <label >
                                                <input name="gluu_users_can_register" type="radio" id="gluu_users_can_register_2" ';
			if ($gluu_users_can_register == 3) {
				$html .= " checked ";
			}
			$html .= 'value="3" style="margin-right: 3px">
                                                Disable automatic registration
                                            </label>
                                        </p>
                                    </div>
                                    <table class="table">
                                        <tr>
                                            <td style="width: 250px"><b>New User Default Role:</b></td>
                                            <td>';
			$user_types = array(
				array('name' => 'Regular User', 'status' => '1'),
				array('name' => 'System Administrator User', 'status' => '3')
			);
			$html .= '<div class="form-group" style="margin-bottom: 0px !important;">
                                                    <select id="UserType" class="form-control" name="gluu_user_role" >';
			
			foreach ($user_types as $user_type) {
				$html .= '<option ';
				if ($user_type['status'] == $gluu_user_role) $html .= ' selected ';
				$html .= 'value="' . $user_type['status'] . '">' . $user_type['name'] . '</option>';
				
			}
			$html .= '</select>
                                                </div>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="border: none !important;width: 250px">
                                                <div>
                                                    <input type="submit" id="edit-gluu-sso-generaledit-setup-button" onclick="setFormSubmitting()" name="saveButton" value="Save" style="float: right; width: 100px !important;" class="form-submit"/>
                                                </div>
                                            </td>
                                            <td style="border: none !important;width: 250px">
                                                <div>
                                                     <a class="form-submit button button-primary button-large" onclick="edit_cancel_function()" style="text-align:center;float: left; width: 100px !important;" href="' . $base_url . 'admin/config/people/gluu_sso/general">Cancel</a>
                                                </div>
                                            </td>
                                        </tr>
                                    </table>
                        </div>
                    </fieldset>
                </div>
        </div>';
			
			
		}
		
		return $html;
	}
	
	/**
	 * Checking is oxd working;
	 */
	function gluu_sso_is_oxd_registered_edit()
	{
		if (variable_get('gluu_oxd_id')) {
			$oxd_id = variable_get('gluu_oxd_id');
			if (!$oxd_id) {
				return 0;
			} else {
				return $oxd_id;
			}
		}
	}
