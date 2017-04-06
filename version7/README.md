# Drupal OpenID Connect Single Sign-On (SSO) Module By Gluu

![image](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/drupal.png)

Gluu's OpenID Connect Single Sign-On (SSO) Drupal module will enable you to authenticate users against any standard OpenID Connect Provider (OP). If you don't already have an OP you can [deploy the free open source Gluu Server](https://gluu.org/docs/deployment).  

## Requirements
In order to use the Drupal module you will need to have a standard OP (like Google or a Gluu Server) and the oxd server.

* Compatibility : 7.x versions

* [Gluu Server Installation Guide](https://www.gluu.org/docs/deployment/).

* [oxd Server Installation Guide](https://oxd.gluu.org/docs/oxdserver/install/)


## Installation
 
### Download
[Drupal Module](https://github.com/GluuFederation/drupal-oxd-module/blob/master/version7/gluu_sso.tar.gz?raw=true)

1. Open menu tab Modules and click on `Install new module` button
![Manager](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/d1.png) 

2. Choose downloaded module and click on `INSTALL` button. 
![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/d2.png) 

### Activate module
 
1. Go to Modules page
2. Find module OpenID Connect Single Sign-On (SSO) Module By Gluu, choose on enabled checkbox and save configuration.
![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/5.png) 
3. Go to Configuration page and open module configuration page.
![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/6.png) 

## Configuration

### General
 
In your Drupal admin menu panel you should now see the OpenID Connect menu tab. Click the link to navigate to the General configuration  page:

![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/1.png) 

1. Automatically register any user with an account in the OpenID Provider: By setting registration to automatic, any user with an account in the OP will be able to dynamically register for an account in your Drupal site. They will be assigned the new user default role specified below.
2. Only register and allow ongoing access to users with one or more of the following roles in the OP: Using this option you can limit registration to users who have a specified role in the OP, for instance `drupal`. This is not configurable in all OP's. It is configurable if you are using a Gluu Server. [Follow the instructions below](#role-based-enrollment) to limit access based on an OP role. 
3. New User Default Role: specify which role to give to new users upon registration.  
4. URI of the OpenID Provider: insert the URI of the OpenID Connect Provider.
5. Custom URI after logout: custom URI after logout (for example "Thank you" page).
6. oxd port: enter the oxd-server port (you can find this in the `oxd-server/conf/oxd-conf.json` file).
7. Click `Register` to continue.

If your OpenID Provider supports dynamic registration, no additional steps are required in the general tab and you can navigate to the [OpenID Connect Configuration](#openid-connect-configuration) tab. 

If your OpenID Connect Provider doesn't support dynamic registration, you will need to insert your OpenID Provider `client_id` and `client_secret` on the following page.

![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/2.png) 
To generate your `client_id` and `client_secret` use the redirect uri: `https://{site-base-url}/index.php?gluuOption=oxdOpenId`.

> If you are using a Gluu server as your OpenID Provider, you can make sure everything is configured properly by logging into to your Gluu Server, navigate to the OpenID Connect > Clients page. Search for your `oxd id`.

#### Enrollment and Access Management

Navigate to your Gluu Server admin GUI. Click the `Users` tab in the left hand navigation menu. Select `Manage People`. Find the person(s) who should have access. Click their user entry. Add the `User Permission` attribute to the person and specify the same value as in the module. For instance, if in the module you have limit enrollment to user(s) with role = `drupal`, then you should also have `User Permission` = `drupal` in the user entry. Update the user record, and now they are ready for enrollment at your Drupal site. 

### OpenID Connect Configuration

![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/3.png) 

#### User Scopes

Scopes are groups of user attributes that are sent from the OP to the application during login and enrollment. By default, the requested scopes are `profile`, `email`, and `openid`.  

To view your OP's available scopes, in a web browser navigate to `https://OpenID-Provider/.well-known/openid-configuration`. For example, here are the scopes you can request if you're using [Google as your OP](https://accounts.google.com/.well-known/openid-configuration). 

If you are using a Gluu server as your OpenID Provider, you can view all available scopes by navigating to the OpenID Connect > Scopes interface. 

In the module interface you can enable, disable and delete scopes. 

> If you have chosen to limit enrollment to users with specific roles in the OP, you will also need to request the `Permission` scope, as shown in the above screenshot. 

#### Authentication

##### Bypass the local Drupal login page and send users straight to the OP for authentication

Check this box so that when users attempt to login they are sent straight to the OP, bypassing the local Drupal login screen.
When it is not checked, it will give proof the following screen.   

![upload](https://raw.githubusercontent.com/GluuFederation/drupal-oxd-module/master/version7/docu/4.png) 

##### Select acr

To signal which type of authentication should be used, an OpenID Connect client may request a specific authentication context class reference value (a.k.a. "acr"). The authentication options available will depend on which types of mechanisms the OP has been configured to support. The Gluu Server supports the following authentication mechanisms out-of-the-box: username/password (basic), Duo Security, Super Gluu, and U2F tokens, like Yubikey.  

Navigate to your OpenID Provider configuration webpage `https://OpenID-Provider/.well-known/openid-configuration` to see supported `acr_values`. In the `Select acr` section of the module page, choose the mechanism which you want for authentication. 

Note: If the `Select acr` value is `none`, users will be sent to pass the OP's default authentication mechanism.
