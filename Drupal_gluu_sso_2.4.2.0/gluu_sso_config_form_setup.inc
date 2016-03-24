<?php
/**
 * @file
 * Contains instructions for setup of miniorange sso Gateway.
 */

/**
 * Checking extension status.
 */
function is_oxd_registered() {
    $oxd_id 	= variable_get('oxd_id');
    if($oxd_id ) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Checking extension status for disabling all inputs and buttons.
 */
function is_en_di() {
    $oxd_id 	= variable_get('oxd_id');
    if($oxd_id ) {
        return '';
    } else {
        return 'disabled';
    }
}

/**
 * Html setting page.
 */
function oxd_openid_apps_config() {

    $html='
        <input type="hidden" name="option" value="oxd_openid_enable_apps" />
        <div class="oxd_openid_table_layout">';
    if(!is_oxd_registered()) {
        $html.='<div class="mess_red">
                     Please enter OXD configuration to continue.
                </div><br/>';
    }

    $html.='

            <table>

                <br/>
                <tr>
                    <td>
                        ';
    $custom_scripts = variable_get('oxd_openid_custom_scripts');
    if(!empty($custom_scripts)){
        foreach($custom_scripts as $custom_script){
            $html.='<td>
                                    <input type="hidden" '.is_en_di().' id="'.$custom_script['value'].'_enable" class="app_enable" name="oxd_openid_'.$custom_script['value'].'_enable" value="1" onchange="previewLoginIcons();"';
            if( variable_get('oxd_openid_'.$custom_script['value'].'_enable') == 1 ){
                $html.='checked';
            }
            $html.='/><strong style="display: none">'.$custom_script['name'].'</strong></td>';
        }
    }
    $html.='
                    </td>
                </tr>
                <tr>
                    <td>
                    <h3>Gluu login config</h3>
                     <p>Customize shape, theme and size of the login icons</p>
                        <br>
                        <hr>
                        <h3>Customize Login Icons</h3>

                    </td>
                </tr>
                <tr>
                    <td>
                        <b>Shape</b>
                        <b style="margin-left:130px; display: none">Theme</b>
                        <b style="margin-left:130px;">Space between Icons</b>
                        <b style="margin-left:86px;">Size of Icons</b>
                    </td>
                </tr>
                <tr>';
    $html.='<td class="oxd_openid_table_td_checkbox"><input type="radio"
                            name="oxd_openid_login_theme"
                             value="circle"
                              onclick="checkLoginButton();
                               oxdLoginPreview(
                               document.getElementById(\'oxd_login_icon_size\').value,
                               \'circle\',
                               setLoginCustomTheme(),
                               document.getElementById(\'oxd_login_icon_custom_color\').value,
                               document.getElementById(\'oxd_login_icon_space\').value
                               )" '.is_en_di();
    if( variable_get('oxd_openid_login_theme') == 'circle') {
        $html.='checked';
    }
    $html.='/>Round <span style="margin-left:106px; display: none">
                    <input type="radio"
                    id="oxd_openid_login_default_radio"
                      name="oxd_openid_login_custom_theme"
                       value="default"
                        onclick="
                        checkLoginButton();
                        oxdLoginPreview(
                                        setSizeOfIcons(),
                                        setLoginTheme(),
                                        \'default\',
                                        document.getElementById(\'oxd_login_icon_custom_color\').value,
                                        document.getElementById(\'oxd_login_icon_space\').value,
                                        document.getElementById(\'oxd_login_icon_height\').value
                                        )" '.is_en_di();
    if( variable_get('oxd_openid_login_custom_theme') == 'default' ){
        $html.='checked';
    }
    $html.='/>Default
                        </span>
                        <span  style="margin-left:111px;">
                                <input style="width:50px" onkeyup="oxdLoginSpaceValidate(this)" id="oxd_login_icon_space" name="oxd_login_icon_space" type="text" class="form-text" value="'.variable_get('oxd_login_icon_space').'" '.is_en_di().'/>
                                <input id="oxd_login_space_plus" type="button" style="width: 50px;" value="+" onmouseup="oxdLoginPreview(setSizeOfIcons() ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value)" '.is_en_di().'/>
                                <input id="oxd_login_space_minus" type="button" style="width: 50px;" value="-" onmouseup="oxdLoginPreview(setSizeOfIcons()  ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value)" '.is_en_di().'/>
                        </span>
                        <span id="commontheme" style="margin-left:50px; ">
                        <input style="width:50px" id="oxd_login_icon_size" onkeyup="oxdLoginSizeValidate(this)" name="oxd_login_icon_custom_size" type="text" class="form-text" value="'.variable_get('oxd_login_icon_custom_size').'" '.is_en_di().'/>
                        <input id="oxd_login_size_plus" style="width: 50px;" type="button" value="+" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_size\').value ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value, document.getElementById(\'oxd_login_icon_space\').value)" '.is_en_di().'/>
                        <input id="oxd_login_size_minus" type="button" style="width: 50px;" value="-" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_size\').value ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value)" '.is_en_di().'/>
                        </span>
                        <span style="margin-left:115px; display: none" class="longbuttontheme">Width:&nbsp;
                        <input style="width:50px" id="oxd_login_icon_width" onkeyup="oxdLoginWidthValidate(this)" name="oxd_login_icon_custom_width" type="text" value="'.variable_get('oxd_login_icon_custom_width').'" '.is_en_di().'/>
                        <input id="oxd_login_width_plus" type="button" style="width: 50px;" value="+" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_width\').value ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di().'/>
                        <input id="oxd_login_width_minus" type="button" style="width: 50px;" value="-" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_width\').value ,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di().'/>
                        </span>
                    </td>';
    drupal_add_js('var tempHorSize = '.variable_get('oxd_login_icon_custom_size').'
                        var tempHorTheme = "'.variable_get('oxd_openid_login_theme').'";
                        var tempHorCustomTheme = "'.variable_get('oxd_openid_login_custom_theme').'"
                        var tempHorCustomColor = "'.variable_get('oxd_login_icon_custom_color').'"
                        var tempHorSpace = "'.variable_get('oxd_login_icon_space').'"
                        var tempHorHeight = '.variable_get('oxd_login_icon_custom_height').'
                        var customScript = '.json_encode(variable_get('oxd_openid_custom_scripts')).';', 'inline');

    $html.='</tr>
                <tr>
                    <td class="oxd_openid_table_td_checkbox">
                        <input type="radio" name="oxd_openid_login_theme"  value="oval" onclick="checkLoginButton();oxdLoginPreview(document.getElementById(\'oxd_login_icon_size\').value,\'oval\',setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_size\').value )" '.is_en_di();
    if( variable_get('oxd_openid_login_theme') == 'oval' ) {
        $html.='checked';
    }
    $html.='/>Rounded Edges
                        <span style="margin-left:50px; display: none">
                            <input type="radio" id="oxd_openid_login_custom_radio"  name="oxd_openid_login_custom_theme" value="custom" onclick="checkLoginButton();oxdLoginPreview(setSizeOfIcons(), setLoginTheme(),\'custom\',document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di().'/>';
    if( variable_get('oxd_openid_login_custom_theme') == 'custom' ){
        $html.='checked';
    }
    $html.='/>Custom Background*
                            </span>
                        <span style="margin-left:249px; display: none" class="longbuttontheme" >Height:
                    <input style="width:50px" id="oxd_login_icon_height" onkeyup="oxdLoginHeightValidate(this)" name="oxd_login_icon_custom_height" type="text" value="'.variable_get('oxd_login_icon_custom_height').'" '.is_en_di().'
                    <input id="oxd_login_height_plus" type="button" value="+" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_width\').value,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di().'/>
                    <input id="oxd_login_height_minus" type="button" value="-" onmouseup="oxdLoginPreview(document.getElementById(\'oxd_login_icon_width\').value,setLoginTheme(),setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di().'/>
                    </span>
                    </td>
                </tr>
                <tr>
                    <td class="oxd_openid_table_td_checkbox">
                        <input type="radio" name="oxd_openid_login_theme" value="square" onclick="checkLoginButton();oxdLoginPreview(document.getElementById(\'oxd_login_icon_size\').value ,\'square\',setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_size\').value )" '.is_en_di();
    if( variable_get('oxd_openid_login_theme') == 'square' ) {
        $html.='checked';
    }
    $html.='/>Square
            <span style="margin-left:113px; display: none">
            <input id="oxd_login_icon_custom_color" style="width:135px;" name="oxd_login_icon_custom_color"  class="color" value="'.variable_get('oxd_login_icon_custom_color').'" onchange="oxdLoginPreview(setSizeOfIcons(), setLoginTheme(),\'custom\',document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value)" '.is_en_di().'/>';
    $html.='</span>
            </td>
                </tr>
                <tr style="display: none">
                    <td class="oxd_openid_table_td_checkbox">
                        <input type="radio" id="iconwithtext" name="oxd_openid_login_theme" value="longbutton" onclick="checkLoginButton();oxdLoginPreview(document.getElementById(\'oxd_login_icon_width\').value ,\'longbutton\',setLoginCustomTheme(),document.getElementById(\'oxd_login_icon_custom_color\').value,document.getElementById(\'oxd_login_icon_space\').value,document.getElementById(\'oxd_login_icon_height\').value)" '.is_en_di();
                if( variable_get('oxd_openid_login_theme') == 'longbutton' ) {
                    $html.='checked';
                }
                $html.='/>Long Button with Text
                    </td>
                </tr>
                <tr>
                    <td>
                        <br><b>Preview : </b>
                        <br/><span hidden id="no_apps_text">No apps selected</span>
                        <div >';
    if(!empty($custom_scripts)){
        foreach($custom_scripts as $custom_script){
            $html.='<img class="oxd_login_icon_preview" id="oxd_login_icon_preview_'.$custom_script['value'].'" src="'.$custom_script['image'].'" />';
        }
    }
    $html.='</div>
    <div style="display: none">';
    if(!empty($custom_scripts)){
        foreach($custom_scripts as $custom_script){
            $html.='<a id="oxd_login_button_preview_'.$custom_script['value'].'" class="btn btn-block btn-defaulttheme btn-social btn-facebook btn-custom-size"> <i class="fa "></i>';
            $html.=variable_get('oxd_openid_login_button_customize_text').' '.$custom_script['name'].'</a>';
        }
    }
    $html.='</div>
    <div style="display: none">';
    if(!empty($custom_scripts)) {
        foreach ($custom_scripts as $custom_script) {
            $html .= '<i class="oxd_custom_login_icon_preview fa " id="oxd_custom_login_icon_preview_' . $custom_script['value'] . '"  style="color:#ffffff;text-align:center;margin-top:5px;"></i>';
        }
    }
    $html.='</div>
    <div style="display: none">';
    if(!empty($custom_scripts)){
        foreach($custom_scripts as $custom_script){
            $html.='<a id="oxd_custom_login_button_preview_'.$custom_script['value'].'" class="btn btn-block btn-customtheme btn-social  btn-custom-size"> <i class="fa "></i>';
            $html.=variable_get('oxd_openid_login_button_customize_text').' '.$custom_script['name'].'</a>';
        }
    }
    $html.='</div>
                </td>
                </tr>';
    $html.='</table>
        </div>
    ';
    $html.='<script>
                function oxdLoginIncrement(e,t,r,a,i){
                    var h,s,c=!1,_=a;
                    s=function(){
                        "add"==t&&r.value<60?r.value++:"subtract"==t&&r.value>20&&r.value--,h=setTimeout(s,_),_>20&&(_*=i),c||(document.onmouseup=function(){
                                clearTimeout(h),document.onmouseup=null,c=!1,_=a
                            },
                            c=!0)
                        };
                        e.onmousedown=s;
                }
                function oxdLoginSpaceIncrement(e,t,r,a,i){
                    var h,s,c=!1,_=a;s=function(){
                        "add"==t&&r.value<60?r.value++:"subtract"==t&&r.value>0&&r.value--,h=setTimeout(s,_),_>20&&(_*=i),c||(document.onmouseup=function(){
                            clearTimeout(h),document.onmouseup=null,c=!1,_=a
                        },c=!0)
                    },e.onmousedown=s;
                }
                function oxdLoginWidthIncrement(e,t,r,a,i){
                var h,s,c=!1,_=a;s=function(){
                    "add"==t&&r.value<1000?r.value++:"subtract"==t&&r.value>140&&r.value--,h=setTimeout(s,_),_>20&&(_*=i),c||(document.onmouseup=function(){clearTimeout(h),document.onmouseup=null,c=!1,_=a},c=!0)},e.onmousedown=s;}
                function setLoginTheme(){return jQuery(\'input[name=oxd_openid_login_theme]:checked\', \'#gluu-sso-config-form\').val();}
                function setLoginCustomTheme(){return jQuery(\'input[name=oxd_openid_login_custom_theme]:checked\', \'#gluu-sso-config-form\').val();}
                function setSizeOfIcons(){

                    if((jQuery(\'input[name=oxd_openid_login_theme]:checked\', \'#gluu-sso-config-form\').val()) == \'longbutton\'){
                        return document.getElementById(\'oxd_login_icon_width\').value;
                    }else{
                        return document.getElementById(\'oxd_login_icon_size\').value;
                    }
                }
                oxdLoginPreview(setSizeOfIcons(),tempHorTheme,tempHorCustomTheme,tempHorCustomColor,tempHorSpace,tempHorHeight);
                function oxdLoginPreview(t,r,l,p,n,h){
                    if(l == \'default\'){
                        if(r == \'longbutton\'){
                            var a = "btn-defaulttheme";
                            jQuery("."+a).css("width",t+"px");
                            jQuery("."+a).css("padding-top",(h-29)+"px");
                            jQuery("."+a).css("padding-bottom",(h-29)+"px");
                            jQuery(".fa").css("padding-top",(h-35)+"px");
                            jQuery("."+a).css("margin-bottom",(n-5)+"px");
                        }else{
                            var a="oxd_login_icon_preview";
                            jQuery("."+a).css("margin-left",(n-4)+"px");

                            if(r=="circle"){
                                jQuery("."+a).css({height:t,width:t});
                                jQuery("."+a).css("borderRadius","999px");
                            }else if(r=="oval"){
                                jQuery("."+a).css("borderRadius","5px");
                                jQuery("."+a).css({height:t,width:t});
                            }else if(r=="square"){
                                jQuery("."+a).css("borderRadius","0px");
                                jQuery("."+a).css({height:t,width:t});
                            }
                        }
                    }
                    else if(l == \'custom\'){
                        if(r == \'longbutton\'){
                            var a = "btn-customtheme";
                            jQuery("."+a).css("width",(t)+"px");
                            jQuery("."+a).css("padding-top",(h-29)+"px");
                            jQuery("."+a).css("padding-bottom",(h-29)+"px");
                            jQuery(".fa").css("padding-top",(h-35)+"px");
                            jQuery("."+a).css("margin-bottom",(n-5)+"px");
                            jQuery("."+a).css("background","#"+p);
                        }else{
                            var a="oxd_custom_login_icon_preview";
                            jQuery("."+a).css({height:t-8,width:t});
                            jQuery("."+a).css("padding-top","8px");
                            jQuery("."+a).css("margin-left",(n-4)+"px");

                            if(r=="circle"){
                                jQuery("."+a).css("borderRadius","999px");
                            }else if(r=="oval"){
                                jQuery("."+a).css("borderRadius","5px");
                            }else if(r=="square"){
                                jQuery("."+a).css("borderRadius","0px");
                            }
                            jQuery("."+a).css("background","#"+p);
                            jQuery("."+a).css("font-size",(t-16)+"px");
                        }
                    }
                    previewLoginIcons();
                }
                function checkLoginButton(){
                    if(document.getElementById(\'iconwithtext\').checked) {
                        if(setLoginCustomTheme() == \'default\'){
                            jQuery(".oxd_login_icon_preview").hide();
                            jQuery(".oxd_custom_login_icon_preview").hide();
                            jQuery(".btn-customtheme").hide();
                            jQuery(".btn-defaulttheme").show();
                        }else if(setLoginCustomTheme() == \'custom\'){
                            jQuery(".oxd_login_icon_preview").hide();
                            jQuery(".oxd_custom_login_icon_preview").hide();
                            jQuery(".btn-defaulttheme").hide();
                            jQuery(".btn-customtheme").show();
                        }
                        jQuery("#commontheme").hide();
                        jQuery(".longbuttontheme").show();
                    }else {
                        if(setLoginCustomTheme() == \'default\'){
                            jQuery(".oxd_login_icon_preview").show();
                            jQuery(".btn-defaulttheme").hide();
                            jQuery(".btn-customtheme").hide();
                            jQuery(".oxd_custom_login_icon_preview").hide();
                        }else if(setLoginCustomTheme() == \'custom\'){
                            jQuery(".oxd_login_icon_preview").hide();
                            jQuery(".oxd_custom_login_icon_preview").show();
                            jQuery(".btn-defaulttheme").hide();
                            jQuery(".btn-customtheme").hide();
                        }
                        jQuery("#commontheme").show();
                        jQuery(".longbuttontheme").hide();
                    }
                    previewLoginIcons();
                }
                function previewLoginIcons() {
                    var flag = 0;
                    customScript.forEach(function(value) {
                        if (document.getElementById(value.value+\'_enable\').checked) {
                            flag = 1;
                            if(document.getElementById(\'oxd_openid_login_default_radio\').checked && !document.getElementById(\'iconwithtext\').checked)
                                jQuery("#oxd_login_icon_preview_"+value.value).show();
                            if(document.getElementById(\'oxd_openid_login_custom_radio\').checked && !document.getElementById(\'iconwithtext\').checked)
                                jQuery("#oxd_custom_login_icon_preview_"+value.value).show();
                            if(document.getElementById(\'oxd_openid_login_default_radio\').checked && document.getElementById(\'iconwithtext\').checked)
                                jQuery("#oxd_login_button_preview_"+value.value).show();
                            if(document.getElementById(\'oxd_openid_login_custom_radio\').checked && document.getElementById(\'iconwithtext\').checked)
                                jQuery("#oxd_custom_login_button_preview_"+value.value).show();
                        } else if(!document.getElementById(value.value+\'_enable\').checked){
                            jQuery("#oxd_login_icon_preview_"+value.value).hide();
                            jQuery("#oxd_custom_login_icon_preview_"+value.value).hide();
                            jQuery("#oxd_login_button_preview_"+value.value).hide();
                            jQuery("#oxd_custom_login_button_preview_"+value.value).hide();
                        }
                    });
                    if(flag) {
                    jQuery("#no_apps_text").hide();
                } else {
                    jQuery("#no_apps_text").show();
                }
                }
                oxdLoginIncrement(document.getElementById(\'oxd_login_size_plus\'), "add", document.getElementById(\'oxd_login_icon_size\'), 300, 0.7);
                oxdLoginIncrement(document.getElementById(\'oxd_login_size_minus\'), "subtract", document.getElementById(\'oxd_login_icon_size\'), 300, 0.7);
                oxdLoginSpaceIncrement(document.getElementById(\'oxd_login_space_plus\'), "add", document.getElementById(\'oxd_login_icon_space\'), 300, 0.7);
                oxdLoginSpaceIncrement(document.getElementById(\'oxd_login_space_minus\'), "subtract", document.getElementById(\'oxd_login_icon_space\'), 300, 0.7);
                oxdLoginWidthIncrement(document.getElementById(\'oxd_login_width_plus\'), "add", document.getElementById(\'oxd_login_icon_width\'), 300, 0.7);
                oxdLoginWidthIncrement(document.getElementById(\'oxd_login_width_minus\'), "subtract", document.getElementById(\'oxd_login_icon_width\'), 300, 0.7);
                checkLoginButton();
            </script>';
    return $html;
}

/**
 * App form for setting.
 */
function gluu_sso_config_form($form, &$form_state)
{
    $form['gluu_sso_config_form_setup_save_settings'] = array(
        '#type' => 'submit',
        '#value' => t('Save settings'),
        '#submit' => array('gluu_sso_save_settings'),
        '#disabled'=>!is_oxd_registered()
    );
    $form['markup_1'] = array(
        '#markup' => oxd_openid_apps_config()
    );

    return $form;
}

/**
 * Button submit for save settings.
 */
function gluu_sso_save_settings(&$form, $form_state) {
    if($form['gluu_sso_config_form_setup_save_settings']['#value']){

        variable_set('oxd_openid_login_theme',$_POST['oxd_openid_login_theme'] );
        variable_set('oxd_login_icon_custom_size',$_POST['oxd_login_icon_custom_size'] );
        variable_set('oxd_login_icon_space',$_POST['oxd_login_icon_space'] );
        variable_set('oxd_login_icon_custom_width',$_POST['oxd_login_icon_custom_width'] );
        variable_set('oxd_login_icon_custom_height',$_POST['oxd_login_icon_custom_height'] );
        variable_set('oxd_openid_login_custom_theme',$_POST['oxd_openid_login_custom_theme'] );
        variable_set('oxd_login_icon_custom_color', $_POST['oxd_login_icon_custom_color'] );
        drupal_set_message(t('All settings saved successfully!'));
    }
}