#ifndef __DEALCONSTANT_H__
#define __DEALCONSTANT_H__


//类型
#define BOOL bool
typedef unsigned long long      uint64;
typedef long long       int64;
typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef unsigned char   uchar;

//nas常量
#define SHARE_PATH "/mnt/share"

#define MAX_INFO_LEN    256
#define LONG_INFO_LEN   1024
#define PAGE_INFO_LEN   4096
#define MAX_SHAREPATH_LEN       64
#define NET_NAME_LEN    64

#define NOT_FIND        0xFFFFFFFF

//系统状态
#define MTU_DEFAULT     1500
#define MTU_MIN         64
#define NETDEV_NONE     0
#define NETDEV_STATIC   1
#define NETDEV_DHCP     2
#define IF_AUTO         0
#define IF_MASTER       1
#define IF_SLAVE        2

//模块名称
#define NAS_SYSINFO     "SYSINFO"
#define NAS_NETACCESS   "NETACCESS"
#define NAS_NETCONFIG   "NETCONFIG"
#define NAS_SVCNGR      "SVCNGR"
#define NAS_ALARM       "ALARM"

//配置文件路径
#define RESOLVCONF      "/etc/resolv.conf"
#define NETWORKCONF     "/etc/sysconfig/network"
#define NETDEVCONF      "/etc/sysconfig/network-scripts/ifcfg-"
#define MODCONF         "/etc/modprobe.conf"

#define NETACCESSCFG    "/etc/nas/Conf/netaccess.conf"
#define SMBCONF         "/etc/samba/smb.conf"
#define SMBCONFTEST     "/etc/nas/Conf/smb.conf"
#define NFSCONF         "/etc/exports"
#define NFSCONFTEST     "/etc/nas/Conf/exports"
#define RSYNCCONF       "/etc/nas/Conf/rsyncd.conf"
#define FTPCONF         "/etc/nas/Conf/ftp.conf"
#define WEBCONF         "/etc/nas/Conf/webdav.conf"
#define RSYNCUSERFILE   "/etc/nas/Conf/rsyncd.pwd"
#define WEBUSERFILE     "/etc/nas/Conf/webdav.user"
#define WEBGROUPFILE    "/etc/nas/Conf/webdav.group"

#define ISCSITARGETCONF     "/etc/iet/ietd.conf"
#define ISCSIHOSTSALLOW     "/etc/iet/initiators.allow"
#define ISCSIHOSTSALLOW_TMP "/etc/iet/initiators.allow_tmp"
#define ISCSITARGETTEST     "/etc/nas/Conf/ietd.conf"
#define ISCSI_TEXT          "/etc/nas/Conf/iscsi.txt"

//传输服务操作
#define SC_RUNSTAT      0
#define SC_START        1
#define SC_STOP         2
#define SC_RESTART      3
#define SC_RELOAD       4
#define SC_INITSTAT     10
#define SC_INITON       11
#define SC_INITOFF      12

#define SNC_DEF         0x40001000
#define SNC_SMB         0x40001001
#define SNC_NFS         0x40001002
#define SNC_RSYNC       0x40001003
#define SNC_FTP         0x40001004
#define SNC_WEB         0x40001005
#define SNC_ISCSITARGET 0x40001006
#define SNC_DDFS        0x40001007
#define SNC_FC          0x40001008
#define SNC_UPS         0x40001009
#define SNC_NETWORK     0x40001111

#define SN_SMB          "smb"
#define SN_NFS          "nfs"
#define SN_RSYNC        "rsyncd"
#define SN_FTP          "proftpd"
#define SN_WEB          "httpd"
#define SN_ISCSITARGET  "iscsi-target"
#define SN_NETWORK      "network"
#define SN_FC           "scst"
#define SN_UPS          "apcupsd"
#define SN_DDFS         "ddfs"

#define SERVICE_RUNNING 1
#define SERVICE_STOP    0
#define SERVICE_INITON  1
#define SERVICE_INITOFF 0
#define SVC_NUL         0xFF

//权限
#define SA_RW           2
#define SA_RO           1
#define SA_NO           0

//NFS
#define NFS_ROOTSQUASH   1
#define NFS_NOROOTSQUASH 2
#define NFS_ALLSQUASH    3
#define NFS_NOROOTSQUASHMARK    0x10

#define NFS_ANONUID     99
#define NFS_ANONGID     99

#define NFS_WDELAY      0
#define NFS_NO_WDELAY   1
#define NFS_SECURE      0
#define NFS_INSECURE    1
#define NFS_SYNC        0
#define NFS_ASYNC       1

//ISCSI-TARGET
#define ISCSI_FILEIO    0
#define ISCSI_BLOCKIO   1

#define ISCSI_WRITETHRU 0
#define ISCSI_WRITEBACK 1
#define ISCSI_READONLY  2

//ISCSI - User Type
#define ISCSI_GLOBALUSR 0
#define ISCSI_INCOMING  1
#define ISCSI_OUTGOING  2
#define MAX_TARGETIQN_LEN    200
#define MAX_IQN_LEN     223

// 共享
#define RET_SUCCESS     0
#define RET_FAILED      -1
#define DIRECTORY_MODE  0777
#define SMB_DIRMODE     02777
#define SMB_CREATEMODE  0666

#define CA_NO           "no"
#define CA_RO           "ro"
#define CA_RW           "rw"

// 信号
#define SIGUSR001       0xA001
#define SIGUSR002       0xA002

// 报警初始设置
#define ALM_PERIOD_DEF  60      // sec

#endif

