#ifndef __LVM_CONF_H__
#define __LVM_CONF_H__

#define MAIN_PATH  "/etc/nas/"
#define GW_BIN_PATH "/usr/local/bin/"
#define MOUNT_PATH "/mnt/share/"
#define PERL_HEAD  "/usr/bin/perl"

#define PL_DISK_INIT   "Script/diskinit.pl"
#define PL_SINGEL_VG   "Script/listsinglevg.pl"
#define PL_LDAP_OPER   "Script/ldapoper.pl"
#define PL_QUOTA       "Script/quota.pl"
#define PL_SMBPW       "Script/smbpass.pl"
#define PL_SMB         "Script/smb.pl"
#define PL_RAID        "Script/raid.pl"
#define PL_ADPW        "Script/ad_passwd.pl"
#define PL_FC          "Script/fcoper.pl"

#define PL_DISK_ALARM  "Script/diskalarm.pl"
#define PL_QUOTA_ALARM "Script/quota.pl"

// NAS 的配置文件相关
#define CONF_NAS   "Conf/nas.conf"
#define CONF_MAIL  "Conf/mail.conf"
#define CONF_ALARM  "Conf/alarm.conf"

#define CONF_ISCSI       "Conf/iscsi.txt"
#define CONF_USERTYPE    "Conf/usertype.conf"
#define CONF_LVTYPE      "Conf/lvtype.conf"

#define CONF_FSTAB  "/etc/fstab"
#define CONF_SMB    "/etc/samba/smb.conf"
#define CONF_FCCONF "/etc/scst.conf"
#define CONF_UPS    "/etc/apcupsd/apcupsd.conf"

// DDFS 配置文件路径
#define DDFS_CONF     "/etc/nas/Ddfs/"
// DDFS 分区挂载路径 
#define DDFS_PARTPATH "/mnt/ddfs/"
// DDFS挂载点路径
#define DDFS_HANG     "/mnt/share/ddfs/"
// node 路径
#define DDFS_NODE     "/mnt/ddfs/ddfs/"
// DDFS 原始配置文件路径
#define DDFS_SRC      "/etc/ddfs/ddfsrc"
// DDFS 服务器配置所在路径
#define DDFS_SERVER   "/etc/sysconfig/ddfs"

#endif /* __LVM_CONF_H__ */
