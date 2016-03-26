DRUPAL GLUU SSO module 
=========================
![image](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/plugin.jpg)

DRUPAL-GLUU-SSO module gives access for login to your Drupal site, with the help of GLUU server.

There are already 2 versions of DRUPAL-GLUU-SSO (2.4.2.0 and 2.4.3.0) modules, each in its turn is working with oxD and GLUU servers.
For example if you are using DRUPAL-gluu-sso-2.4.2.0 module, you need to connect with oxD-server-2.4.2.

Now I want to explain in details how to use module step by step. 

Module will not be working if your host does not have https://. 

## Step 1. Install Gluu-server 

(version 2.4.2 or 2.4.3)

If you want to use external gluu server, You can not do this step.   

[Gluu-server installation gide](https://www.gluu.org/docs/deployment/).

## Step 2. Download oxD-server 

(version 2.4.2 or 2.4.3)

[Download oxD-server-2.4.2.Final](https://ox.gluu.org/maven/org/xdi/oxd-server/2.4.2.Final/oxd-server-2.4.2.Final-distribution.zip).

or

[Download oxD-server-2.4.3.DEMO](https://ox.gluu.org/maven/org/xdi/oxd-server/2.4.3-SNAPSHOT/oxd-server-2.4.3-SNAPSHOT-distribution.zip).

## Step 3. Unzip and run oXD-server
 
1. Unzip your oxD-server. 
2. Open the command line and navigate to the extracted folder in the conf directory.
3. Open oxd-conf.json file.  
4. If your server is using 8099 port, please change "port" number to free port, which is not used.
5. Set parameter "op_host":"Your gluu-server-url (internal or external)"
6. Open the command line and navigate to the extracted folder in the bin directory.
7. For Linux environment, run sh oxd-start.sh&. 
8. For Windows environment, run oxd-start.bat.
9. After the server starts, go to Step 4.

## Step 4. Download Drupal-gluu-sso module
 
(version 2.4.2 or 2.4.3)

[Download Drupal-gluu-sso-2.4.2.0 module](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/Drupal_gluu_sso_2.4.2.0/Drupal_gluu_sso_2.4.2.0.tar.gz).

or

[Download Drupal-gluu-sso-2.4.3.0 module](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/Drupal_gluu_sso_2.4.3.0/Drupal_gluu_sso_2.4.3.0.tar.gz).

For example if you are using gluu-server-2.4.2 it is necessary to use oxD-server-2.4.2 and Drupal-gluu-sso-2.4.2.0-module

## Step 5. Install module
 
1. Open menu tab Modules and click on ```Install new module``` button
![Manager](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d1.png) 

2. Choose downloaded module and click on ```INSTALL``` button. 
![upload](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d2.png) 

## Step 6. Activate module
 
1. Go to Modules page
2. Find module Gluu SSO {version}, choose on enabled checkbox and save configuration.
![upload](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d3.png) 
3. Go to Configuration page and open module configuration page.
![upload](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d4.png) 

## Step 7. General

![General](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d5.png)  

1. Admin Email: please add your or admin email address for registrating site in Gluu server.
2. Oxd port in your server: choose that port which is using oxd-server (see in oxd-server/conf/oxd-conf.json file).
3. Click next to continue.

If You are successfully registered in gluu server, you will see bottom page.

![oxD_id](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d6.png)

For making sure go to your gluu server / OpenID Connect / Clients and search for your oxD ID

If you want to reset configurations click on Reset configurations button.

## Step 8. OpenID Connect Configuration

OpenID Connect Configuration page for Drupal-gluu-sso 2.4.2.0 and Drupal-gluu-sso 2.4.3.0 are different.

### Scopes.
You can look all scopes in your gluu server / OpenID Connect / Scopes and understand the meaning of  every scope.
Scopes are need for getting loged in users information from gluu server.
Pay attention to that, which scopes you are using that are switched on in your gluu server.

In Drupal-gluu-sso 2.4.2.0  you can only enable, disable and delete scope.
![Scopes1](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d7.png) 

In Drupal-gluu-sso 2.4.3.0 you can not only enable, disable and delete scope, but also add new scope, but when you add new scope by {any name}, necessary to add that scope in your gluu server too. 
![Scopes2](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d8.png) 

### Custom scripts.

![Customscripts](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d9.png)  

You can look all custom scripts in your gluu server / Configuration / Manage Custom Scripts / and enable login type, which type you want.
Custom Script represent itself the type of login, at this moment gluu server supports (U2F, Duo, Google +, Basic) types.

### Pay attention to that.

1. Which custom script you enable in your Drupal site in order it must be switched on in gluu server too.
2. Which custom script you will be enable in OpenID Connect Configuration page, after saving that will be showed in Drupal Configuration page too.
3. When you create new custom script, both fields are required.

## Step 9. Drupal Configuration

### Customize Login Icons
 
Pay attention to that, if custom scripts are not enabled, nothing will be showed.
Customize shape, space between icons and size of the login icons.

![DrupalConfiguration](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d10.png)  

## Step 10. Show icons in frontend

![frontend](https://raw.githubusercontent.com/GluuFederation/gluu-sso-drupal-module/master/docu/d11.png) 
