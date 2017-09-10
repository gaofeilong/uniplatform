<?php
// +----------------------------------------------------------------------
// | ThinkPHP
// +----------------------------------------------------------------------
// | Copyright (c) 2008 http://thinkphp.cn All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: liu21st <liu21st@gmail.com>
// +----------------------------------------------------------------------

// 定义ThinkPHP框架路径
define('THINK_PATH', 'ThinkPHP');
//定义项目名称和路径1
define('APP_NAME', 'Rbac');
define('APP_PATH', 'Rbac');
define('NO_CACHE_RUNTIME',True);
//define('BUILD_DIR_SECURE',True);
//define('BUILD_DIR_SECURE',True);
// 加载框架入口文件
require(THINK_PATH."/ThinkPHP.php");
//实例化一个网站应用实例
App::run();
?>