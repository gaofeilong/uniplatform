<?php
if (!defined('THINK_PATH')) exit();
$config = include 'config.inc.php';
$array  = array(
    'USER_AUTH_ON'          => true,
    'USER_AUTH_TYPE'        => 1,                   // 默认认证类型 1 登录认证 2 实时认证
    'USER_AUTH_KEY'         => 'authId',            // 用户认证SESSION标记
    'ADMIN_AUTH_KEY'        => 'administrator',
    'USER_AUTH_MODEL'       => 'User',              // 默认验证数据表模型
    'AUTH_PWD_ENCODER'      => 'md5',               // 用户认证密码加密方式

    'USER_AUTH_GATEWAY'     => '/Public/login',     // 默认认证网关
    'DEFAULT_MODULE'        => 'Public',            // 默认模块名称
    'DEFAULT_ACTION'        => 'login',             // 默认操作名称     
    'NOT_AUTH_MODULE'       => 'Public',            // 默认无需认证模块

    'REQUIRE_AUTH_MODULE'   => '',                  // 默认需要认证模块
    'NOT_AUTH_ACTION'       => '',                  // 默认无需认证操作
    'REQUIRE_AUTH_ACTION'   => '',                  // 默认需要认证操作
    'GUEST_AUTH_ON'         => false,               // 是否开启游客授权访问
    'GUEST_AUTH_ID'         => 0,                   // 游客的用户ID
    'SHOW_RUN_TIME'         => false,               // 运行时间显示
    'SHOW_ADV_TIME'         => false,               // 显示详细的运行时间
    'SHOW_DB_TIMES'         => false,               // 显示数据库查询和写入次数
    'SHOW_CACHE_TIMES'      => false,               // 显示缓存操作次数
    'SHOW_USE_MEM'          => false,               // 显示内存开销
    'DB_LIKE_FIELDS'        => 'title|remark',
    'RBAC_ROLE_TABLE'       => 'sci_role',
    'RBAC_USER_TABLE'       => 'sci_role_user',
    'RBAC_ACCESS_TABLE'     => 'sci_access',
    'RBAC_NODE_TABLE'       => 'sci_node',
    'RBAC_USER_TABLE'       => 'sci_user',
);
return array_merge($config,$array);
?>
