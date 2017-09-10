#!/bin/bash
###############################################################################
#
# @ File Name  : package.sh
# @ Date       : 2013-5-30
# @ Author     : gaofeilong <gaofeilonggmail@163.com>
# @ Usage      : ./install.bin install|uninstall
# @ Description: 统一平台安装脚本
#       1 sed -n -e '1,/^# end_of_script #$/!p' scistor-1.0.1.bin >Install.tar.bz2
#       2 sed -n -e '1,/^# end_of_script #$/p' scistor-1.0.1.bin >install.sh
#       3 cat install.sh Install.tar.bz2 >scistor-1.0.1.bin; chmod a+x scistor-1.0.1.bin
#
# ############################################################################/
INSTALL_FILE_NAME=${0##/*/}
TMP_DIR=/tmp/${INSTALL_FILE_NAME%.bin}
TMP_INSTALL_DIR=$TMP_DIR/Install
TMP_INSTALL_BIN_DIR=$TMP_INSTALL_DIR/bin
TMP_INSTALL_ETC_DIR=$TMP_INSTALL_DIR/etc/scigw
TMP_INSTALL_LIB_DIR=$TMP_INSTALL_DIR/lib
TMP_INSTALL_RPM_DIR=$TMP_INSTALL_DIR/rpm
TMP_INSTALL_SQL_DIR=$TMP_INSTALL_DIR/sql
TMP_INSTALL_WEB_DIR=$TMP_INSTALL_DIR/web
TMP_INSTALL_DDFS_DIR=$TMP_INSTALL_DIR/ddfs
TMP_INSTALL_ADFS_DIR=$TMP_INSTALL_DIR/adfs
TMP_INSTALL_SCRIPTS_DIR=$TMP_INSTALL_DIR/scripts
TMP_INSTALL_DATA_DIR=$TMP_INSTALL_DIR/istri/data
TMP_INSTALL_INDEX_DIR=$TMP_INSTALL_DIR/istri/index
TMP_INSTALL_SENSORS_DIR=$TMP_INSTALL_DIR/others/lm_sensors-3.3.3

CWD=`pwd`
DDFS_BIN_DIR=/usr/bin
ADFS_BIN_DIR=/usr/bin
SCI_ETC_DIR=/etc/scigw
SCI_WEB_DIR=/var/www/html
SCI_BIN_DIR=/usr/local/bin
SCI_LIB_DIR=/usr/local/lib64/scigw
SCI_BACKUP_DIR=/usr/local/share/scigw
SCI_WEB_SCISTOR_DIR=$SCI_WEB_DIR/scistor
SCI_WEB_PHPMYADMIN_DIR=$SCI_WEB_DIR/phpMyAdmin
SYS_SERVICE_DIR=/etc/init.d
INSTALL_TAR_PACKAGE=Install.tar.bz2

###############################################################################

print_error()
{
        printf "\033[40;31merror\033[0m\n"
}
print_ok()
{
        # printf "\033[40;32mok\033[0m\n"
        printf "ok\n"
}

decompress()
{
        if [[ -e $TMP_DIR ]]; then
                rm -rf $TMP_DIR
        fi
        mkdir -p $TMP_DIR
        printf "\tdecompress package..."
        sed -n -e '1,/^# end_of_script #$/!p' $1 > $TMP_DIR/$INSTALL_TAR_PACKAGE
        if ! tar xf $TMP_DIR/$INSTALL_TAR_PACKAGE -C $TMP_DIR; then
                print_error
                exit 1
        fi
        print_ok
}

checkagent()
{
        if [[ -e $SYS_SERVICE_DIR/gwagent ]] ||\
           [[ -e $SYS_BIN_DIR/AgentMain ]] ; then
                return 1
        fi
}

installagent()
{
        if [[ ! -e $SCI_BACKUP_DIR/$INSTALL_FILE_NAME ]]; then
                [[ ! -e $SCI_BACKUP_DIR ]] && mkdir -p $SCI_BACKUP_DIR
                cp -rf $0 $SCI_BACKUP_DIR
        fi
        decompress $1
        cd $TMP_INSTALL_DIR

        # bin
        printf "\tcopy executable files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwserver ]]; then
                cp -rf $TMP_INSTALL_BIN_DIR/ArchiveMain $SCI_BIN_DIR
        fi
        cp -rf $TMP_INSTALL_BIN_DIR/AgentMain $SCI_BIN_DIR
        print_ok

        # etc
        printf "\tcopy config files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwserver ]]; then
                if [[ -e $SCI_ETC_DIR ]]; then
                        rm -rf $SCI_ETC_DIR 
                fi
                cp -rf $TMP_INSTALL_ETC_DIR /etc/
        fi 
        print_ok

        # lib
        printf "\tcopy library files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwserver ]]; then
                if [[ -e $SCI_LIB_DIR ]]; then
                        rm -rf $SCI_LIB_DIR/*
                else
                        mkdir -p $SCI_LIB_DIR
                fi
                cp -rfl $TMP_INSTALL_LIB_DIR/* $SCI_LIB_DIR
        fi
        print_ok

        # scripts
        printf "\tcopy scripts..."
        if [[ ! -e $SYS_SERVICE_DIR/gwserver ]]; then
                cp -rf $TMP_INSTALL_SCRIPTS_DIR/{scigw.sh,remote.sh} $SCI_BIN_DIR
        fi
        cp -rf $TMP_INSTALL_SCRIPTS_DIR/gwagent $SYS_SERVICE_DIR
        print_ok

        # selinux
        printf "\tconfigure selinux..."
        SELINUX_CONF=/etc/selinux/config
        if grep -q "^SELINUX=enforcing" $SELINUX_CONF; then
                sed -i '1,$s/^SELINUX=enforcing/SELINUX=disabled/g' $SELINUX_CONF
        fi
        setenforce 0 &>/dev/null
        print_ok

        # service
        # stop networkmanager and firewalls
        /etc/init.d/NetworkManager stop &>/dev/null
        chkconfig NetworkManager off
        /etc/init.d/iptables stop &>/dev/null
        chkconfig iptables off
        /etc/init.d/ip6tables stop &>/dev/null
        chkconfig ip6tables off

        # 配置gwserver服务
        cd $CWD
        printf "\tstart gwagent..."
        if ! /etc/init.d/gwagent start &>/dev/null; then
              print_error
              return 1
        fi
        chkconfig gwagent on
        print_ok
}

uninstallagent()
{
        # 清空配置文件、库文件、页面文件、执行文件、脚本等
        printf "\tstop gwagent..."
        service gwagent stop &>/dev/null
        chkconfig gwagent off &>/dev/null
        print_ok

        printf "\tremove files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwserver ]]; then
                rm -rf $SCI_ETC_DIR
                rm -rf $SCI_BIN_DIR/{ArchiveMain,scigw.sh,remote.sh}
        fi
        rm -rf $SYS_SERVICE_DIR/gwagent
        rm -rf $SCI_BIN_DIR/AgentMain
        print_ok
}

checkserver()
{
        if [[ -e $SYS_SERVICE_DIR/gwserver ]] ||\
           [[ -e $SYS_BIN_DIR/ServerMain ]]; then
                return 1
        fi
}

installserver()
{
        if [[ ! -e $SCI_BACKUP_DIR/$INSTALL_FILE_NAME ]]; then
                [[ ! -e $SCI_BACKUP_DIR ]] && mkdir -p $SCI_BACKUP_DIR
                cp $0 $SCI_BACKUP_DIR
        fi

        decompress $1

        # bin
        printf "\tcopy executable files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwagent ]]; then
                cp -rf $TMP_INSTALL_BIN_DIR/ArchiveMain $SCI_BIN_DIR
        fi
        cp -rf $TMP_INSTALL_BIN_DIR/ServerMain $SCI_BIN_DIR
        print_ok

        # etc
        printf "\tcopy config files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwagent ]]; then
                if [[ -e $SCI_ETC_DIR ]]; then
                        rm -rf $SCI_ETC_DIR 
                fi
                cp -rf $TMP_INSTALL_ETC_DIR /etc
        fi
        print_ok

        # lib
        printf "\tcopy library files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwagent ]]; then
                if [[ -e $SCI_LIB_DIR ]]; then
                        rm -rf $SCI_LIB_DIR/*
                else
                        mkdir -p $SCI_LIB_DIR
                fi
                cp -rfl $TMP_INSTALL_LIB_DIR/* $SCI_LIB_DIR
        fi
        print_ok

        # web
        printf "\tcopy web files..."
        if [[ -e $SCI_WEB_PHPMYADMIN_DIR ]]; then
                rm -rf $SCI_WEB_PHPMYADMIN_DIR 
        fi
        if [[ -e $SCI_WEB_SCISTOR_DIR ]]; then
                rm -rf $SCI_WEB_SCISTOR_DIR
        fi
        cp -rf $TMP_INSTALL_WEB_DIR/* $SCI_WEB_DIR
        chmod -R 777 $SCI_WEB_SCISTOR_DIR
        print_ok

        # scripts
        printf "\tcopy scripts..."
        if [[ ! -e $SYS_SERVICE_DIR/gwagent ]]; then
                cp -rf $TMP_INSTALL_SCRIPTS_DIR/{scigw.sh,remote.sh} $SCI_BIN_DIR
        fi
        cp -rf $TMP_INSTALL_SCRIPTS_DIR/gwserver $SYS_SERVICE_DIR
        print_ok

        # rpm
        printf "\tinstall rpm package..."
        for file in `find $TMP_INSTALL_RPM_DIR -name "*.rpm"`; do
                rpm -ivh --nodeps $file &>/dev/null
        done
        print_ok

        # sensor
        # printf "\tinstall sensors..."
        # cd $TMP_INSTALL_SENSORS_DIR
        # if ! make install &>/dev/null; then
        #         print_error
        #         return 1
        # fi
        # cp $TMP_INSTALL_SENSORS_DIR/prog/init/lm_sensors.init $SYS_SERVICE_DIR/lm_sensors
        # print_ok
        # cd $CWD

        # ddfs
        printf "\tinstall ddfs..."
        ddfs_install_file=`ls ${TMP_INSTALL_DDFS_DIR}/ddfs-*.bin`
        if [[ -e $DDFS_BIN_DIR/ddfs_mount ]] && [[ -e $DDFS_BIN_DIR/ddfs_umount ]]; then
                printf "\tscistor ddfs already installed, cover it?[y|n] "
                read answer
                if [[ "$answer" == "y" ]]; then
                        printf "\treinstall ddfs..."
                        $ddfs_install_file install &>/dev/null<<EOF
                        y
EOF
                        if [[ $? -ne 0 ]]; then
                                print_error
                                return 1
                        fi
                        print_ok
                fi        
        else
                $ddfs_install_file install &>/dev/null
                if [[ $? -ne 0 ]]; then
                        print_error
                        return 1
                fi
                print_ok
        fi

        # adfs
        printf "\tinstall adfs..."
        adfs_install_file=`ls ${TMP_INSTALL_ADFS_DIR}/adfs-*.bin`
        if [[ -e $ADFS_BIN_DIR/adfs_mkfs ]] &&\
           [[ -e $ADFS_BIN_DIR/adfs_mount ]] &&\
           [[ -e $ADFS_BIN_DIR/adfs_umount ]]; then
                printf "\tscistor adfs already installed, cover it?[y|n] "
                read answer
                if [[ "$answer" == "y" ]]; then
                        printf "\treinstall adfs..."
                        $adfs_install_file install &>/dev/null<<EOF
                        y
EOF
                        if [[ $? -ne 0 ]]; then
                                print_error
                                return 1
                        fi
                        print_ok
                fi        
        else
                $adfs_install_file install &>/dev/null
                if [[ $? -ne 0 ]]; then
                        print_error
                        return 1
                fi
                print_ok
        fi
        sed -i "/^dataPath/d" /etc/scistor/fs/adfs/adfsrc
        /etc/init.d/adfs restart &>/dev/null
        chkconfig adfs on&>/dev/null

        # selinux
        printf "\tconfigure selinux..."
        SELINUX_CONF=/etc/selinux/config
        if grep -q "^SELINUX=enforcing" $SELINUX_CONF; then
                sed -i '1,$s/^SELINUX=enforcing/SELINUX=disabled/g' $SELINUX_CONF
        fi
        setenforce 0 &>/dev/null
        print_ok

        # service
        # stop networkmanager and firewalls
        /etc/init.d/NetworkManager stop &>/dev/null
        chkconfig NetworkManager off
        /etc/init.d/iptables stop &>/dev/null
        chkconfig iptables off
        /etc/init.d/ip6tables stop &>/dev/null
        chkconfig ip6tables off

        # 不能有缩进
#         sensors-detect &>/dev/null<<-EOF
# yes 
# yes 
# yes 
# yes 
# yes 
# EOF
# 
#         printf "\trestart lmsensors..."
#         if ! /etc/init.d/lm_sensors restart &>/dev/null; then
#                 print_error
#         else 
#                 print_ok
#         fi
#         chkconfig lm_sensors on

        printf "\trestart httpd..."
        if ! /etc/init.d/httpd restart &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        chkconfig httpd on

        printf "\trestart mysqld..."
        if ! /etc/init.d/mysqld restart &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        chkconfig mysqld on

        # sql 只有当mysql服务起来了这里才能够运行
        printf "\timport database..."
        mysql < $TMP_INSTALL_SQL_DIR/uniplatform.sql
        print_ok

        # 配置gwserver服务
        printf "\tstart gwserver..."
        if ! /etc/init.d/gwserver start &>/dev/null; then
              print_error
              return 1
        fi
        chkconfig gwserver on
        print_ok

        echo "installagent process..."
        installagent $1

        rm -rf $TMP_DIR
}

remove_sensors()
{
        rm -f /usr/local/lib/libsensors.so.4.3.2 
        rm -f /usr/local/lib/libsensors.so.4 
        rm -f /usr/local/lib/libsensors.so
        rm -f /usr/local/lib/libsensors.a
        rm -f /usr/local/include/sensors/error.h 
        rm -f /usr/local/include/sensors/sensors.h 
        rm -f /usr/local/man/man3/libsensors.3 
        rm -f /usr/local/man/man5/sensors.conf.5
        rm -f /usr/local/sbin/sensors-detect
        rm -f /usr/local/man/man8/sensors-detect.8
        rm -f /usr/local/sbin/fancontrol 
        rm -f /usr/local/sbin/pwmconfig
        rm -f /usr/local/man/man8/fancontrol.8 
        rm -f /usr/local/man/man8/pwmconfig.8
        rm -f /usr/local/bin/sensors
        rm -f /usr/local/man/man1/sensors.1
        rm -f /usr/local/bin/sensors-conf-convert
        rm -f /usr/local/sbin/isadump 
        rm -f /usr/local/sbin/isaset
        rm -f /usr/local/man/man8/isadump.8 
        rm -f /usr/local/man/man8/isaset.8
        rm -f /etc/init.d/lm_sensors
        rmdir /usr/local/include/sensors 2> /dev/null || true
}

uninstallserver()
{
        # 清空配置文件、库文件、页面文件、执行文件、脚本等
        printf "\tstop gwserver..."
        service gwserver stop &>/dev/null
        chkconfig gwserver off &>/dev/null
        print_ok

        printf "\tremove files..."
        if [[ ! -e $SYS_SERVICE_DIR/gwagent ]]; then
                rm -rf $SCI_ETC_DIR
                rm -rf $SCI_BIN_DIR/{ArchiveMain,scigw.sh,remote.sh}
        fi
        rm -rf $SCI_WEB_SCISTOR_DIR
        rm -rf $SCI_WEB_PHPMYADMIN_DIR
        rm -rf $SYS_SERVICE_DIR/gwserver
        rm -rf $SCI_BIN_DIR/ServerMain
        # rm -rf /usr/local/lib64/scigw  # 不删除库文件
        print_ok

        # 清空数据库
        printf "\tremove database..."
        mysql -e "drop database if exists uniplatform" &>/dev/null
        print_ok

        # 卸载用到的 rpm 包
        printf "\tremove rpm package..."
        rpm -e apcupsd-3.14.10 &>/dev/null
        rpm -e kudzu-1.2.57.1.24-1.el5.centos.x86_64 &>/dev/null
        rpm -e php-ldap-5.3.2-6.el6.x86_64 &>/dev/null
        rpm -e php-mysql-5.3.2-6.el6.x86_64 &>/dev/null
        print_ok

        # 停止相关的服务
        printf "\tstop service..."
        service mysqld stop &>/dev/null
        service httpd stop &>/dev/null
        # service lm_sensors stop &>/dev/null
        # chkconfig lm_sensors off &>/dev/null
        print_ok

        # 卸载 sensors
        # printf "\tremove sensors..."
        # remove_sensors &>/dev/null
        # print_ok

        echo "uninstallagent process..."
        uninstallagent
}


checkindex()
{
        if [[ -e /etc/rc.d/init.d/istriIndex ]]; then
                return 1
        fi
}

installindex()
{
        decompress $1
        cd $TMP_INSTALL_INDEX_DIR
        printf "\tinstall index..."
        if ! ./install_istri_index.sh install &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        printf "\tinstall rpm package..."
        for file in `ls $TMP_INSTALL_RPM_DIR/{tcl*.rpm,expect*.rpm}`; do
                rpm -ivh --nodeps $file &>/dev/null
        done
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

uninstallindex()
{
        decompress $1
        cd $TMP_INSTALL_INDEX_DIR
        printf "\tuninstall index..."
        if ! ./install_istri_index.sh uninstall &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

checkdata()
{
        if [[ -e /etc/rc.d/init.d/istriData ]]; then
                return 1
        fi
}

installdata()
{
        decompress $1
        cd $TMP_INSTALL_DATA_DIR
        printf "\tinstall data..."
        if ! ./install_istri_data.sh install &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

uninstalldata()
{
        decompress $1
        cd $TMP_INSTALL_DATA_DIR
        printf "\tuninstall data..."
        if ! ./install_istri_data.sh uninstall &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

checkddfs()
{
        if [[ -e /usr/bin/ddfs_mkfs ]]; then
                return 1
        fi
}

uninstallddfs()
{
        decompress $1
        cd $TMP_INSTALL_DDFS_DIR
        printf "\tuninstall ddfs..."
        DDFS_INSTALL_FILE=`ls ddfs*.bin`
        if ! ./$DDFS_INSTALL_FILE uninstall &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

checkadfs()
{
        if [[ -e /usr/bin/adfs_mkfs ]]; then
                return 1
        fi
}

uninstalladfs()
{
        decompress $1
        cd $TMP_INSTALL_ADFS_DIR
        printf "\tuninstall adfs..."
        ADFS_INSTALL_FILE=`ls adfs*.bin`
        if ! ./$ADFS_INSTALL_FILE uninstall &>/dev/null; then
                print_error
                return 1
        fi
        print_ok
        cd $CWD
        rm -rf $TMP_DIR
}

main()
{
        if [[ "$1" == "installserver" ]] || [[ "$1" == "installagent" ]] ||\
           [[ "$1" == "installindex" ]]  || [[ "$1" == "installdata" ]]; then
                suffix=${1:7}   # suffix=server or agent or index or data
                check$suffix
                if [[ $? -ne 0 ]]; then
                        printf "\t$suffix already installed, please uninstall and try again\n"
                        exit 1
                else
                        if [[ $suffix == "server" ]]; then
                                checkindex
                                ret1=$?
                                checkdata
                                ret2=$?
                                if [[ $ret1 -ne 0 ]] || [[ $ret2 -ne 0 ]]; then
                                        printf "\tinstall error: istri already installed, please uninstall istri and try again\n"
                                        exit 1
                                fi
                        fi
                        echo "$1 process..."
                        $1 $0
                        if [[ $? -ne 0 ]]; then
                                echo "install $suffix error, clean up environment..."
                                un$1
                        fi
                        echo "done"
                fi
        elif [[ "$1" == "uninstallserver" ]] || [[ "$1" == "uninstallagent" ]] ||\
             [[ "$1" == "uninstallindex" ]]  || [[ "$1" == "uninstalldata" ]]  ||\
             [[ "$1" == "uninstallddfs" ]]   || [[ "$1" == "uninstalladfs" ]]; then
                suffix=${1:9}
                check$suffix
                if [[ $? -eq 0 ]]; then
                        printf "\t$suffix already uninstalled\n"
                else
                        echo "unintall $suffix process..."
                        $1 $0
                        echo "done"
                fi
        else
                 echo "  $0 installserver|uninstallserver"
        fi
}

main $@
exit 0
# end_of_script #
