<?php
// 0x X    X    X    X    X    X    X    X
// 0b 0000 0000 0000 0000 0000 0000 0000 0000
//    ---- ---- --------- -------------------
//    保留 控制 模块      功能

// 保留 [31:28] 保留位(0x0)
// 控制 [27:24] 控制码，本地请求(0x01)还是远端节点请求(0x00)
// 模块 [23:16] 标识请求属于哪个模块，最多256个模块(0x00-0xFF)
// 功能 [15:0 ] 标识请求属于模块内的哪个功能，最多64K个功能(0x0000-0xFFFF)
//      正常功能位用前三位，最后一位预留，用以功能扩充

define('CMD_OK',                           0x00000000); 
define('CMD_ERR',                          0x00000001);
define('CMD_UNKNOWN',                      0x000000FF);

define('REMOTE_REQUEST',                   0x00000000);
define('LOCAL_REQUEST',                    0x01000000);
define('TYPE_MASK',                        0x0F000000);

define('CMD_SYS',                          0x00010000); 
define('CMD_NET',                          0x00020000); 
define('CMD_DISK',                         0x00030000); 
define('CMD_DDFS',                         0x00040000); 
define('CMD_ARCHIVE',                      0x00050000); 
define('CMD_NAS',                          0x00060000); 
define('CMD_VTL',                          0x00070000); 
define('MODULE_MASK',                      0x00FF0000);

define('CMD_SYS_INFO_LOOKUP',              0x00010010);
define('CMD_SYS_TIME_LOOKUP',              0x00010020); 
define('CMD_SYS_TIME_SET',                 0x00010030);
define('CMD_SYS_WARNING_LOOKUP',           0x00010040);
define('CMD_SYS_WARNING_SET',              0x00010050);
define('CMD_SYS_PASSWORD_SET',             0x00010060);
define('CMD_SYS_UPS_LOOKUP',               0x00010070);
define('CMD_SYS_UPS_SET',                  0x00010080);
define('CMD_SYS_LICENSE_LOOKUP',           0x02080070);
define('CMD_SYS_LICENSE_EXPORT',           0x02080080);
define('CMD_SYS_LICENSE_IMPORT',           0x02080090);
define('CMD_SYS_UPGRADE_LOOKUP',           0x000100C0);
define('CMD_SYS_UPGRADE_IMPORT',           0x000100D0);
define('CMD_SYS_LOG_LOOKUP',               0x000100E0);
define('CMD_SYS_LOG_EXPORT',               0x000100F0);
define('CMD_SYS_EMAIL_TEST',               0x00010100);

define('CMD_NET_BASE_LOOKUP',              0x00020010);
define('CMD_NET_BASE_SET',                 0x00020020);
define('CMD_NET_CARD_LOOKUP',              0x00020030);
define('CMD_NET_CARD_SET',                 0x00020040);
define('CMD_NET_BOND_SET',                 0x00020050);
define('CMD_NET_BOND_CREATE',              0x00020060);
define('CMD_NET_BOND_REMOVE',              0x00020070);
define('CMD_NET_FC_LOOKUP',                0x00020080);
define('CMD_NET_FC_SET',                   0x00020090);
define('CMD_NET_ISCSI_LOOKUP',             0x000200A0);
define('CMD_NET_ISCSI_SET',                0x000200B0);

define('CMD_STORAGE_DISK_LOOKUP',          0x00030010);
define('CMD_STORAGE_VG_LOOKUP',            0x00031010);
define('CMD_STORAGE_VG_CREATE',            0x00030030);
define('CMD_STORAGE_VG_REMOVE',            0x00030040);
define('CMD_STORAGE_VG_EXTEND',            0x00030050);
define('CMD_STORAGE_LV_LOOKUP',            0x00032000);
define('CMD_STORAGE_LV_CREATE',            0x00032010);
define('CMD_STORAGE_LV_REMOVE',            0x00032020);
define('CMD_STORAGE_LV_EXTEND',            0x00032030);

define('CMD_MP_LOOKUP',                    0x00040010);
define('CMD_MP_DETAIL',                    0x00040020);
define('CMD_MP_CREATE',                    0x00040030);
define('CMD_MP_REMOVE',                    0x00040040);
define('CMD_MP_SET',                       0x00040050);
define('CMD_MP_MOUNT',                     0x00040060);
define('CMD_MP_UMOUNT',                    0x00040070);
define('CMD_MP_FIX',                       0x00040080);
define('CMD_MP_LIST',                      0x00040090);

define('CMD_ARCHIVE_LOOKUP',               0x00050010);
define('CMD_ARCHIVE_DETAIL',               0x00050020);
define('CMD_ARCHIVE_CREATE',               0x00050030);
define('CMD_ARCHIVE_REMOVE',               0x00050040);
define('CMD_ARCHIVE_START',                0x00050050);
define('CMD_ARCHIVE_STOP',                 0x00050060);
define('CMD_ARCHIVE_SERVICE_START',        0x00050070);
define('CMD_ARCHIVE_SERVICE_STOP',         0x00050080);
define('CMD_ARCHIVE_SET',                  0x00050090);

