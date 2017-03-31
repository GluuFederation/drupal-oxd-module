<?php

	/**
	 * Gluu-oxd-library
	 *
	 * An open source application library for PHP
	 *
	 *
	 * @copyright Copyright (c) 2017, Gluu Inc. (https://gluu.org/)
	 * @license	  MIT   License            : <http://opensource.org/licenses/MIT>
	 *
	 * @package	  Oxd Library by Gluu
	 * @category  Library, Api
	 * @version   3.0.0
	 *
	 * @author    Gluu Inc.          : <https://gluu.org>
	 * @link      Oxd site           : <https://oxd.gluu.org>
	 * @link      Documentation      : <https://oxd.gluu.org/docs/libraries/php/>
	 * @director  Mike Schwartz      : <mike@gluu.org>
	 * @support   Support email      : <support@gluu.org>
	 * @developer Volodya Karapetyan : <https://github.com/karapetyan88> <mr.karapetyan88@gmail.com>
	 *

	 *
	 * This content is released under the MIT License (MIT)
	 *
	 * Copyright (c) 2017, Gluu inc, USA, Austin
	 *
	 * Permission is hereby granted, free of charge, to any person obtaining a copy
	 * of this software and associated documentation files (the "Software"), to deal
	 * in the Software without restriction, including without limitation the rights
	 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	 * copies of the Software, and to permit persons to whom the Software is
	 * furnished to do so, subject to the following conditions:
	 *
	 * The above copyright notice and this permission notice shall be included in
	 * all copies or substantial portions of the Software.
	 *
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	 * THE SOFTWARE.
	 *
	 */

	/**
	 * Class socket connection
	 *
	 * Class for connection to oxd server via socket
	 *
	 * @package		  Gluu-oxd-library
	 * @subpackage	Libraries
	 * @category	  Socket connection class
	 * @see	        Oxd_RP_config
	 */


	namespace Drupal\gluu_sso\Plugin\oxds;
	use Drupal\gluu_sso\Plugin\oxds\Oxd_RP_config;
	class Client_Socket_OXD_RP{

	    /**
	     * @static
	     * @var object $socket        Socket connection
	     */
	    protected static $socket = null;
	    /**
	     * @var string $base_url      Base url for log file directory and oxd-rp-setting.json file.
	     */
	    protected  $base_url = __DIR__;

	    /**
	     * Constructor
	     *
	     * @return	void
	     */
	    public function __construct()
	    {

	        $configJSON = file_get_contents($this->base_url.'/oxd-rp-settings.json');
	        $configOBJECT = json_decode($configJSON);
	        if(!$configOBJECT->authorization_redirect_uri){
	            if(!$configJSON = file_get_contents($this->base_url.'/oxd-rp-settings-test.json')){
	                $error = error_get_last();
	                $this->log("oxd-configuration-test: ", 'Error problem with json data.');
	                $this->error_message("HTTP request failed. Error was: " . $error['message']);
	            }
	        }
	        $configOBJECT = json_decode($configJSON);
	        $this->define_variables($configOBJECT);

	        if(is_int(Oxd_RP_config::$oxd_host_port) && Oxd_RP_config::$oxd_host_port>=0 && Oxd_RP_config::$oxd_host_port<=65535){

	        }else{
	            $this->error_message(Oxd_RP_config::$oxd_host_port."is not a valid port for socket. Port must be integer and between from 0 to 65535.");
	        }
	    }

	    /**
	     * Defining oxd-setting.json file for static object Oxd_RP_config
	     *
	     * @return void
	     **/
	    public function define_variables($configOBJECT){
	        Oxd_RP_config::$op_host = $configOBJECT->op_host;
	        Oxd_RP_config::$oxd_host_port = $configOBJECT->oxd_host_port;
	        Oxd_RP_config::$authorization_redirect_uri = $configOBJECT->authorization_redirect_uri;
	        Oxd_RP_config::$post_logout_redirect_uri = $configOBJECT->post_logout_redirect_uri;
	        Oxd_RP_config::$scope = $configOBJECT->scope;
	        Oxd_RP_config::$application_type = $configOBJECT->application_type;
	        Oxd_RP_config::$response_types = $configOBJECT->response_types;
	        Oxd_RP_config::$grant_types = $configOBJECT->grant_types;
	        Oxd_RP_config::$acr_values = $configOBJECT->acr_values;

	    }
	    /**
	     * Sending request to oXD server via socket
	     *
	     * @param  string  $data
	     * @param  int  $char_count
	     * @return object
	     */
	    public function oxd_socket_request($data,$char_count = 8192){
				//echo $data;
				//die();
	        if (!self::$socket = stream_socket_client('127.0.0.1:' . Oxd_RP_config::$oxd_host_port, $errno, $errstr, STREAM_CLIENT_PERSISTENT)) {
	            $this->log("Client: socket::socket_connect is not connected, error: ",$errstr);
	            die($errno);
	        }else{
	            $this->log("Client: socket::socket_connect", "socket connected");
	        }
	        $this->log("Client: oxd_socket_request", fwrite(self::$socket, $data));
	        fwrite(self::$socket, $data);
	        $result = fread(self::$socket, $char_count);
	        if($result){
	            $this->log("Client: oxd_socket_response", $result);
	        }else{
	            $this->log("Client: oxd_socket_response", 'Error socket reading process.');
	        }
	        if(fclose(self::$socket)){
	            $this->log("Client: oxd_socket_connection", "disconnected.");
	        }
	        return $result;
	    }
	    /**
	     * Showing errors and exit.
	     *
	     * @param  string  $error
	     * @return void
	     **/
	    public function error_message($error)
	    {
	        die($error);
	    }
	    /**
	     * Saving process in log file.
	     *
	     * @param  string  $process
	     * @param  string  $message
	     * @return void
	     **/
	    public function log($process, $message){
	        $OldFile  = $this->base_url.'/logs/oxd-php-server-'.date("Y-m-d") .'.log';
	        $person = "\n".date('l jS \of F Y h:i:s A')."\n".$process.$message."\n";
	        file_put_contents($OldFile, $person, FILE_APPEND | LOCK_EX);

	    }



	}
