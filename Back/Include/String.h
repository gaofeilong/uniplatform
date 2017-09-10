#ifndef _STRING_H_
#define _STRING_H_

/* config file path */
#define NETWORK_DIR             "/etc/sysconfig/network-scripts/ifcfg-"
#define MOD_CONF_PATH           "/etc/modprobe.conf"
#define TMP_MOD_CONF_PATH       "/tmp/modprobe.conf.tmp_"
#define DNS_CONF_PATH           "/etc/resolv.conf"
#define NFS_CONF_PATH           "/etc/exports"
#define DDFS_DEF_CONF_FILE      "/etc/ddfs/ddfsrc"
#define DEFAULT_FBS_CONF        "/etc/istri/conf/istri.conf"
#define DEFAULT_FBS_CONF_SH     "/etc/istri/conf/conf.sh"
#define DEFAULT_MYSQL_CONF      "/etc/scigw/default/Mysql.conf"
#define SCIGW_FBS_CONF          "/etc/scigw/default/istri.conf"
#define SCIGW_FBS_CONF_BACKUP   "/etc/scigw/default/istri.conf.bak"
#define SCIGW_CONF              "/etc/scigw/default/scigw.conf"
#define DEFAULT_ADFS_CONF       "/etc/scistor/fs/adfs/adfsrc"

#define GATEWAY_CONF            "/etc/scigw/gateway.conf"
#define TIMER_CONF              "/etc/scigw/timer.conf"
#define TIMER_PID_FILE          "/var/run/GWTimer.pid"
#define INSTALL_PACKET_PATH     "/usr/share/scigw/"
#define CL_SCIGW                (string("/usr/local/bin/scigw.sh"))


/* tmp file path */
#define TMP_FILE_PATH           "/tmp/gw_tmp_file.inf"
#define CRON_FILE               "/var/spool/cron/root"
#define TMP_ETH_CONF            "/tmp/gw_tmp_eth.conf"
#define TMP_DDFS_LIC            "/tmp/scigw_ddfs.lic"
#define TMP_ISTRI_LIC           "/tmp/scigw_xxx.xxx.xxx.xxx_istri.lic"

#define SYSTEM_TYPE_SINGLE      "single"
#define SYSTEM_TYPE_DISTRIBUTE  "distribute"
#define LOOP_BACK_IP            "127.0.0.1"

/* sql command */
#define INSERT_LINE(tn, field, value) (string("insert into ") + tn + string("(") + field + string(")values(") \
                                       + value + string(")")).c_str();
#define SELECT_LINE(tn, field)        (string("select ") + field + string(" from ") + tn).c_str()
#define SELECT_WHERE(tn, field, where) (string("select ") + field + string(" from ") + tn + string(" where ") + where).c_str()
#define DELETE_LINE(tn, where)        (string("delete from ") + tn + string(" where ") + where).c_str()
#define UPDATA_LINE(tn, value, where) (string("update ") + tn + string(" set ") + value + string(" where ") + where).c_str()
#define CL_GET_EMAIL_CONF(level)        (string("select server,send,send_passwd,verify,recv from ")+\
                                                "email_config where level like '\%"+level+"\%' and state=1")

/* shell command */
/* 系统操作SHELL命令  */
#define CL_CP(src,dest)         (string("(cp '") + src + "' '" + dest + "')").c_str()
#define CL_MV(src,dest)         (string("(mv '") + src + "' '" + dest + "')").c_str()
#define CL_DEL_PATH(path)       (string("(rm -rf ") + path + ")")

/* 网络管理相关SHELL命令  */
#define CL_UNBOND_MOD           "(modprobe -r bonding)"
#define CL_RESTART_NETWORK      "(service network restart)"
#define CL_GET_ALLNCNAME        "(ifconfig -a | egrep \"^eth|^bond\" | awk '{print $1}')"
#define CL_GET_NETWORK_IP(port) (string("ifconfig ") + port + \
                                "| egrep -o 'inet addr:([0-9]{1,3}.){3}[0-9]{1,3}' | egrep -o '([0-9]{1,3}.){3}[0-9]{1,3}'")