define('CMD_NAS_ISCSI_TARGET_LOOKUP'      ,0x00061010);
define('CMD_NAS_ISCSI_TARGET_ADD'         ,0x00061020);
define('CMD_NAS_ISCSI_TARGET_DEL'         ,0x00061030);
define('CMD_NAS_ISCSI_MAP'                ,0x00061050);
define('CMD_NAS_ISCSI_UNMAP'              ,0x00061060);
define('CMD_NAS_ISCSI_SHOWMAP'            ,0x00061070);
define('CMD_NAS_ISCSI_SHOWUNMAP'          ,0x00061080);
define('CMD_NAS_ISCSI_CHAP_ADD'           ,0x00061090);
define('CMD_NAS_ISCSI_CHAP_DEL'           ,0x000610A0);
define('CMD_NAS_ISCSI_CHAP_GET'           ,0x000610B0);
define('CMD_NAS_ISCSI_GETALLOW'           ,0x000610D0);
define('CMD_NAS_ISCSI_SETALLOW'           ,0x000610E0);

define('CMD_NAS_FC_LOOKUP'                ,0x00062010);
define('CMD_NAS_FC_MAP'                   ,0x00062020);
define('CMD_NAS_FC_UNMAP'                 ,0x00062030);

define('CMD_NAS_AUTH_GET'                 ,0x00063010);
define('CMD_NAS_AUTH_SET'                 ,0x00063020);

define('CMD_NAS_AUTH_GROUP_GET'           ,0x00063110);
define('CMD_NAS_AUTH_GROUP_ADD'           ,0x00063120);
define('CMD_NAS_AUTH_GROUP_DEL'           ,0x00063130);
define('CMD_NAS_AUTH_GROUP_SET'           ,0x00063140);

define('CMD_NAS_AUTH_USER_GET'            ,0x00063210);
define('CMD_NAS_AUTH_USER_ADD'            ,0x00063220);
define('CMD_NAS_AUTH_USER_DEL'            ,0x00063230);
define('CMD_NAS_AUTH_USER_SET'            ,0x00063240);

define('CMD_NAS_QUOTA_GROUP_GET'          ,0x00063310);
define('CMD_NAS_QUOTA_GROUP_SET'          ,0x00063320);
define('CMD_NAS_QUOTA_USER_GET'           ,0x00063330);
define('CMD_NAS_QUOTA_USER_SET'           ,0x00063340);

define('CMD_NAS_SHARE_LIST'               ,0x00065010);
define('CMD_NAS_SHARE_ADD'                ,0x00065020);
define('CMD_NAS_SHARE_DEL'                ,0x00065030);
define('CMD_NAS_SHARE_GET'                ,0x00065040);
define('CMD_NAS_SHARE_SET'                ,0x00065050);

define('CMD_NAS_SERVICE_LIST'             ,0x00064010);
define('CMD_NAS_SERVICE_CTRL'             ,0x00064020);

