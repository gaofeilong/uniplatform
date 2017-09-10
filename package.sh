#!/bin/bash
###############################################################################
#
# @ File Name  : package.sh
# @ Date       : 2013-5-30
# @ Author     : gaofeilong <gaofeilonggmail@163.com>
# @ Usage      : ./package，注意，打包前需要手动验证所有修改部分都已经提交
# @ Description: 统一平台安装包打包脚本，Install 安装目录中需要更新的部分：
#                bin, sql, web, scripts
#
# ############################################################################/
CWD=`pwd`

CPP_DIR=$CWD/Back
WEB_DIR=$CWD/Web
WEB_SCISTOR_DIR=$CWD/Web
WEB_PHPMYADMIN_DIR=$CWD/Web

INSTALL_DIR=$CWD/Install
INSTALL_BIN_DIR=$INSTALL_DIR/bin
INSTALL_ETC_DIR=$INSTALL_DIR/etc
INSTALL_LIB_DIR=$INSTALL_DIR/lib
INSTALL_RPM_DIR=$INSTALL_DIR/rpm
INSTALL_SQL_DIR=$INSTALL_DIR/sql
INSTALL_WEB_DIR=$INSTALL_DIR/web
INSTALL_OTHERS_DIR=$INSTALL_DIR/others
INSTALL_SCRIPTS_DIR=$INSTALL_DIR/scripts

INSTALL_PREFIX=scistor-uniplatform
SYS_REVISION=CentOS6.0-x86_64
SVN_REVISION=0

PACKAGE_FILE=Install.tar.bz2
BIN_FILES="ArchiveMain ServerMain AgentMain"

exit_error()
{
        echo "Error: $1"
        exit 1
}

cleanup_env()
{
        # 清空安装包里的旧的程序文件和脚本文件
        rm -rf $INSTALL_BIN_DIR/* 
        rm -rf $INSTALL_SQL_DIR/* 
        rm -rf $INSTALL_WEB_DIR/* 
        rm -rf $INSTALL_SCRIPTS_DIR/* 

        # 检查svn是否处于最新状态，清空页面缓存
        for cache in `svn st | grep "^?" | egrep "?\s+Web/scistor/Rbac/Runtime/"`; do
                rm -rf $cache
        done
        SVN_REVISION=`svn up | sed -n '$p' | grep -o "[0-9]\+"`
}

copy_files()
{
        # 编译C++
        cd $CPP_DIR
        if ! make clean &>/dev/null || ! make &>/dev/null; then
                exit_error "compile cpp error"
        fi
        cp -rf $BIN_FILES $INSTALL_BIN_DIR
        make clean &>/dev/null;
        cd $CWD

        # 拷贝web页面文件，数据库管理工具、脚本
        cp -rf $WEB_DIR/{phpMyAdmin,scistor} $INSTALL_WEB_DIR
        cp -rf $CPP_DIR/Utils/Script/{scigw.sh,remote.sh} $INSTALL_SCRIPTS_DIR
        cp -rf $CPP_DIR/Utils/Script/{gwserver,gwagent} $INSTALL_SCRIPTS_DIR
        cp -rf $CPP_DIR/Utils/Script/uniplatform.sql $INSTALL_SQL_DIR

}

build_install_package()
{
        mkdir tmp 
        cp -r Install tmp/
        cd tmp
        find . -name "*.svn" | xargs rm -rf
        tar cjvf $PACKAGE_FILE Install &>/dev/null
        cp $PACKAGE_FILE ../
        cd ../
        rm tmp -rf
        INSTALL_FILE_NAME=${INSTALL_PREFIX}-${SYS_REVISION}-${SVN_REVISION}.bin
        cat install.sh $PACKAGE_FILE > $INSTALL_FILE_NAME
        chmod a+x $INSTALL_FILE_NAME

        # 清空安装包中的临时文件
        rm -rf $INSTALL_BIN_DIR/* 
        rm -rf $INSTALL_SQL_DIR/* 
        rm -rf $INSTALL_WEB_DIR/* 
        rm -rf $INSTALL_SCRIPTS_DIR/* 
        rm -rf $PACKAGE_FILE
}

main()
{
        echo "build install package..."
        printf "\tclean up environment..."
        cleanup_env
        printf "ok\n"
        printf "\tcopy files..."
        copy_files
        printf "ok\n"
        printf "\ttar package..."
        build_install_package
        printf "ok\n"
        echo "done"
}

main $@