#define CL_GET_SERVER_IP        (string("awk -F '=' '/networkId/{print $2}' ") + SCIGW_CONF +\
                                        "|xargs ifconfig | egrep -o 'inet addr:([0-9]{1,3}.){3}[0-9]{1,3}' "+\
                                        "| egrep -o '([0-9]{1,3}.){3}[0-9]{1,3}'")

/* NFS管理相关SHELL命令  */
#define CL_NFS_START            ("service nfs start")
#define CL_NFS_STOP             ("service nfs stop")
#define CL_NFS_STATUS           ("service nfs status")
#define CL_NFS_ACTIVE_CLIENT(ip, user, pw, mp)\
                                (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" ls " + mp)
                                //(CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" cd " + mp + ";cd -")
#define CL_NFS_ONLINE_CLIENT    ("netstat 2>/dev/null| grep nfs | awk '{print $5}' | awk -F ':' '{print $1}'")
#define CL_NFS_RELOAD_CONF      ("exportfs -ar")
#define CL_NFS_REMOTE_MOUNT(ip, user, pw, serverip, src, mp)\
                                (CL_SCIGW + " --remotemount " + ip + " " + user + " " + pw + " " + serverip + " " + src + " " + mp)
#define CL_NFS_REMOTE_UMOUNT(ip, user, pw, mp)\
                                (CL_SCIGW + " --remoteumount " + ip + " " + user + " " + pw + " " + mp)
/* NFS管理相关SQL命令  */
#define SQL_SELECT_NFS_INFO     ("select * from nfsserver left join nfsclient on nfsserver.id=nfsclient.serverid")
#define SQL_SELECT_NFS(name)    (string("select * from nfsserver where name='") + name + "'")
#define SQL_INSERT_NFS_SERVER(name, path, mp, parameter) \
                                (string("insert into nfsserver(`name`, `path`, `mp`, `parameter`) values('") +\
                                        name + "','" + path + "','" + mp + "','" + parameter+ "')")
#define SQL_INSERT_NFS_CLIENT(id, ip, password) \
                                (string("insert into nfsclient values(") + id + ",'" + ip + "','" + password + "')")
#define SQL_UPDATE_NFS_SERVER(id, p)    (string("update nfsserver set parameter='") + p + "' where id=" + id)
#define SQL_DELETE_NFS_SERVER_BY_ID(id) (string("delete from nfsserver where id=") + id)
#define SQL_DELETE_NFS_CLIENT_BY_ID(id) (string("delete from nfsclient where serverid=") + id)
#define SQL_DELETE_NFS_CLIENT_BY_ID_IP(id, ip)\
                                (string("delete from nfsclient where serverid=") + id + " and ip='" + ip +"'")

/************ add by gfl 2013-4-19 **********************************/
#define IPINDEX                 0
#define MASKINDEX               1
#define MACINDEX                2
#define MTUINDEX                3
#define GWINDEX                 4
#define SPEEDINDEX              5
#define BOOTPROTOINDEX          6
#define VENDORINDEX             7
#define LINKINDEX               8
#define STATEINDEX              9

#define CL_GET_SYS              (CL_SCIGW + " --getsys")
#define CL_GET_CPU              (CL_SCIGW + " --getcpu")
#define CL_GET_MEM              (CL_SCIGW + " --getmem")
#define CL_GET_FAN              (CL_SCIGW + " --getfan")
#define CL_GET_DNS              (CL_SCIGW + " --getdns ")
#define CL_GET_HOST             (CL_SCIGW + " --gethost")
#define CL_GET_TEMP             (CL_SCIGW + " --gettemp")
#define CL_GET_DISK             (CL_SCIGW + " --getdisk")
#define CL_GET_NET_NAME         (CL_SCIGW + " --getnetname")
#define CL_GET_NET_INFO(x)      (CL_SCIGW + " --getnetinfo " + (x))
#define CL_IS_UPS_LINKED        (CL_SCIGW + " --getlinkstate")
#define CL_GET_UPS_STATE        (CL_SCIGW + " --getservice apcupsd")
#define CL_SET_UPS_STATE(s)     (CL_SCIGW + " --setservicestate apcupsd " + (s))
#define CL_GET_UPS_TIME         (CL_SCIGW + " --getupstime")
#define CL_SET_UPS_TIME(t)      (CL_SCIGW + " --setupstime " + (t))
#define CL_GET_UPS_POWER        (CL_SCIGW + " --getupspower")
#define CL_SET_UPS_POWER(p)     (CL_SCIGW + " --setupspower " + (p))

