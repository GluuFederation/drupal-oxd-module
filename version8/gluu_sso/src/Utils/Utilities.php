<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace Drupal\gluu_sso\Utils;

use Drupal\gluu_sso\Plugin\oxds\Get_client_access_token;


/**
 * Description of Utilities
 *
 * @author sampad
 */
class Utilities {
    public function get_protection_access_token($config){
        $gluu_config = $config->get('gluu_config');
        $getProtectionAccessToken = new Get_client_access_token();
        $getProtectionAccessToken->setRequest_client_id($gluu_config['gluu_client_id']);
        $getProtectionAccessToken->setRequest_client_secret($gluu_config['gluu_client_secret']);
        $getProtectionAccessToken->setRequestOpHost($gluu_config['op_host']);
        $getProtectionAccessToken->setRequest_oxd_id($config->get('gluu_oxd_id'));
        if($gluu_config['connection_type'] == 2){
            $getProtectionAccessToken->request(rtrim($gluu_config['oxd_web_host'],'/').'/get-client-token');
        }else{
            $getProtectionAccessToken->request();
        }
        return $getProtectionAccessToken->getResponse_access_token();
    }
}
