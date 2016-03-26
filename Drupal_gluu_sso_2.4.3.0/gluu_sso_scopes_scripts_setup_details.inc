<?php
/*echo '<pre>';
var_dump($_REQUEST);exit;*/
/**
 * @file
 * Contains instructions for setup of miniorange sso Gateway.
 */
function is_oxd_registered(){
    $oxd_id 	= variable_get('oxd_id');
    if($oxd_id ) {
        return 1;
    } else {
        return 0;
    }
}
function is_en_di(){
    $oxd_id 	= variable_get('oxd_id');
    if($oxd_id ) {
        return '';
    } else {
        return 'disabled="false"';
    }
}

/**
 * Form for showing details for Setting up Gateway.
 */
function gluu_sso_scopes_scripts_setup($form, &$form_state){
    $img_path = drupal_get_path('module', 'gluu_sso');
    global $base_url;
    $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files/images';
    if (!variable_get('oxd_id')) {
        $form['markup_1'] = array(
              '#markup' => '<div class="mess_red">Please enter OXD configuration to continue.</div>
                            <h3>Enter gluu configuration.</h3>',
        );
    }
    $form['markup_4'] = array(
        '#markup' => '<style>td{
    font-weight: normal !important;
    border-width: 1px !important;
    border-style: solid !important;
    border-color: #bebfb9 !important;
    padding: 3px 10px !important;
}
    </style><fieldset style="border: 2px solid #53cc6b;">
                        <legend>
                            <div class="about">
                                <img style="height: 45px" src="' . url($bu . '/icons/gl.png') . '">server config
                            </div>
                        </legend>
                        <table style="margin-left: 30px; margin-right: 30px; width: 95% !important;" class="form-table">
                            <tbody>
                            <tr>
                                <th scope="row" style="width: 20% !important;">
                                    Scopes
                                </th>
                                <td>
                                <div>
                                    <div style="float: left">',
    );
    $options = variable_get('gluu_config');
    $scopes = variable_get('gluu_scops');
    $check ='<div>';
    if(!empty($scopes)){
        foreach ($scopes as $custom_scope) {
            if($options && in_array($custom_scope, $options['scope'])){
                if ($custom_scope == 'openid'){
                    $check.="<input style='margin-right:5px;' ".is_en_di()." type='checkbox' checked name='scope[]' disabled id='$custom_scope' value='$custom_scope' /><strong style='margin-right:30px;'>$custom_scope</strong>";
                    $check.="<input style='margin-right:5px;' ".is_en_di()." type='hidden' checked name='scope[]' id='$custom_scope' value='$custom_scope' />";
                }else{
                    $check.="<input style='margin-right:5px;' ".is_en_di()." type='checkbox' checked name='scope[]' id='$custom_scope' value='$custom_scope' /><strong style='margin-right:30px;'>$custom_scope</strong>";
                }

            }else{
                if ($custom_scope == 'openid'){
                    $check.="<input ".is_en_di()." type='hidden' name='scope[]' id='$custom_scope' value='$custom_scope' />";
                    $check.="<input ".is_en_di()." type='checkbox' name='scope[]' id='$custom_scope' disabled value='$custom_scope' /><strong style='margin-right:30px;'>$custom_scope</strong>";
                }else{
                    $check.="<input ".is_en_di()." type='checkbox' name='scope[]' id='$custom_scope' value='$custom_scope' /><strong style='margin-right:30px;'>$custom_scope</strong>";
                }
            }
        }
    }
    $form['markup_5'] = array(
           '#markup' =>$check.'</div></div>
                        </div>
                        </td>
                        </tr>
                        <tr>
                            <th scope="row" rowspan="2">
                                Add scopes
                            </th>
                        <td>
                        <br/>
                        <div>
                        <div id="p_scents">
                        <button '.is_en_di().' type="button" id="add_new_scope">Add scope</button>
                        <p>
                        <input type="text" '.is_en_di().' name="new_scope[]" value="" class="form-text" size="60" maxlength="128" placeholder="Input scope name" />',
    );
    $form['markup_6'] = array(
          '#markup' => '</p>
                        </div>
                        </div>
                        </td>
                        </tr>
                        <tr>
                        <td>
                            <div >
                                <button type="button" style="width:100%;font-size: 20px; color: black; font-weight: bold; cursor: pointer; text-align: center;" id="show_scope_table">Click here to delete scopes</b>
                            </div>
                            <br>
                            <table id="custom_scope_table" class="form-table" style="width:100%;display: none; text-align: center">
                                <tbody>
                                <tr>
                                    <th> <h3>N</h3> </th>
                                    <th><h3>Scope name</h3></th>
                                    <th><h3>Delete</h3></th>
                                </tr>'
    );
    $count = 0;
    if(!empty($scopes)){
        foreach ($scopes as $custom_scope) {
            $count++;
            $name = $count + 6;
            $table_scopes = '';
            if ($custom_scope == 'openid') {
                $table_scopes = "<tr>
                                <td>" . $count . "</td>
                                <td>" . $custom_scope . "</td>
                                <td></td>
                              </tr>";
            }else{
                $table_scopes = "<tr>
                                <td>" . $count . "</td>
                                <td>" . $custom_scope . "</td>
                                <td><button ".is_en_di()."  type='submit' value=' $custom_scope ' name='set_oxd_config'>Delete</button></td>
                              </tr>";
            }

            $form["markup_" . $name] = array(
                '#markup' => $table_scopes
            );
        }
    }
    $html = '';
    $custom_scripts = variable_get('oxd_openid_custom_scripts');
    if(!empty($custom_scripts)){
        foreach($custom_scripts as $custom_script){
            $html.='
                                                            <input style="margin-right:5px;"  type="checkbox" '.is_en_di().' id="'.$custom_script['value'].'_enable" class="app_enable" name="oxd_openid_'.$custom_script['value'].'_enable" value="1" onchange="previewLoginIcons();"';
            if( variable_get('oxd_openid_'.$custom_script['value'].'_enable') == 1 ){
                $html.='checked';
            }
            $html.='/><strong style="margin-right:30px;">'.$custom_script['name'].'</strong>';
        }
    }
    $name = $count + 7;
    $form["markup_" . $name] = array(
        '#markup' => '</tbody>
                        </table>
                        <br>
                        </td>
                        </tr>
                            <th>
                                <p>Custom scripts</p>
                                <i style="color: red">Select applications to enable login for your users.</i>
                            </th>
                            <td>
                                <h3>Manage Authentication</h3>
                                <p>An OpenID Connect Provider (OP) like the Gluu Server may provide many different work flows for
                                authentication. For example, an OP may offer password authentication, token authentication, social
                                authentication, biometric authentication, and other different mechanisms. Offering a lot of different
                                types of authentication enables an OP to offer the most convenient, secure, and affordable option to
                                identify a person, depending on the need to mitigate risk, and the sensors and inputs available on the
                                device that the person is using.
                                </p>
                                <p>
                                The OP enables a client (like a Drupal site), to signal which type of authentication should be
                                used. The client can register a
                                <a target="_blank" href="http://openid.net/specs/openid-connect-registration-1_0.html#ClientMetadata">default_acr_value</a>
                                or during the authentication process, a client may request a specific type of authentication using the
                                 <a target="_blank" href="http://openid.net/specs/openid-connect-core-1_0.html#AuthRequest">acr_values</a> parameter.
                                This is the mechanism that the Gluu SSO Plugin uses: each login icon corresponds to a acr request value.
                                For example, and acr may tell the OpenID Connect to use Facebook, Google or even plain old password
                                authentication. The nice thing about this approach is that your applications (like Drupal) don\'t have
                                to implement the business logic for social login--it\'s handled by the OpenID Connect Provider.
                                </p>

                                <p>
                                If you are using the Gluu Server as your OP, you\'ll notice that in the Manage Custom Scripts
                                tab of oxTrust (the Gluu Server admin interface), each authentication script has a name.
                                This name corresponds to the acr value.  The default acr for password authentication is set in
                                the
                                <a target="_blank" href="https://www.gluu.org/docs/admin-guide/configuration/#manage-authentication">LDAP Authentication</a>,
                                section--look for the "Name" field. Likewise, each custom script has a "Name", for example
                                see the
                                <a target="_blank" href="https://www.gluu.org/docs/admin-guide/configuration/#manage-custom-scripts">Manage Custom
                                Scripts</a> section.
                                </p><hr><br/>'.$html.'</td>
                            <tr>
                            <th scope="row" rowspan="2">
                                <p>Add multiple custom scripts</p>
                                <i style="color:red" class="description">Both fields are required</p>
                            </th>
                            <td>

                                <br/>
                                <div>
                                    <button '.is_en_di().'  type="button" id="add_new_suctom_script">Add acr</button>
                                    <input '.is_en_di().'  type="hidden" name="count_scripts" value="1" id="count_scripts">
                                    <div id="p_scents_script">
                                        <p>
                                            <input '.is_en_di().'  type="text" style="margin-right: 5px " class="form-text" name="new_custom_script_name_1" size="30"  placeholder="Display name (example Google+)">
                                            <input '.is_en_di().' type="text" style="margin-right: 5px " class="form-text" name="new_custom_script_value_1" size="40"  placeholder="ACR Value (script name in the Gluu Server)">
                                            <input '.is_en_di().' type="file" name="image_url_1" class="button-secondary" value="Upload app image">
                                        </p>
                                    </div>
                                </div>
                            </td>
                            </tr>
                            <tr>
                            <td>
                        <div >
                        <button type="button" style="width:100%;font-size: 20px; color: black; font-weight: bold; cursor: pointer; text-align: center;" id="show_script_table">Click here to delete ACRs</button>
                        </div>
                        <br>
                        <table id="custom_script_table" class="form-table" style="width:100%;display: none; text-align: center">
                        <tbody>
                        <tr>
                        <th> <h3>N</h3> </th>
                        <th><h3>Display Name</h3></th>
                        <th><h3>ACR Value</h3></th>
                        <th><h3>Image</h3></th>
                        <th><h3>Delete</h3></th>
                        </tr>',
    );
    $count2 = 0;
    $custom_script = variable_get('oxd_openid_custom_scripts');
    if(!empty($custom_script)) {
        foreach ($custom_script as $custom_scripts) {
            $count2++;
            $k = $custom_scripts['value'];
            $name = $count + 8;
            $form["markup_" . $name] = array(
                '#markup' => '<tr>
                            <td>' . $count2 . '</td>
                            <td>' . $custom_scripts['name'] . '</td>
                            <td>' . $custom_scripts['value'] . '</td>
                            <td>
                                <img src="' . $custom_scripts['image'] . '" width="40px" height="40px">
                            </td>
                            <td>
                                <button '.is_en_di().' type="submit" value="'. $k .'" name="set_custom_script">Delete</button>
                            </td>
                         </tr>'
            );
            $count++;
        }
    }
    $name = $count + 8;
    $form["markup_" . $name] = array(
        '#markup' =>   '</tbody>
                        </table>
                        <br>
                        </td>
                        </tr>
                        </tbody>
                        </table>
                        </fieldset>',
    );
    $form['gluu_sso_gateway_instructions_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update scopes and scripts settings'),
        '#submit' => array('gluu_sso_scope_script_submit'),
        '#disabled' =>!is_oxd_registered()
    );
    return $form;
}