#define CL_SET_HOST(host)       (CL_SCIGW + " --sethost " + host)
#define CL_SET_DNS1(dns)        (CL_SCIGW + " --setdns " + dns)
#define CL_SET_DNS2(dns1, dns2) (CL_SCIGW + " --setdns " + dns1 + " " + dns2)
#define CL_CLEAR_DNS            (CL_SCIGW + " --cleardns")

/************ add by gfl 2013-4-24 **********************************/
#define CL_GET_TIME             (CL_SCIGW + " --gettime")
#define CL_SET_TIME(t)          (CL_SCIGW + " --settime " + (t))
#define CL_GET_TIME_ZONE        (CL_SCIGW + " --gettimezone")
#define CL_SET_TIME_ZONE(z)     (CL_SCIGW + " --settimezone " + (z))
#define CL_GET_NTP_SERVER       (CL_SCIGW + " --getntpserver")
#define CL_SET_NTP_SERVER(ip)   (CL_SCIGW + " --setntpserver " + (ip))
#define CL_SYNC_TIME(ip)        (CL_SCIGW + " --synctime " + (ip))

/************ add by gfl 2013-5-28 **********************************/
#define CL_GET_PATH_CAPACITY(p) (CL_SCIGW + " --getpathcapacity " + (p))

/************ add by gfl 2013-5-13 **********************************/
#define ARCHIVE_CONF_FILE       string("archive.conf")
#define ARCHIVE_CONF_DIR        string("/etc/scigw/archive")
#define ARCHIVE_CONF(id)        (string(ARCHIVE_CONF_DIR) + "/" + id + "/" + ARCHIVE_CONF_FILE)
#define ARCHIVE_BACKUP_SUFFIX   string(".arv.backup")
#define ARCHIVE_MAIN            string("/usr/local/bin/ArchiveMain")
#define DEFAULT_ARCHIVE_CONF    ARCHIVE_CONF("default")

/************ add by gfl 2013-7-10 **********************************/
#define FBS_INSTALL_DIR                         (string("/usr/local/share/scigw"))
#define FBS_INDEX_DAEMON                        ("/etc/init.d/istriIndex")
#define FBS_DATA_DAEMON                         ("/etc/init.d/istriData")
#define MKFS_DDFS_FILE                          ("/usr/bin/mkfs.ddfs")
#define MOUNT_DDFS_FILE                         ("/usr/bin/mount.ddfs")
#define FSCK_DDFS_FILE                          ("/usr/bin/fsck.ddfs")
#define CL_FBS_ISTRI_INDEX_START                ("service istriIndex start")
#define CL_FBS_ISTRI_DATA_START                 ("service istriData start")
#define CL_FBS_ISTRI_INDEX_STOP                 ("service istriIndex stop")
#define CL_FBS_ISTRI_DATA_STOP                  ("service istriData stop")
#define CL_FBS_ISTRI_INDEX_FIX                  ("fsck.istri.sh index")
#define CL_FBS_ISTRI_DATA_FIX                   ("fsck.istri.sh data")
#define CL_FBS_ISTRI_INDEX_STATUS               ("service istriIndex status")
#define CL_FBS_ISTRI_DATA_STATUS                ("service istriData status")
#define CL_FBS_IMPORT_LIC(ip,u,p)               (CL_SCIGW + " --importistrilicense "+ip+" "+u+" "+p)
#define CL_FBS_EXPORT_LIC(ip,u,p)               (CL_SCIGW + " --exportistrilicense "+ip+" "+u+" "+p)
#define CL_FBS_GET_LIC_INFO(ip,u,p)             (CL_SCIGW + " --getistrilicense "+ip+" "+u+" "+p)
#define CL_FBS_GET_DATA_USAGE(paths)            (CL_SCIGW + " --getdatausage "+ paths)
#define CL_FBS_GET_INDEX_USAGE(paths)           (CL_SCIGW + " --getpathcapacity "+ paths)
#define CL_FBS_GET_INDEXSERVERIP                (string("awk -F '=' '/^indexServerIp/{print $2}' ") + DEFAULT_FBS_CONF)
#define CL_FBS_GET_DATA_ID                      (string("awk -F '=' '/^id/{print $2}' ") + DEFAULT_FBS_CONF_SH)
#define CL_FBS_GET_IDXROOTPATH                  (string("awk -F '=' '/^idxRootPath/{printf $2\" \"}END{print \"\"}' ") + DEFAULT_FBS_CONF)
#define CL_FBS_GET_DATAPATH                     (string("awk -F '=' '/^dataPath/{printf $2\" \"}END{print \"\"}' ") + DEFAULT_FBS_CONF)
#define CL_FBS_GET_CONF_BACKUP                  (CL_SCIGW + " --backupconfig "+SCIGW_FBS_CONF+" "+ SCIGW_FBS_CONF_BACKUP)
#define CL_FBS_CONNECT_TEST(ip, user, pw)       (CL_SCIGW + " --testconnection " + ip + " " + user + " " + pw)
#define CL_FBS_CREATE_INSTALL_DIR(ip, user, pw) (CL_SCIGW + " --remotecommand " + ip + " " + user + " " + pw + " 'rm -rf " + \
                                                FBS_INSTALL_DIR + " /tmp/scistor-uniplatform-* && mkdir -p " + FBS_INSTALL_DIR + "'")
