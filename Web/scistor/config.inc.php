<?php
return array(
    'URL_MODEL'             => 3,               // 如果你的环境不支持PATHINFO 请设置为3
    'APP_DEBUG'             => FALSE,
    'URL_CASE_INSENSITIVE'  => FALSE,

    'MAGIC'                 => 0x42,
    'VERSION'               => 0x31,
    'SOCKET_PORT'           => 40001,
    'SOCKET_ADDRESS'        => '127.0.0.1',     // 服务器地址
    'AGENT_ADDRESS'         => '127.0.0.1',     // 节点地址

    'DB_TYPE'               => 'mysql',         // 数据库类型
    'DB_HOST'               => '127.0.0.1',     // 服务器地址
    'DB_NAME'               => 'uniplatform',   // 数据库名
    'DB_USER'               => 'root',          // 用户名
    'DB_PWD'                => '',              // 密码
    'DB_PORT'               => 3306,            // 端口
    'DB_PREFIX'             => '',              // 表前缀
);
?>
