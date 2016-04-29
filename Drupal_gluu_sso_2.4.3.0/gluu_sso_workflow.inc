<?php
/**
 * @file
 * Contains workflow of the miniOrange Active Directory/sso Module.
 */

/**
 * Show workflow.
 */
function gluu_sso_workflow($form, &$form_state) {
  global $base_url;
  $bu = $base_url.'/sites/all/modules/Drupal_gluu_sso_2.4.3.0/files';

  $form['markup_1'] = array(
      '#markup' => '<h1><a id="Drupal_GLUU_SSO_module_0"></a>Drupal GLUU SSO module</h1>
                <p><img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-Drupal-module/master/plugin.jpg" alt="image"></p>
                <p>Drupal-GLUU-SSO module gives access for login to your Drupal site, with the help of GLUU server.</p>
                <p>There are already 2 versions of Drupal-GLUU-SSO (2.4.2.0 and 2.4.3.0) modules, each in its turn is working with oxD and GLUU servers.
                    For example if you are using Drupal-gluu-sso-2.4.3.0 module, you need to connect with oxD-server-2.4.2.</p>
                <p>Now I want to explain in details how to use module step by step.</p>
                <p>Module will not be working if your host does not have https://.</p>
                <h2><a id="Step_1_Install_Gluuserver_13"></a>Step 1. Install Gluu-server</h2>
                <p>(version 2.4.2 or 2.4.3)</p>
                <p>If you want to use external gluu server, You can not do this step.</p>
                <p><a target="_blank" href="https://www.gluu.org/docs/deployment/">Gluu-server installation gide</a>.</p>
                <h2><a id="Step_2_Download_oxDserver_21"></a>Step 2. Download oxD-server</h2>
                <p><a target="_blank" href="https://ox.gluu.org/maven/org/xdi/oxd-server/2.4.3.Final/oxd-server-2.4.3.Final-distribution.zip">Download oxD-server-2.4.3.Final</a>.</p>
                <h2><a id="Step_3_Unzip_and_run_oXDserver_31"></a>Step 3. Unzip and run oXD-server</h2>
                <ol>
                    <li>Unzip your oxD-server.</li>
                    <li>Open the command line and navigate to the extracted folder in the conf directory.</li>
                    <li>Open oxd-conf.json file.</li>
                    <li>If your server is using 8099 port, please change “port” number to free port, which is not used.</li>
                    <li>Set parameter “op_host”:“Your gluu-server-url (internal or external)”</li>
                    <li>Open the command line and navigate to the extracted folder in the bin directory.</li>
                    <li>For Linux environment, run sh <a href="http://oxd-start.sh">oxd-start.sh</a>&amp;.</li>
                    <li>For Windows environment, run oxd-start.bat.</li>
                    <li>After the server starts, go to Step 4.</li>
                </ol>
                <h2><a id="Step_6_General_73"></a>Step 4. General</h2>
                <p><img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d5.png" alt="General"></p>
                <ol>
                    <li>Admin Email: please add your or admin email address for registrating site in Gluu server.</li>
                    <li>Port number: choose that port which is using oxd-server (see in oxd-server/conf/oxd-conf.json file).</li>
                    <li>Click <code>Next</code> to continue.</li>
                </ol>
                <p>If You are successfully registered in gluu server, you will see bottom page.</p>
                <p><img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d6.png" alt="oxD_id"></p>
                <p>For making sure go to your gluu server / OpenID Connect / Clients and search for your oxD ID</p>
                <p>If you want to reset configurations click on Reset configurations button.</p>
                <h2><a id="Step_8_OpenID_Connect_Configuration_89"></a>Step 5. OpenID Connect Configuration</h2>
                <p>OpenID Connect Configuration page for Drupal-gluu-sso 2.4.3.0.</p>
                <h3><a id="Scopes_93"></a>Scopes.</h3>
                <p>You can look all scopes in your gluu server / OpenID Connect / Scopes and understand the meaning of  every scope.
                    Scopes are need for getting loged in users information from gluu server.
                    Pay attention to that, which scopes you are using that are switched on in your gluu server.</p>
                <p>In Drupal-gluu-sso 2.4.3.0 you can not only enable, disable and delete scope, but also add new scope, but when you add new scope by {any name}, necessary to add that scope in your gluu server too.
                    <img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d8.png" alt="Scopes1"></p>
                <h3><a id="Custom_scripts_104"></a>Custom scripts.</h3>
                <p><img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d9.png" alt="Customscripts"></p>
                <p>You can look all custom scripts in your gluu server / Configuration / Manage Custom Scripts / and enable login type, which type you want.
                    Custom Script represent itself the type of login, at this moment gluu server supports (U2F, Duo, Google +, Basic) types.</p>
                <h3><a id="Pay_attention_to_that_111"></a>Pay attention to that.</h3>
                <ol>
                    <li>Which custom script you enable in your Drupal site in order it must be switched on in gluu server too.</li>
                    <li>Which custom script you will be enable in OpenID Connect Configuration page, after saving that will be showed in Drupal Configuration page too.</li>
                    <li>When you create new custom script, both fields are required.</li>
                </ol>
                <h2><a id="Step_9_Drupal_Configuration_117"></a>Step 6. Drupal Configuration</h2>
                <h3><a id="Customize_Login_Icons_119"></a>Customize Login Icons</h3>
                <p>Pay attention to that, if custom scripts are not enabled, nothing will be showed.
                    Customize shape, space between icons and size of the login icons.</p>
                <p><img style="width: 100%; height: 100%;"  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d10.png" alt="DrupalConfiguration"></p>
                <h2><a id="Step_10_Show_icons_in_frontend_126"></a>Step 7. Show icons in frontend</h2>
                <p><img  src="https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d11.png" alt="frontend"></p>
',
  );

  return $form;
}