#define CL_FBS_COPY_INSTALL_PACKET(ip,user,pw)  (CL_SCIGW + " --copyinstallpacket " + ip + " " + user + " " + pw)
#define CL_FBS_INSTALL_GWAGENT(ip,user,pw)      (CL_SCIGW + " --installagent "+ip+" "+user+" "+pw)
#define CL_FBS_UNINSTALL_GWAGENT(ip,user,pw)    (CL_SCIGW + " --uninstallagent "+ip+" "+user+" "+pw)
#define CL_FBS_STARTUP_GWAGENT(ip,user, pw)     (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" service gwagent start")
#define CL_FBS_INSTALL_INDEX(ip,user,pw)        (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --installindex")
#define CL_FBS_UNINSTALL_INDEX(ip,user,pw)      (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --uninstallindex")
#define CL_FBS_INSTALL_DATA(ip,user,pw)         (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --installdata")
#define CL_FBS_UNINSTALL_DATA(ip,user,pw)       (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --uninstalldata")
#define CL_FBS_SET_ID(ip,user,pw,id)            (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" tool_istri_setid.sh "+id)
#define CL_FBS_SET_MYSQL_SERVER(ip,user,pw, s)  (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --setmysqlserver "+s)
#define CL_FBS_SET_ISTRI_IDXIP(ip,user,pw, s)   (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" scigw.sh --setistriserver "+s)
#define CL_FBS_CLEAR_DATA_ID(id)                (string("util.istri_index -r -c /etc/istri/conf/istri.conf -i ")+id)
#define CL_FBS_HOTPLUG_DATA_PATH(path)          (string("util.istri_data -a -c ") + DEFAULT_FBS_CONF + " -p " + path)
#define CL_FBS_HOTPLUG_INDEX_PATH(path)         (string("util.istri_index -a -c ") + DEFAULT_FBS_CONF + " -p " + path)
#define CL_FBS_CLEAR_ID(ip,user,pw,id)          (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+" "+CL_FBS_CLEAR_DATA_ID(id))
#define CL_FBS_CLEAR_DATAPATH(ip,user,pw)       (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+\
                                                        " sed -i '/^dataPath.*/d' "+DEFAULT_FBS_CONF)
#define CL_FBS_CLEAR_IDXROOTPATH(ip,user,pw)    (CL_SCIGW + " --remotecommand "+ip+" "+user+" "+pw+\
                                                        " sed -i '/^idxRootPath.*/d' "+DEFAULT_FBS_CONF)
#define CL_FBS_CLEAR_ENV(ip,user,pw)            (CL_SCIGW + " --remotecommand " + ip + " " + user + " " + pw + \
                                                        " rm -rf " + FBS_INSTALL_DIR + " /tmp/scistor-uniplatform-*")
#define CL_REMOTE_COPY(ip,user,pw,src,dest)     (CL_SCIGW + " --remotecopy "+ip+" "+user+" "+pw+" "+src+" "+dest)
#define CL_FBS_ISTRIINDEX_STATUS(ip,user,pw)    (CL_SCIGW + " --remoteindexstatus "+ip+" "+user+" "+pw)
#define CL_FBS_ISTRIDATA_STATUS(ip,user,pw)     (CL_SCIGW + " --remotedatastatus "+ip+" "+user+" "+pw)

