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
	 * Oxd RP config
	 *
	 * Class Oxd_RP_config, setting all configuration
	 *
	 * @package		  Gluu-oxd-library
	 * @subpackage	Libraries
	 * @category	  Base class for all protocols
	 */
	namespace Drupal\gluu_sso\Plugin\oxds;
	class Oxd_RP_config
{
    /**
     * @static
     * @var string $op_host        Gluu server url, which need to connect
     */
    public static $op_host;
    /**
     * @static
     * @var int $oxd_host_port        Socket connection port
     */
    public static $oxd_host_port;
    /**
     * @static
     * @var string $authorization_redirect_uri        Site authorization redirect uri
     */
    public static $authorization_redirect_uri;
    /**
     * @static
     * @var string $post_logout_redirect_uri        Site logout redirect uri
     */
    public static $post_logout_redirect_uri;
    /**
     * @static
     * @var array $scope        For getting needed scopes from gluu-server
     */
    public static $scope;
    /**
     * @static
     * @var string $application_type        web or mobile
     */
    public static $application_type;
    /**
     * @static
     * @var array $response_types        OpenID Authentication response types
     */
    public static $response_types;
    /**
     * @static
     * @var array $grant_types        OpenID Token Request type
     */
    public static $grant_types;

    /**
     * @static
     * @var array $acr_values        Gluu login acr type, can be basic, duo, u2f, gplus and etc.
     */
    public static $acr_values;
}