$OPERATION = array (
    CMD_SYS                             => "SYSTEM",
    CMD_NET                             => "NETWORK", 
    CMD_DISK                            => "STORAGE",
    CMD_DDFS                            => "DDFS",
    CMD_ARCHIVE                         => "ARCHIVE",
    CMD_NAS                             => "NAS",
    CMD_VTL                             => "VTL",

    CMD_SYS_INFO_LOOKUP                 => "查看系统信息",
    CMD_SYS_TIME_LOOKUP                 => "查看系统时间",
    CMD_SYS_TIME_SET                    => "配置系统时间",
    CMD_SYS_WARNING_LOOKUP              => "查看报警邮件",
    CMD_SYS_WARNING_SET                 => "配置报警邮件",
    CMD_SYS_PASSWORD_SET                => "配置密码",
    CMD_SYS_UPS_LOOKUP                  => "查看UPS配置",
    CMD_SYS_UPS_SET                     => "配置UPS",
    CMD_SYS_LICENSE_LOOKUP              => "查看LICENSE",
    CMD_SYS_LICENSE_IMPORT              => "导入LICENSE",
    CMD_SYS_LICENSE_EXPORT              => "导出LICENSE",
    CMD_SYS_UPGRADE_LOOKUP              => "查看系统更新",
    CMD_SYS_UPGRADE_IMPORT              => "导入更新包",
    CMD_SYS_LOG_LOOKUP                  => "查看系统日志",
    CMD_SYS_LOG_EXPORT                  => "导出日志",

    CMD_NET_BASE_LOOKUP                 => "查看基础网络配置",
    CMD_NET_BASE_SET                    => "配置基础网络信息",
    CMD_NET_CARD_LOOKUP                 => "查看网卡配置",
    CMD_NET_CARD_SET                    => "配置普通网卡",
    CMD_NET_BOND_SET                    => "配置绑定网卡",
    CMD_NET_BOND_CREATE                 => "绑定网卡",
    CMD_NET_BOND_REMOVE                 => "解除网卡绑定",
    CMD_NET_FC_LOOKUP                   => "查看FC服务配置",
    CMD_NET_FC_SET                      => "配置FC服务",
    CMD_NET_ISCSI_LOOKUP                => "差可能ISCSI服务配置",
    CMD_NET_ISCSI_SET                   => "配置ISCSI服务",

    CMD_STORAGE_DISK_LOOKUP             => "查看储存信息",
    CMD_STORAGE_VG_LOOKUP               => "查看卷组信息",
    CMD_STORAGE_VG_CREATE               => "创建卷组",
    CMD_STORAGE_VG_REMOVE               => "删除卷组",
    CMD_STORAGE_VG_EXTEND               => "扩容卷组",
    CMD_STORAGE_LV_LOOKUP               => "查看逻辑卷",
    CMD_STORAGE_LV_CREATE               => "创建逻辑卷",
    CMD_STORAGE_LV_REMOVE               => "删除逻辑卷",
    CMD_STORAGE_LV_EXTEND               => "扩容逻辑卷",

    CMD_MP_LOOKUP                       => "查看消冗挂载点列表",
    CMD_MP_DETAIL                       => "查看消冗挂载点详细信息",
    CMD_MP_CREATE                       => "创建消冗挂载点",
    CMD_MP_REMOVE                       => "删除消冗挂载点",
    CMD_MP_SET                          => "编辑消冗挂载点",
    CMD_MP_MOUNT                        => "挂载消冗挂载点",
    CMD_MP_UMOUNT                       => "卸载消冗挂载点",
    CMD_MP_FIX                          => "修复消冗挂载点",
    CMD_MP_LIST                         => "查看消冗挂载点列表",

    CMD_ARCHIVE_LOOKUP                  => "查看归档任务列表",
    CMD_ARCHIVE_DETAIL                  => "查看归档任务详细信息",
    CMD_ARCHIVE_CREATE                  => "创建归档任务",
    CMD_ARCHIVE_REMOVE                  => "删除归档任务",
    CMD_ARCHIVE_START                   => "启动归档任务",
    CMD_ARCHIVE_STOP                    => "停止归档任务",
    CMD_ARCHIVE_SERVICE_START           => "启动归档服务",
    CMD_ARCHIVE_SERVICE_STOP            => "停止归档服务",
    CMD_ARCHIVE_SET                     => "编辑归档任务",

    CMD_NAS_SHARE_ISCSI_LOOKUP          => "",
    CMD_NAS_SHARE_ISCSI_TARGET_CREATE   => "",
    CMD_NAS_SHARE_ISCSI_TARGET_REMOVE   => "",
    CMD_NAS_SHARE_ISCSI_USER_CREATE     => "",
    CMD_NAS_SHARE_ISCSI_USER_REMOVE     => "",
    CMD_NAS_SHARE_ISCSI_USER_SET        => "",
    CMD_NAS_SHARE_ISCSI_VOL_MAP         => "",
    CMD_NAS_SHARE_ISCSI_VOL_UNMAP       => "",

    CMD_NAS_SHARE_FC_LOOKUP             => "",
    CMD_NAS_SHARE_FC_VOL_MAP            => "",
    CMD_NAS_SHARE_FC_VOL_UNMAP          => "",

    CMD_NAS_SHARE_NAS_LOOKUP            => "",
    CMD_NAS_SHARE_NAS_DETAIL            => "",
    CMD_NAS_SHARE_NAS_CREATE            => "",
    CMD_NAS_SHARE_NAS_REMOVE            => "",
    CMD_NAS_SHARE_NAS_UPDATE            => "",
    CMD_NAS_SHARE_NAS_SET               => "",

    CMD_NAS_USER_AUTH_LOOKUP            => "",
    CMD_NAS_USER_AUTH_SET               => "",

    CMD_NAS_USER_GROUP_LOOKUP           => "",
    CMD_NAS_USER_GROUP_CREATE           => "",
    CMD_NAS_USER_GROUP_REMOVE           => "",
    CMD_NAS_USER_GROUP_SET              => "",

    CMD_NAS_USER_USER_LOOKUP            => "",
    CMD_NAS_USER_USER_CREATE            => "",
    CMD_NAS_USER_USER_REMOVE            => "",
    CMD_NAS_USER_USER_SET               => "",

    CMD_NAS_SERVICE_LOOKUP              => "",
    CMD_NAS_SERVICE_START               => "",
    CMD_NAS_SERVICE_STOP                => "",
    CMD_NAS_SERVICE_OFFBOOT             => "",
    CMD_NAS_SERVICE_ONBOOT              => "",
);
?>