// 清空、插入、更新链接测试表
#define SQL_TRUNCATE_FBS_CONENCTION_TEST        (string("truncate fbs_connection_test"))
#define SQL_INSERT_FBS_CONNECTION_TEST(ip,user,pw,type,id)      (string("insert into fbs_connection_test values('")+\
                                                                ip+"','"+user+"','"+pw+"','"+type+"',"+id+",'waiting')")
#define SQL_UPDATE_FBS_CONNECTION_TEST(ip, st)  (string("update fbs_connection_test set `state`='") + st + "' where ip='" + ip +"'")
// 清空、插入、更新安装表
#define SQL_TRUNCATE_FBS_INSTALL                (string("truncate fbs_install"))
#define SQL_INSERT_FBS_INSTALL(ip,user,pw,type,id)      (string("insert into fbs_install values('")+ip+"','"+\
                                                                user+"','"+pw+"','"+type+"',"+id+",0,'waiting','waiting')")
#define SQL_UPDATE_FBS_INSTALL(ip,rate,desc,st) (string("update fbs_install set `progress`=")+\
                                                        rate+",`description`='"+desc+"',`state`='"+st+"' where ip='"+ip+"'")
// 插入、更新、删除已安装的index节点表、data节点表记录
#define SQL_INSERT_FBS_INDEX_NODE(ip,u,p,st)    (string("insert into fbs_index_node(`ip`,`user`,`password`,`state`) values('")+\
                                                        ip+"','"+u+"','"+p+"','"+st+"')")
#define SQL_INSERT_FBS_DATA_NODE(id,ip,u,p,st)  (string("insert into fbs_data_node(`indexid`,`ip`,`user`,`password`,`state`)values(")+\
                                                        id+",'"+ip+"','"+u+"','"+p+"','"+st+"')")
#define SQL_DELETE_FBS_INDEX_NODE(ip)           (string("delete from fbs_index_node where ip='") + ip + "'")
#define SQL_DELETE_FBS_DATA_NODE(ip)            (string("delete from fbs_data_node where ip='") + ip + "'")
#define SQL_DELETE_FBS_DATA_NODE_FROM_ID(id)    (string("delete from fbs_data_node where id='") + id + "'")
#define SQL_UPDATE_FBS_DATA_NODE(ip,state)      (string("update fbs_data_node set `state`='") + state + "' where ip='" + ip +"'")
#define SQL_UPDATE_FBS_DATA_NODE_STATE(id,time,st)\
                                                (string("update fbs_data_node set `updatetime`='")+\
                                                time+"',`state`='"+st+"' where id="+id)
#define SQL_UPDATE_FBS_DATA_NODE_USAGE(id, capacity, used, left, total, real, dedup, time, st) \
                                                (string("update fbs_data_node set `capacity`=")+capacity+",`used`="+used+\
                                                 ",`left`="+left+",`total`="+total+",`real`="+real+",`ratio`="+dedup+\
                                                 ",`updatetime`='"+time+"',`state`='"+st+"' where id="+id)
#define SQL_UPDATE_FBS_INDEX_NODE_STATE(ip,time,st) \
                                                (string("update fbs_index_node set `updatetime`='")+\
                                                time+"',`state`='"+st+"' where ip='"+ip+"'")
#define SQL_UPDATE_FBS_INDEX_NODE_USAGE(ip,capacity1,used1,left1,time,st) \
                                                (string("update fbs_index_node set `capacity1`=") +capacity1+",`used1`="+used1+\
                                                 ",`left1`="+left1+",`updatetime`='"+time+"',`state`='"+st+"' where ip='"+ip+"'")
#define SQL_UPDATE_NODE_STATE_LOST(ip, table)   (string("update ")+table+" set `state`='lost' where ip='"+ip+"' and `state`<>'unsetid'")
// 清空、插入、更新删除表
#define SQL_TRUNCATE_FBS_REMOVE                 (string("truncate fbs_remove"))
#define SQL_INSERT_FBS_REMOVE(ip,u,pw,type,id)  (string("insert into fbs_remove values('")+\
                                                        ip+"','"+user+"','"+pw+"','"+type+"',"+id+",0,'waiting','waiting')")