/**
 * Form submit method.
 */
function gluu_sso_gateway_proceed($form, &$form_state){
    $instructions_read = $form['gluu_sso_read_instructions']['#value'];
    if ($instructions_read == 1) {
        variable_set('gluu_sso_gateway_check', TRUE);
        drupal_set_message(t('Configure your Gateway URL'));
        $form_state['redirect'] = 'admin/config/people/gluu_sso/gateway_configuration';
    }
    else {
        variable_set('gluu_sso_gateway_check', FALSE);
        drupal_set_message(t('Please follow the instructions to setup the Gateway and check the checkbox below'));
    }
}

function gluu_sso_scope_script_submit(&$form, $form_state) {
    if(is_oxd_registered()) {
        $custom_scripts = variable_get('oxd_openid_custom_scripts');
        foreach($custom_scripts as $custom_script){
            variable_set( 'oxd_openid_'.$custom_script['value'].'_enable', isset( $_POST['oxd_openid_'.$custom_script['value'].'_enable']) ? $_POST['oxd_openid_'.$custom_script['value'].'_enable'] : 0);
        }
        $error = true;
        $error_array = array();
        $oxd_config = !empty(variable_get('gluu_config'))? variable_get('gluu_config') : array();
        $oxd_config['scope'] = !empty($_POST['scope']) && isset($_POST['scope'])? $_POST['scope'] : $oxd_config['scope'];
        variable_set('gluu_config', $oxd_config );
        if(!empty($_POST['new_scope']) && isset($_POST['new_scope'])){
            foreach($_POST['new_scope'] as $scope){
                if($scope){
                    $get_scopes = variable_get('gluu_scops');
                    array_push($get_scopes, $scope);
                    variable_set('gluu_scops', $get_scopes );
                }
            }
        }
        if(!empty($_POST['set_oxd_config']) && isset($_POST['set_oxd_config'])){
            $custom_scripts = variable_get('gluu_scops');
            $up_cust_sc =  array();
            foreach($custom_scripts as $custom_script){
                if($custom_script !=trim($_POST['set_oxd_config'])){
                    array_push($up_cust_sc,$custom_script);
                }
            }
            variable_set('gluu_scops', $up_cust_sc );
        }
        if(!empty($_POST['set_custom_script']) && isset($_POST['set_custom_script'])){
            $custom_scripts = variable_get('oxd_openid_custom_scripts');
            $up_cust_sc =  array();
            foreach($custom_scripts as $custom_script){
                if($custom_script['value'] != trim($_POST['set_custom_script'])){
                    array_push($up_cust_sc,$custom_script);
                }
            }
            variable_set('oxd_openid_custom_scripts', $up_cust_sc );
        }

        if(isset($_POST['count_scripts'])){
            $img_path = drupal_get_path('module', 'gluu_sso');
            for($i=1; $i<=$_POST['count_scripts']; $i++){
                if(isset($_POST['new_custom_script_name_'.$i]) && !empty($_POST['new_custom_script_name_'.$i]) &&
                   isset($_POST['new_custom_script_value_'.$i]) && !empty($_POST['new_custom_script_value_'.$i]) && isset($_FILES['image_url_'.$i]) && !empty($_FILES['image_url_'.$i])){
                    $custom_scripts = variable_get('oxd_openid_custom_scripts');
                    foreach($custom_scripts as $custom_script){
                        if($custom_script['value'] == $_POST['new_custom_script_value_'.$i] || $custom_script['name'] == $_POST['new_custom_script_name_'.$i]){
                            $error = false;
                            array_push($error_array, $i);
                        }
                    }
                    if($error){
                        $temp = explode(".", basename($_FILES['image_url_'.$i]["name"]));

                        $newfilename = round(microtime(true)) . '.' . end($temp);

                        $target_dir = $img_path . '/files/images/icons/';
                        $target_file = $target_dir . $newfilename;


                            move_uploaded_file($_FILES['image_url_'.$i]["tmp_name"], $target_file);
                        array_push($custom_scripts, array('name'=>$_POST['new_custom_script_name_'.$i],'image'=>'/'.$target_file,'value'=>$_POST['new_custom_script_value_'.$i]));
                        variable_set('oxd_openid_custom_scripts', $custom_scripts );
                    }else{
                        drupal_set_message(t('Name = '.$_POST['new_custom_script_name_'.$i]. ' or value = '. $_POST['new_custom_script_value_'.$i]. ' is exist.'), 'error');
                    }
                }
            }
        }
        $config_option = variable_get('gluu_config');
        $update_site_registration = new Update_site_registration();
        $update_site_registration->setRequestOxdId(variable_get('oxd_id'));
        $update_site_registration->setRequestAcrValues($config_option['acr_values']);
        $update_site_registration->setRequestAuthorizationRedirectUri($config_option['authorization_redirect_uri']);
        $update_site_registration->setRequestRedirectUris($config_option['redirect_uris']);
        $update_site_registration->setRequestGrantTypes($config_option['grant_types']);
        $update_site_registration->setRequestResponseTypes(['code']);
        $update_site_registration->setRequestLogoutRedirectUri($config_option['logout_redirect_uri']);
        $update_site_registration->setRequestContacts([$config_option['reg_email']]);
        $update_site_registration->setRequestApplicationType('web');
        $update_site_registration->setRequestClientLogoutUri($config_option['logout_redirect_uri']);
        $update_site_registration->setRequestScope($config_option['scope']);
        $status = $update_site_registration->request();
        $up_mes = '';
        if(!$status['status']){
            $up_mes = $status['message'];
        }
        if(!$error){
            if($up_mes){
                $error_message = $up_mes.'<br/>';
            }else{
                $error_message = '';
            }

            foreach($error_array as $error_a){
                $error_message.= 'Name = '.$_POST['new_custom_script_name_'.$error_a]. ' or value = '. $_POST['new_custom_script_value_'.$error_a]. ' is exist.<br/>';
            }
            drupal_set_message(t($error_message), 'error');
        }else{
            if($up_mes){
                $error_message = "Problem with updating scopes in gluu server: ".$up_mes.'<br/>';
            }else{
                $error_message = '';
            }
            drupal_set_message(t($error_message.'Your settings are saved successfully.'));
        }
    }else {
        drupal_set_message(t('Please register an account before trying to enable any app.'), 'error');
    }
}