#define SQL_UPDATE_FBS_REMOVE(ip,rate,desc,st)  (string("update fbs_remove set `progress`=")+\
                                                        rate+",`description`='"+desc+"',`state`='"+st+"' where ip='"+ip +"'")

// 查询指定IP的数据节点的ID
#define SQL_SELECT_INDEX_COUNT                  (string("select count(*) from fbs_index_node"))
#define SQL_SELECT_INDEX_NODE_STATE_ONLY(ip)    (string("select state from fbs_index_node where ip='") + ip +"'")
#define SQL_SELECT_DATA_NODE_STATE_ONLY(id)     (string("select state from fbs_data_node where id=") + id)
#define SQL_SELECT_INDEX_NODE_STATE             (string("select ip, updatetime, state from fbs_index_node"))
#define SQL_SELECT_DATA_NODE_STATE              (string("select ip, updatetime, state from fbs_data_node"))
#define SQL_SELECT_DATA_NODE_IP(id)             (string("select ip from fbs_data_node where id='") + id + "'")
#define SQL_SELECT_DATA_STATUS(ip)              (string("select state from fbs_data_node where ip='") + ip + "'")
#define SQL_SELECT_DATA_NODE_ID(ip)             (string("select id from fbs_data_node where ip='") + ip + "'")
#define SQL_SELECT_INDEX_NODE_ID(ip)            (string("select id from fbs_index_node where ip='") + ip + "'")
#define SQL_SELECT_INDEX_NODE_IP(id)            (string("select ip from fbs_index_node where id='") + id + "'")
#define SQL_SELECT_INDEX_NODE_INFO(ip)          (string("select password from fbs_index_node where ip='") + ip + "'")
#define SQL_SELECT_INDEX_IP_FROM_NODE_IP(ip)    (string("select ip from fbs_index_node where id=") +\
                                                        "(select indexid from fbs_data_node where ip='" + ip + "')")
#define SQL_SELECT_INDEX_IP_AND_DATA_ID(ip)     (string("select fbs_index_node.ip,fbs_data_node.id ") +\
                                                        "from fbs_index_node,fbs_data_node where " +\
                                                        "fbs_index_node.id=fbs_data_node.indexid and fbs_data_node.ip='"+ip+"'")

/********************************************************************/

// cat /sys/devices/virtual/net/bond0/bonding/slaves | sed -n 's/ /\n/p'
#define CL_BOND_INFO(bondName)  (string("(cat /sys/class/net/") + \
                                        bondName + string("/bonding/slaves |") +\
                                        string("awk '{for(i=1;i<=NF;i++) print $i}')")).c_str()

#define CL_RESTART_NETCARD(nc)  (string("(ifdown ") + nc + string("&&ifup ") + nc + ")").c_str()
#define CL_IFUP_NETCARD(nc)     (string("(ifup ") + nc + ")").c_str()
#define CL_IFDOWN_NETCARD(nc)   (string("(ifdown ") + nc + ")").c_str()

/*
#define CL_MODIFY_MOD_CNF(bn,m) (string("(echo \"alias ") + bn + string(" bonding\" >> ") + string(MOD_CONF_PATH) + ")" + \
                                        string(" && (echo \"options " + bn + " miimon=100 mode=" + m + "\" >> ") + \
                                        string(MOD_CONF_PATH) + ")").c_str()

#define CL_FILTER_MOD_CNF(bn)   (string("sed -i -r \"/^alias ") + bn + "|^options " + bn + "/d\" " + string(MOD_CONF_PATH))

#define CL_FIND_MOD(bondName)   (string("(echo `cat ") + string(MOD_CONF_PATH) + "|egrep \"^alias " + bondName \
                                        + "|^options " + bondName + "\"`)").c_str()
*/

#define CL_NETCARD_NAME         (string("kudzu -p -c network | grep \"^device:\" | awk -F [:\\\"\\ ] '{print $3}'")).c_str()
#define CL_NETCARD_VERSION(eth) (string("kudzu -p -c network | grep ") + eth + string(" -A 2 | grep \"^desc:\"") + \
                                        string("| awk -F [:\\\"] '{print $3}'")).c_str()
#define CL_NETCARD_STATUS(eth)  (string("ethtool ") + eth + string("| grep \"Link\" | awk -F [:\\ ] '{print $4}'")).c_str()

/* DDFS管理相关SHELL命令  */
#define DDFS_FILESYSTEM                 "ddfs"
#define CL_DIFF_DEVICE(p1, p2)          (string("df ") + p1 + " " + p2 + " | sed '1d' | uniq")
#define CL_GET_DDFS_CONF(dp)            (dp + string("/ddfsrc")).c_str()
#define CL_GET_DDFS_PID(dp)             (dp + string("/ddfs.pid")).c_str()
#define CL_DDFS_MKFS(conf)              (string("(ddfs_mkfs '") + conf + "')").c_str()
#define CL_DDFS_MOUNT(mp,conf)          (string("(ddfs_mount '") + mp + "' '" + conf + "')").c_str()
#define CL_DDFS_UMOUNT(mp,conf)         (string("(ddfs_umount '") + mp + "' '" + conf + "')").c_str()
#define CL_DDFS_FCSK(conf)              (string("(fsck.ddfs -c '") + conf + "')").c_str()
#define CL_DDFS_DEDUP(mp)               (string("(util.ddfs -d '") + mp + "' | awk -F [,:\\ ] '{print $3\" \"$7\" \"$12}' | " + \
                                                "awk '{for(i=1;i<=NF;++i) print $i}')").c_str()
#define CL_DDFS_IMPORT_LIC              (CL_SCIGW + " --importddfslicense ")
#define CL_DDFS_EXPORT_LIC              (CL_SCIGW + " --exportddfslicense ")
#define CL_DDFS_GET_LIC_INFO            (CL_SCIGW + " --getddfslicense")
/* DDFS管理相关SQL命令  */
#define SQL_SELECT_DDFS_MP(mp)          (string("select * from ddfsmp where mp='" + mp + "'"))

/* 其他SHELL命令 */
#define CL_GET_FIRST_DATA_PATH(conf)    (string("(cat '") + conf + "'| grep \"^dataPath1\"  | awk -F [=] '{print $2}')").c_str()
#define CL_SET_DATA_PATH(path,conf)     (string("(util.ddfs -vol '") + path + string("' '") + conf + "')").c_str();
#define CL_SET_SYSTEM_TYPE_SINGLE       (string("sed -i \"s/^systemType=.*/systemType=single/g\" ") + SCIGW_CONF)
        
/* ADFS管理相关SHELL命令  */
#define CL_GET_ADFS_MP_TOTAL(firstDataPath)     (string("adfs fs dedup -get path=") + firstDataPath + " total") 
#define CL_GET_ADFS_MP_REAL(firstDataPath)      (string("adfs fs dedup -get path=") + firstDataPath + " real")
#define CL_GET_ADFS_MP_RATIO(firstDataPath)     (string("adfs fs dedup -get path=") + firstDataPath + " factor")
#define CL_ADFS_MKFS(pathNumber)        (string("adfs_mkfs ") + DEFAULT_ADFS_CONF + " " + pathNumber)
#define CL_ADFS_MOUNT(pathNumber)       (string("adfs_mount ") + DEFAULT_ADFS_CONF + " " + pathNumber)
#define CL_ADFS_UMOUNT(pathNumber)      (string("adfs_umount ") + DEFAULT_ADFS_CONF + " " + pathNumber)
#define CL_IS_ADFS_MOUNTED(mp)          (string("mount 2>/dev/null| awk '{if($1==\"adfs\" && $3==\"" + mp + "\"){print \"mounted\"}}'"))
/* ADFS管理相关SQL命令  */
#define SQL_SELECT_ADFS_MP(first)       (string("select * from adfsmp where firstdatapath='" + first + "'"))
#define SQL_DELETE_ADFS_MP(first)       (string("delete from adfsmp where firstdatapath='" + first + "'"))
#define SQL_UPDATE_ADFS_MP(first,other) (string("update adfsmp set otherdatapath='") + other + "' where firstdatapath='" + first + "'")
#define SQL_INSERT_ADFS_MP(first,other) (string("insert into adfsmp(`firstdatapath`, `otherdatapath`) values('") +\
                                                first + "','" + other + "')")
#endif  //_STRING_H_
