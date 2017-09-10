#!/bin/bash
###############################################################################
#
# @ File Name  : scigw.sh
# @ Date       : 2013-04-22
# @ Author     : gaofeilong <gaofeilonglcu@gmail.com>
# @ Description: 网关系统辅助脚本，获取主机状态和设置参数
#                必须的组件: sed, awk, uname, kudzu, hdparm, ethtool
# @ History    : 2013-04-22：创建
#                2013-04-23：getnetname、getnetinfo和服务
#                2013-06-26：添加 remotecopy remotecommand testconnection 
#
##############################################################################/

dnsFile=/etc/resolv.conf
hostFile=/etc/hosts
networkFile=/etc/sysconfig/network
tmpFile=/var/tmp/scigw_tmp.txt
scigwConf=/etc/scigw/default/scigw.conf
ntpConf=/etc/ntp.conf
upsConf=/etc/apcupsd/apcupsd.conf
installDir=/usr/local/share/scigw
mysqlConf=/etc/scigw/default/Mysql.conf
istriConf=/etc/istri/conf/istri.conf
remote=/usr/local/bin/remote.sh
#apacheUploadPath=/root/trunk/Web/scistor/Public/Uploads
apacheUploadPath=/var/www/html/scistor/Public/Uploads
#remote=./remote.sh

# $1: utils path
check()
{
        if [[ ! -e $1 ]]; then
                echo "-bash: $1: No such file or directory"
                exit 127
        fi
}

# no arguments and output like this:
#       CentOS Linux release 6.0 (Final)
#       2.6.32-71.el6.x86_64
#       1.0.1
getsys()
{
        cat /etc/redhat-release && \
        uname -r && \
        awk -F [\ =] '/^version/{print $NF}' $scigwConf
}

# no arguments and output like this:
# 0                                     # processor 0
# Intel(R) Xeon(R) CPU X3430 @ 2.40GHz  # vendor
# 1200.000                              # cpu MHz 
# 8388608                               # cache size 
# 1                                     # processor 1
# Intel(R) Xeon(R) CPU X3430 @ 2.40GHz  # vendor
# 1200.000                              # cpu MHz    
# 8388608                               # cache size 
# ...
getcpu()
{
        awk -F ": " '/^processor|^cpu MHz|^cache size|^model name/ { \
                print $2 \
        }' /proc/cpuinfo | awk '{ \
                if ($NF == "KB") { \
                        print $1 * 1024 \
                } else { \
                        for (i = 1; i<= NF; i++) { \
                                printf $i " " \
                        } \
                print "" \
                } \
        }'
}

# no arguments and output like this:
#       8166544 kB      # MemTotal
getmem()
{
        sizeK=`awk '/^MemTotal/{print $2}' /proc/meminfo`
        sizeB=$[$sizeK * 1024]
        echo "$sizeB"
}

# no arguments and output like this:
#       eth0
#       bond0
#       ...
getnetname()
{
        # all net interface
        # awk -F":" '/:/{print $1}' /proc/net/dev

        # only eth*, bond*, br*
        awk -F":" '/:/{print $1}' /proc/net/dev \
                | awk '{print $NF}' | awk '/^eth|^br|^bond/{print $1}' | sort
}

# $1: net device name
# output:
#       192.168.0.88                    # ip:     [192.168.0.88]
#       255.255.254.0                   # mask:   [255.255.254.0]
#       00:25:90:0D:45:DE               # mac:    [00:25:90:0D:45:DE]
#       1500                            # mtu:    [1500]
#       192.168.0.1                     # gw:     [192.168.0.1]
#       none                            # mode:   [none]
#       on                              # state:  [on]
#       device                          # id:     [subbond]
#       Intel Network Connection        # vendor: [Intel Network Connection]
getnetinfo()
{
        check /sbin/kudzu

        ip=`ifconfig $1   | egrep -o "inet addr:([0-9]{1,3}.){3}[0-9]{1,3}" \
                          | awk -F":" '{print $2}'`
        mask=`ifconfig $1 | egrep -o "Mask:([0-9]{1,3}.){3}[0-9]{1,3}" \
                          | awk -F":" '{print $2}'`
        mac=`ifconfig $1  | egrep -o "HWaddr +([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}" \
                          | awk '{print $2}'`
        mtu=`ifconfig $1  | egrep -o "MTU:[0-9]+" | awk -F":" '{print $2}'`
        # gateway & bootproto 
        netConf="/etc/sysconfig/network-scripts/ifcfg-$1"
        if [[ -e $netConf ]]; then
                if egrep -q "GATEWAY|gateway" $netConf; then
                        gw=`awk -F"=" '/GATEWAY/{print $2}' $netConf`
                else
                        gw=""
                fi
                if egrep -q "BOOTPROTO|bootproto" $netConf; then
                        mode=`awk -F"=" '/BOOTPROTO/{print $2}' $netConf`
                else
                        mode=""
                fi
        else
                gw=""
                mode=""
        fi
        # vendor
        if kudzu -p -c network | egrep -q "^device: $1" && \
           kudzu -p -c network | egrep -q "^desc: "; then
                vendor=`kudzu -p -c network | egrep -A 3 "^device: $1" | \
                        egrep "^desc:" | awk -F "\"" '{print $2}'`
        else
                vendor=""
        fi
        # state, on | off
        if ethtool $1 2>/dev/null | grep -q "Link detected: yes" && \
           service network status | grep -A 1 "Currently active devices:" \
           | grep -q $1; then
                state="linked"
        else
                state="unlink"
        fi
        # speed
        if [[ $state == "linked" ]]; then
                speed=`ethtool $1 2>/dev/null | awk -F": " '/Speed:/{print $2}'`
        else 
                speed="unknown"
        fi
        # etype device| bond | bridge | subbond | subbridge
        tmp=`awk -F"=" '/TYPE/{print $2}' $netConf 2>/dev/null`
        if [[ "$tmp" == "Bridge" ]]; then
                etype="bridge"
        elif [[ "$tmp" == "Ethernet" ]]; then
                if grep -q "BRIDGE=br" $netConf; then
                        etype="subbridge" 
                elif grep -q "MASTER=bond" $netConf; then
                        etype="subbond" 
                else
                        etype="device" 
                fi
        else
                if grep -q "DEVICE=bond" $netConf; then
                        etype="bond"
                else
                # if grep -q "BRIDGE=br" $netConf; then
                #         etype="subbridge" 
                # elif grep -q "SLAVE=bond" $netConf; then
                #         etype="subbond" 
                # else
                #         etype="device" 
                # fi
                etype="others"
                fi
        fi 

        echo "$ip"
        echo "$mask"
        echo "$mac"
        echo "$mtu"
        echo "$gw"
        echo "$speed"
        echo "$mode"
        echo "$vendor"
        echo "$state"
        echo "$etype"
        # echo "ip:     [$ip]"
        # echo "mask:   [$mask]"
        # echo "mac:    [$mac]"
        # echo "mtu:    [$mtu]"
        # echo "gw:     [$gw]"
        # echo "mode:   [$mode]"
        # echo "vendor: [$vendor]"
        # echo "state:  [$state]"
        # echo "etype:  [$etype]"
}

# no arguments and output like this:
#       /dev/sda                # device
#       976762584               # size(byte)
#       WDC WD1002FBYS-02A6B0   # vendor
#       /dev/sdb                # device
#       976762584               # size(byte)
#       ST31000524NS            # vendor
#       ...
getdisk()
{
        rm -f $tmpFile
        for d in `awk '{print $4}' /proc/partitions | egrep "^[sh]d[a-z]+$"`; do
                echo "/dev/$d" >> $tmpFile
                size=`egrep "$d$" /proc/partitions | awk '{print $3}'`
                size=$[$size * 1024]
                echo $size >> $tmpFile
                if hdparm -I "/dev/$d" 2>/dev/null | grep -q 'Model Number:'; then
                        # hdparm -I "/dev/$d" 2>/dev/null \
                        # | awk -F": *" '/Model Number/{print $2}' \
                        # | grep -o "[^ ]\+\( \+[^ ]\+\)" >>$tmpFile

                        hdparm -I "/dev/$d" 2>/dev/null \
                        | awk -F": *" '/Model Number/{print $2}' \
                        | awk '{for(i=1;i<=NF;i++) {printf $i" "};printf "\n"}' >> $tmpFile
                else
                        echo "" >>$tmpFile
                fi
        done
        cat $tmpFile
}

# with new line 
# df -B 1 / /boot /root/nfs | sed '1d'       
# without new line 
# df -B 1 / /boot /root/nfs | sed '1d' | awk '{if(NF==1){printf($1" ")} else {print $0}}'
# count all path capacity spicified 
# df -B 1 / /boot /root/nfs | sed '1d' | awk '{if(NF==1){printf($1" ")} else {print $0}}' | awk 'BEGIN{sum=0}{sum+=$2}END{print sum}'
# output like this:
# 154818260992          # total
# 6170578944            # used
# 140783362048          # left
getpathcapacity()
{
        if [[ $# -ne 0 ]]; then
                df -B 1 $* 2>/dev/null | sed '1d' | \
                        awk '{if(NF==1){printf($1" ")} else {print $0}}' | \
                        awk 'BEGIN {cap=0; used=0; left=0} \
                                   {cap+=$2;used+=$3;left+=$4} \
                               END {print cap"\n"used"\n"left}'

        else
                printf "0\n0\n0\n"
        fi
}

# no arguments and output like this:
#       fan1
#       3308 RPM
#
#       fan2
#       0 RPM
#       ALARM
#       fan3
#       3375 RPM
#
#       ...
getfan()
{
        /usr/local/bin/sensors &>/dev/null
        if [[ $? -ne 0 ]]; then
                echo "-bash: $1: No sensors detected"
                exit 127
        fi
                
        # check /usr/bin/sensors
        /usr/local/bin/sensors |awk -F": +| +\\\(" '/[fF]an/{if(match($NF, ".*ALARM")) \
                {print $1"\n"$2"\nALARM"}else{print $1"\n"$2"\nOK"}}'
}

# no arguments and output like this:
#       temp1
#       +83.0°C
#       ALARM
#       temp2
#       +26.0°C
#       
#       temp3
#       +27.0°C
#
#       ...
gettemp()
{
        /usr/local/bin/sensors &>/dev/null
        if [[ $? -ne 0 ]]; then
                echo "-bash: $1: No sensors detected"
                exit 127
        fi
        /usr/local/bin/sensors |awk -F": +| +\\\(" '/Temp|temp[1-9]/{if(match($NF, ".*ALARM"))\
                 {print $1"\n"$2"\nALARM"}else{print $1"\n"$2"\nOK"}}'
        #         {print $1"---"$2"===ALARM"}else{print $1"---"$2}}'
}

# no arguments and output like this:
#       192.168.0.1
getdns()
{
        awk '/^nameserver/{print $2}' $dnsFile
}

cleardns()
{
        sed -i "/^\s*nameserver/d" $dnsFile
}

# $1: main dns
# $2: backup dns
setdns()
{
        sed -i "/^\s*nameserver/d" $dnsFile
        for i in $*; do
              printf "nameserver %s\n" $i >> $dnsFile
        done
}

# no arguments and output the hostname
#       localhost.localdomain
gethost()
{
        /bin/hostname
}

# $1: hostname
sethost()
{
        sed -i "/^127.0.0.1\s\+.*/d" $hostFile
        printf "127.0.0.1\tlocalhost localhost.localdomain localhost4 localhost4.localdomain4\n" >> $hostFile
        printf "127.0.0.1\t$1 $1\n" >> $hostFile

        grep -q "^HOSTNAME=" $networkFile
        if [[ $? -ne 0 ]]; then
                printf "HOSTNAME=$1" >> $networkFile
        else
                sed -i "s/^HOSTNAME=.*/HOSTNAME=$1/g" $networkFile
        fi
        /bin/hostname $1
}

# no arguments and output like this:
#       2013-05-03 16:31:32 
gettime()
{
        date +"%Y-%m-%d %H:%M:%S"
}

# $1: time string: %Y-%m-%d %H:%M:%S
settime()
{
        date -s "$1 $2"
}

# no arguments and output like this:
#       192.168.0.1
getntpserver()
{
        ntp=`grep "^server" $ntpConf | egrep -o "([0-9]{1,3}.){3}[0-9]{1,3}"`
        echo "$ntp"
}

# $1: ntp server ipaddress | clear
setntpserver()
{
        if [[ "$1" == "clear" ]]; then
                sed -ri "/^server\s+([0-9]{1,3}.){3}[0-9]{1,3}/d" $ntpConf
                return
        fi

        if grep "^server" $ntpConf| egrep -qo "([0-9]{1,3}.){3}[0-9]{1,3}"; then
                sed -ri "s/^server\s+([0-9]{1,3}.){3}[0-9]{1,3}/server\t$1/" $ntpConf
        else
                printf "server\t%s" $1  >>$ntpConf
        fi

}

# "GMT-1"   =>  （GMT +01:00）柏林, 布鲁塞尔, 哥本哈根, 马德里, 巴黎, 罗马  
# "GMT-2"   =>  （GMT +02:00）开罗, 赫尔辛基, 加里宁格勒, 南非, 华沙  
# "GMT-3"   =>  （GMT +03:00）巴格达, 利雅得, 莫斯科, 奈洛比  
# "GMT-4"   =>  （GMT +04:00）阿布扎比, 巴库, 马斯喀特, 特比利斯  
# "GMT-5"   =>  （GMT +05:00）叶卡特琳堡, 伊斯兰堡, 卡拉奇, 塔什干  
# "GMT-6"   =>  （GMT +06:00）阿拉木图, 科伦坡, 达卡, 新西伯利亚  
# "GMT-7"   =>  （GMT +07:00）曼谷, 河内, 雅加达  
# "GMT-8"   =>  （GMT +08:00）北京, 香港, 帕斯, 新加坡, 台北  
# "GMT-9"   =>  （GMT +09:00）大阪, 札幌, 汉城, 东京, 雅库茨克  
# "GMT-10"  =>  （GMT +10:00）堪培拉, 关岛, 墨尔本, 悉尼, 海参崴  
# "GMT-11"  =>  （GMT +11:00）马加丹, 新喀里多尼亚, 所罗门群岛  
# "GMT-12"  =>  （GMT +12:00）奥克兰, 惠灵顿, 斐济, 马绍尔群岛
# "GMT-0"   =>  （GMT  00:00）都柏林, 伦敦, 里斯本, 卡萨布兰卡  
# "GMT+1"   =>  （GMT -01:00）亚速群岛, 佛得角群岛 
# "GMT+2"   =>  （GMT -02:00）中大西洋, 阿森松群岛, 圣赫勒拿岛  
# "GMT+3"   =>  （GMT -03:00）巴西利亚, 布宜诺斯艾利斯, 乔治敦, 福克兰群岛  
# "GMT+4"   =>  （GMT -04:00）大西洋时间(加拿大), 加拉加斯, 拉巴斯  
# "GMT+5"   =>  （GMT -05:00）东部时间(美国和加拿大), 波哥大, 利马, 基多  
# "GMT+6"   =>  （GMT -06:00）中部时间(美国和加拿大), 墨西哥城  
# "GMT+7"   =>  （GMT -07:00）山区时间(美国和加拿大), 亚利桑那  
# "GMT+8"   =>  （GMT -08:00）太平洋时间(美国和加拿大), 提华纳  
# "GMT+9"   =>  （GMT -09:00）阿拉斯加  
# "GMT+10"  =>  （GMT -10:00）夏威夷  
# "GMT+11"  =>  （GMT -11:00）中途岛, 萨摩亚群岛  
# "GMT+12"  =>  （GMT -12:00）埃尼威托克岛, 夸贾林环礁  
#
# no arguments and output like this:
#       GMT+8
gettimezone()
{
        zone=`date -R | awk '{print $NF}'`
        case "$zone" in
        +0000)  gmt="GMT-0";;
        +0100)  gmt="GMT-1";;
        +0200)  gmt="GMT-2";;
        +0300)  gmt="GMT-3";;
        +0400)  gmt="GMT-4";;
        +0500)  gmt="GMT-5";;
        +0600)  gmt="GMT-6";;
        +0700)  gmt="GMT-7";;
        +0800)  gmt="GMT-8";;
        +0900)  gmt="GMT-8";;
        +1000)  gmt="GMT-10";;
        +1100)  gmt="GMT-11";;
        +1200)  gmt="GMT-12";;
        -0100)  gmt="GMT+1";;
        -0200)  gmt="GMT+2";;
        -0300)  gmt="GMT+3";;
        -0400)  gmt="GMT+4";;
        -0500)  gmt="GMT+5";;
        -0600)  gmt="GMT+6";;
        -0700)  gmt="GMT+7";;
        -0800)  gmt="GMT+8";;
        -0900)  gmt="GMT+9";;
        -1000)  gmt="GMT+10";;
        -1100)  gmt="GMT+11";;
        -1200)  gmt="GMT+12";;
        *)
        esac
        echo "$gmt"
}

# $1: time zone string
settimezone()
{
        zic -l $1
}

# $1: ntp server ip address
synctime()
{
        ntpdate $1
}

# no arguments and output like this:
#       60
getupstime()
{
        awk '/^MINUTES/{print $2}' $upsConf
}

# no arguments and output like this:
#       5
getupspower()
{
        awk '/^BATTERYLEVEL/{print $2}' $upsConf
}

# $1: ups time(min)
setupstime()
{
        sed -i "s/^MINUTES.*/MINUTES $1/g" $upsConf
}

# $1: ups battery level(%)
setupspower()
{
        sed -i "s/^BATTERYLEVEL.*/BATTERYLEVEL $1/g" $upsConf
}

getlinkstate()
{
        /sbin/apcaccess
}

# $1: service name
#       ftp   on on             # servicename state onboot
getservice() 
{
        if [[ "$1" == "rsyncd" ]]; then
                if service $1 status | grep -q "running"; then
                        state="on"
                else
                        state="off"
                fi
        else
                service $1 status &>/dev/null
                if [[ $? -eq 0 ]]; then
                        state="on"
                else
                        state="off"
                fi
        fi
        if chkconfig --list $1 | grep -q "on"; then
                onboot="on"
        else
                onboot="off"
        fi
        echo "$state"
        echo "$onboot"
}

# $1: service name
# $2: start|stop|restart
setservicestate()
{
        service $1 $2 >/dev/null
}

# $1: service name
# $2: on|off
setserviceauto()
{
        chkconfig $1 $2
}

# $1: ip
# $2: user
# $3: password
# usage: test scp connection with remote host
testconnection()
{
        $remote "ssh" $1 $2 $3 "cd ~"
}

# $1: ip
# $2: user
# $3: password
# test function
remotetest()
{
        $remote "ssh" $1 $2 $3 "ls /root"
}

# $1: ip
# $2: user
# $3: password
# $4: command
# description: execute command on remote host
# usage: SshCommand root 192.168.1.90 gfl "/bin/mkdir -p /tmp/tmp/mmm"
remotecommand()
{
        $remote "ssh" $*
}

# $1: ip
# $2: user
# $3: password
# $4: source file or dir
# $5: destination file name or dir
# description: copy file or dir to remote host
# usage: ScpFile root 192.168.1.90 gfl install.sh /tmp/tmp
remotecopy()
{
        $remote "scp_send" $*
}

# $1: ip
# $2: user
# $3: password
# $4: source file or dir
# $5: destination file name or dir
# description: copy remote file or dir to local host
# usage: ScpFile root 192.168.1.90 gfl install.sh /tmp/tmp
remotecopyback()
{
        $remote "scp_recv" $*
}

checkinstallfile()
{
        files=`ls $installDir/scistor*.bin 2>/dev/null`
        if [[ -z "$files" ]]; then
                exit 1
        fi
}

# $1: ip
# $2: user
# $3: password
# description: test scp connection with remote host
copyinstallpacket()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        remotecopy $1 $2 $3 $installDir/$installFile $installDir
}

# $1: ip
# $2: user
# $3: password
# description: remote install gateway agent, install.sh must support `installagent'
installagent()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        remotecommand $1 $2 $3 "$installDir/$installFile installagent"
}

# $1: ip
# $2: user
# $3: password
# description: remote uninstall gateway agent, install.sh must support arg: `uninstallagent'
uninstallagent()
{
        ip=`awk -F '=' '/^server/{printf $2}' $mysqlConf`
        if [[ "$ip" == $1 ]]; then      # agent and server on same node
                return 0
        fi
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        remotecommand $1 $2 $3 "$installDir/$installFile uninstallagent"
}

# description: remote install index, install.sh must support `installindex'
installindex()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        $installDir/$installFile installindex
        sed -i "s/^systemType=.*/systemType=distribute/g" $scigwConf
}

# description: remote uninstall index, install.sh must support arg: `uninstallindex'
uninstallindex()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        $installDir/$installFile uninstallindex
        if [[ $? -ne 0 ]]; then
                echo "execute uninstall index error"
                return 1;
        fi
        for path in `awk -F '=' '/^idxRootPath/{printf $2}' $istriConf`; do
                rm -rf $path &>/dev/null
        done
        rm -rf /etc/istri/ -rf
        return 0
}

# description: remote install data, install.sh must support `installdata'
installdata()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        $installDir/$installFile installdata
        sed -i "s/^systemType=.*/systemType=distribute/g" $scigwConf
}

# description: remote uninstall data, install.sh must support arg: `uninstalldata'
uninstalldata()
{
        checkinstallfile
        installFile=`ls -t $installDir | sed -n 1p`
        $installDir/$installFile uninstalldata
        if [[ $? -ne 0 ]]; then
                echo "execute uninstall data error"
                return 1;
        fi
        for path in `awk -F '=' '/^dataPath|^metaPath|^indexPath|^mountPoint/{print $2}' $istriConf`; do
                rm -rf $path &>/dev/null
        done
        rm -rf /etc/istri/ -rf
        return 0
}

remoteindexstatus()
{
        result=`remotecommand $1 $2 $3 service istriIndex status |sed -n '$p'`
        if echo "$result" | grep -q "Service is stopped"; then
                return 0
        else
                return 1
        fi
}

remotedatastatus()
{
        result=`remotecommand $1 $2 $3 service istriData status |sed -n '$p'`
        if echo "$result" | grep -q "Service is stopped"; then
                return 0
        else
                return 1
        fi
}

# $1: default config file /etc/scigw/default/istri.conf
# $2: backup config file /etc/scigw/default/istri.conf.bak
# create backup config file and remove ^dataPathX|^idxRootPathX
backupconfig()
{
        sed -r '/^idxRootPath|^dataPath/d' $1 > $2
}

# $1: mysql server ip address
setmysqlserver()
{
        sed -i "s/^server=.*/server=$1/g" $mysqlConf
}

# $1: istri index server ip address
setistriserver()
{
        sed -i "s/^indexServerIp=.*/indexServerIp=$1/g" $istriConf
}

# $1, $2, ...: path
# output like this:
# 24100634624   # capacity
# 4639789056    # used
# 18236583936   # left
getindexusage()
{
        getpathcapacity $*
}

# $1, $2, ...: path
# output like this:
# 24100634624   # capacity
# 4639789056    # used
# 18236583936   # left
# 55.74KB       # total
# 19.85KB       # real
# 64.39         # dedup
getdatausage()
{
        getpathcapacity $*
        if [[ $# -ne 0 ]]; then
                util.istri_data -d -c $istriConf | \
                        awk -F [,:\ ] '{printf $3"\n"$7"\n"$11"\n"}'
        else 
                printf "0\n0\n0\n"
        fi 
}

# no arguments and output lick this:
# [1024]
# [n]
# [y]
# [y]
# [y]
# [0 minute]
# [0 KB]
# [1.00 TB]
# [2013-10-24-13-45-13]
# [   5   e   6   0   1   0]
# [4199515]
# [QqVu - gwhy - HEvP - ZHq8 - OkVi - aJT3]
getddfslicense()
{
        ddfsoutput=`util.ddfs -l -f 2>/dev/null`
        if [[ $? -ne 0 ]]; then
                echo "cmd error"
                exit 1
        fi
        Version=`echo "$ddfsoutput" | awk -F ':  ' '/^\tVersion/{print $2}'` 
        Activated=`echo "$ddfsoutput" | awk -F ':  ' '/^\tActivated/{print $2}'` 
        TimePoStatus=`echo "$ddfsoutput" | awk -F ':  ' '/^\tTimePoStatus/{print $2}'` 
        QuotaPoStatus=`echo "$ddfsoutput" | awk -F ':  ' '/^\tQuotaPoStatus/{print $2}'` 
        ABSTimePoStatus=`echo "$ddfsoutput" | awk -F ':  ' '/^\tABSTimePoStatus/{print $2}'` 
        AvailTime=`echo "$ddfsoutput" | awk -F ':  ' '/^\tAvailTime/{print $2}'` 
        CurQuota=`echo "$ddfsoutput" | awk -F ':  ' '/^\tCurQuota/{print $2}'` 
        ABSQuota=`echo "$ddfsoutput" | awk -F ':  ' '/^\tABSQuota/{print $2}'` 
        AvailABSTime=`echo "$ddfsoutput" | awk -F ':  ' '/^\tAvailABSTime/{print $2}'` 
        CpuId=`echo "$ddfsoutput" | awk -F ':  ' '/^\tCpuId/{print $2}'` 
        Inode=`echo "$ddfsoutput" | awk -F ':  ' '/^\tInode/{print $2}'` 
        SerialCode=`echo "$ddfsoutput" | awk -F ':  ' '/^\tSerialCode/{print $2}'` 

        echo "$Version"
        echo "$Activated"
        echo "$TimePoStatus"
        echo "$QuotaPoStatus"
        echo "$ABSTimePoStatus"
        [[ "$AvailTime" == "" ]] && echo "0 minute" || echo "$AvailTime"
        [[ "$CurQuota" == "" ]] && echo "0.00 KB" || echo "$CurQuota"
        [[ "$ABSQuota" == "" ]] && echo "0.00KB" || echo "$ABSQuota"
        [[ "$AvailABSTime" == "" ]] && echo "1970-01-01-00-00-00" || echo "$AvailABSTime"
        echo "$CpuId"
        echo "$Inode"
        echo "$SerialCode"
}

importddfslicense()
{
        util.ddfs -l -i $apacheUploadPath/scigw_ddfs.lic
}

exportddfslicense()
{
        rm -rf $apacheUploadPath/scigw_ddfs.lic
        util.ddfs -l -o $apacheUploadPath/scigw_ddfs.lic
}

# $1: ip
# $2: user
# $3: password
# output lick this:
#       [423954]
#       [n]
#       [10]
#       [0]
#       [y]
#       [y]
#       [y]
#       [90.00 day]
#       [0 KB]
#       [1.00 TB]
#       [2013-11-25-18-04-55]
#       [   3   2   f   0   0   1]
#       [398674]
#       [bAQX - uMqX - QPgP - 0jJc - 3Yvh - SqlA]
getistrilicense()
{
        istrioutput=`remotecommand $1 $2 $3 util.istri_index -l -f 2>/dev/null`
        if [[ $? -ne 0 ]]; then
                echo "cmd error"
                exit 1
        fi
        Version=`echo "$istrioutput" | awk -F ':  ' '/^\tVersion/{print $2}'`
        Activated=`echo "$istrioutput" | awk -F ':  ' '/^\tActivated/{print $2}'`
        NodeNumber=`echo "$istrioutput" | awk -F ':  ' '/^\tNodeNumber/{print $2}'`
        CurNodeNumber=`echo "$istrioutput" | awk -F ':  ' '/^\tCurNodeNumber/{print $2}'`
        TimePoStatus=`echo "$istrioutput" | awk -F ':  ' '/^\tTimePoStatus/{print $2}'`
        QuotaPoStatus=`echo "$istrioutput" | awk -F ':  ' '/^\tQuotaPoStatus/{print $2}'`
        ABSTimePoStatus=`echo "$istrioutput" | awk -F ':  ' '/^\tABSTimePoStatus/{print $2}'`
        AvailTime=`echo "$istrioutput" | awk -F ':  ' '/^\tAvailTime/{print $2}'`
        CurQuota=`echo "$istrioutput" | awk -F ':  ' '/^\tCurQuota/{print $2}'`
        ABSQuota=`echo "$istrioutput" | awk -F ':  ' '/^\tABSQuota/{print $2}'`
        AvailABSTime=`echo "$istrioutput" | awk -F ':  ' '/^\tAvailABSTime/{print $2}'`
        CpuId=`echo "$istrioutput" | awk -F ':  ' '/^\tCpuId/{print $2}'`
        Inode=`echo "$istrioutput" | awk -F ':  ' '/^\tInode/{print $2}'`
        SerialCode=`echo "$istrioutput" | awk -F ':  ' '/^\tSerialCode/{print $2}'`
        echo "${Version:0:$[${#Version}-1]}"
        echo "${Activated:0:$[${#Activated}-1]}"
        echo "${NodeNumber:0:$[${#NodeNumber}-1]}"
        echo "${CurNodeNumber:0:$[${#CurNodeNumber}-1]}"
        echo "${TimePoStatus:0:$[${#TimePoStatus}-1]}"
        echo "${QuotaPoStatus:0:$[${#QuotaPoStatus}-1]}"
        echo "${ABSTimePoStatus:0:$[${#ABSTimePoStatus}-1]}"
        [[ "$AvailTime" == "" ]] && echo "0 minute" || echo "${AvailTime:0:$[${#AvailTime}-1]}"
        [[ "$CurQuota" == "" ]] && echo "0.00 KB" || echo "${CurQuota:0:$[${#CurQuota}-1]}"
        [[ "$ABSQuota" == "" ]] && echo "0.00KB" || echo "${ABSQuota:0:$[${#ABSQuota}-1]}"
        [[ "$AvailABSTime" == "" ]] && echo "1970-01-01-00-00-00" || echo "${AvailABSTime:0:$[${#AvailABSTime}-1]}"
        echo "${CpuId:0:$[${#CpuId}-1]}"
        echo "${Inode:0:$[${#Inode}-1]}"
        echo "${SerialCode:0:$[${#SerialCode}-1]}"
}

# $1: ip
# $2: user
# $3: password
importistrilicense()
{
        tmpLic=/tmp/scigw_${1}_istri.lic
        httpLic=$apacheUploadPath/scigw_${1}_istri.lic
        remotecopy $1 $2 $3 $httpLic $tmpLic
        remotecommand $1 $2 $3 util.istri_index -l -i $tmpLic
}

# $1: ip
# $2: user
# $3: password
exportistrilicense()
{
        tmpLic=/tmp/scigw_${1}_istri.lic
        httpLic=$apacheUploadPath/scigw_${1}_istri.lic
        remotecommand $1 $2 $3 "rm -rf $tmpLic; util.istri_index -l -o $tmpLic"
        remotecopyback $1 $2 $3 $tmpLic $httpLic
}

# $1: ip
# $2: user
# $3: password
# $4: serverip
# $5: src
# $6: mp
remotemount()
{
        testconnection $1 $2 $3
        if [[ $? -ne 0 ]]; then
                echo "cannot connect to $6"
                return 1
        fi

        cmd="[[ -e $6 ]] && echo 'directory exist' || echo 'no such directory'"
        result=`remotecommand $1 $2 $3 $cmd`
        len=${#result}
        laststr=${result:len-18:17}

        if [[ "$laststr" == "no such directory" ]]; then
                remotecommand $1 $2 $3 mkdir -p $6
                if [[ $? -ne 0 ]]; then
                        echo "mkdir $6 on client $1 error"
                        return 1
                fi
        fi

        cmd="mount $4:$5 $6"
        remotecommand $1 $2 $3 $cmd
        if [[ $? -ne 0 ]]; then
                echo "mount $6 on client $1 error"
                return 1
        fi
}

# $1: ip
# $2: user
# $3: password
# $4: mp
remoteumount()
{
        remotecommand $1 $2 $3 "umount $4"
        if [[ $? -ne 0 ]]; then
                echo "umount $6 on client $1 error"
                return 1
        fi
}

# $1: logfile path
# description: print log with color
logutils()
{
        tail -f $1 | 
        awk '{
                sub("DBG", "\033[40;32mDBG\033[0m"); 
                sub("INF", "\033[40;34mINF\033[0m"); 
                sub("WRN", "\033[40;33mWRN\033[0m"); 
                sub("ERR", "\033[40;31mERR\033[0m"); 
                print $0
        }'
}

fbsinstalltest()
{
        installFile=`ls -t $installDir | sed -n 1p`
        remotecommand 192.168.1.92 root gfl "rm -rf $installDir && mkdir -p $installDir"
        remotecopy 192.168.1.92 root gfl "$installDir/$installFile" "$installDir/$installFile"
        remotecommand 192.168.1.92 root gfl "$installDir/$installFile installagent"
        remotecommand 192.168.1.92 root gfl "$installDir/$installFile installdata"
        remotecommand 192.168.1.92 root gfl "$installDir/$installFile installindex"
        remotecommand 192.168.1.92 root gfl "rm -rf $installDir"
}

gettest()
{
        echo "--------getsys--------"
        getsys
        echo "--------getcpu--------"
        getcpu
        echo "--------getmem--------"
        getmem
        echo "--------getdisk--------"
        getdisk
        echo "--------getfan--------"
        getfan
        echo "--------getdns--------"
        getdns
        echo "--------gettemp--------"
        gettemp
        echo "--------gethost--------"
        gethost
        echo "--------getnet--------"
        name=`getnetname`
        for dev in $name; do
                echo "========$dev========"
                getnetinfo $dev
        done
}

main()
{
        i=1             # 计数器
        pos=2           # 取得从第几个参数开始的剩余所有参数列表
        args=""         # 保存第pos个参数开始的参数列表
        for arg in $*; do
                if [[ $i -ge $pos ]]; then
                        if [[ "args$args" == "args" ]]; then
                                args=$arg
                        else 
                                args="$args $arg"
                        fi
                fi
                i=$[$i + 1]
        done

        case "$1" in
        # 测试
        --test) gettest $args;;

        # 系统信息、CPU、内存、风扇、温度
        --getsys) getsys $args;;                   # scigw --getsys
        --getcpu) getcpu $args;;                   # scigw --getcpu
        --getmem) getmem $args;;                   # scigw --getmem
        --getfan) getfan $args;;                   # scigw --getfan
        --gettemp) gettemp $args;;                 # scigw --gettemp

        # 磁盘和目录
        --getdisk) getdisk $args;;                 # scigw --getdisk
        --getpathcapacity) getpathcapacity $args;; # scigw -- getpathcapacity path1[, path2, ..]

        # 网络
        --getnetname) getnetname $args;;           # scigw --getnetname
        --getnetinfo) getnetinfo $args;;           # scigw --getnetinfo eth0

        # 时间、时区设置和时间同步
        --gettime) gettime $args;;                 # scigw --gettime
        --settime) settime $args;;                 # scigw --settime YYYY-MM-DD hh:mm:ss
        --synctime) synctime $args;;               # scigw --synctime ip
        --gettimezone) gettimezone $args;;         # scigw --gettimezone
        --settimezone) settimezone $args;;         # scigw --settimezone zone
        --getntpserver) getntpserver $args;;       # scigw --getntpserver
        --setntpserver) setntpserver $args;;       # scigw --setntpserver ip

        # DNS 和主机名
        --getdns) getdns $args;;                   # scigw --getdns
        --setdns) setdns $args;;                   # scigw --setdns dns1[, dns2]
        --gethost) gethost $args;;                 # scigw --gethost
        --sethost) sethost $args;;                 # scigw --sethost hostname
        --cleardns) cleardns $args;;               # scigw --cleardns

        # UPS
        --getupstime) getupstime $args;;           # scigw --getupstime
        --getupspower) getupspower $args;;         # scigw --getupspower
        --setupstime) setupstime $args;;           # scigw --setupstime 60
        --setupspower) setupspower $args;;         # scigw --setupspower 5
        --getlinkstate) getlinkstate $args;;       # scigw --getlinkstate

        # 服务管理
        --getservice) getservice $args;;                 # scigw --getservice service
        --setserviceauto) setserviceauto $args;;         # scigw --setserviceauto service on|off
        --setservicestate) setservicestate $args;;       # scigw --setservicestate service start|stop
        --getddfslicense) getddfslicense $args;;         # scigw --getddfslicense
        --importddfslicense) importddfslicense $args;;   # scigw --importddfslicense lic
        --exportddfslicense) exportddfslicense $args;;   # scigw --exportddfslicense lic
        --getistrilicense) getistrilicense $args;;       # scigw --getistrilicense
        --importistrilicense) importistrilicense $args;; # scigw --importistrilicense lic
        --exportistrilicense) exportistrilicense $args;; # scigw --exportistrilicense lic

        # 连接测试、远程拷贝、远程执行命令
        # rpm -ivh tcl-8.5.7-6.el6.x86_64.rpm
        # rpm -ivh expect-5.44.1.15-2.el6.x86_64.rpm
        --remotecopy) remotecopy $args;;                # scigw --remotecopy ip user password src dest
        --remotecommand) remotecommand $args;;          # scigw --remotecommand ip user password command
        --testconnection) testconnection $args;;        # scigw --testconnection ip user password
        --copyinstallpacket) copyinstallpacket $args;;  # scigw --copyinstallpacket ip user password
        --installagent) installagent $args;;            # scigw --installagent ip user password
        --uninstallagent) uninstallagent $args;;        # scigw --uninstallagent ip user password
        --installindex) installindex $args;;            # scigw --installindex ip user password
        --uninstallindex) uninstallindex $args;;        # scigw --uninstallindex ip user password
        --installdata) installdata $args;;              # scigw --installdata ip user password
        --uninstalldata) uninstalldata $args;;          # scigw --uninstalldata ip user password
        --remoteindexstatus) remoteindexstatus $args;;  # scigw --remoteindexstatus ip user password
        --remotedatastatus) remotedatastatus $args;;    # scigw --remotedatastatus ip user password
        --backupconfig) backupconfig $args;;            # scigw --backupconfig defaultconfig backupconfig
        --fbsinstalltest) fbsinstalltest $args;;        # scigw --fbsinstalltest ip user password
        --remotetest) remotetest $args;;                # scigw --remotetest ip user password

        --setmysqlserver) setmysqlserver $args;;        # scigw --setmysqlserver ip
        --setistriserver) setistriserver $args;;        # scigw --setistriserver ip
        --getindexusage) getindexusage $args;;          # scigw --getindexusage path1[, path2, ..]
        --getdatausage) getdatausage $args;;            # scigw --getdatausage path1[, path2, ..]

        --remotemount) remotemount $args;;              # scigw --remotemount ip user password src mp  
        --remoteumount) remoteumount $args;;            # scigw --remoteumount ip user password mp  
        #  日志工具
        --logutils) logutils $args;;                    # scigw --logutils /var/log/gateway
        *)
        printf "invalid argument, usage: \n"
        printf "\t$0 --getsys\n"
        printf "\t$0 --getcpu\n"
        printf "\t$0 --getmem\n"
        printf "\t$0 --getfan\n"
        printf "\t$0 --gettemp\n"
        echo 
        printf "\t$0 --getdisk\n"
        printf "\t$0 -- getpathcapacity path1[, path2, ..]\n"
        echo 
        printf "\t$0 --getnetname\n"
        printf "\t$0 --getnetinfo eth0\n"
        echo 
        printf "\t$0 --gettime\n"
        printf "\t$0 --settime YYYY-MM-DD hh:mm:ss\n"
        printf "\t$0 --synctime ip\n"
        printf "\t$0 --gettimezone\n"
        printf "\t$0 --settimezone zone\n"
        printf "\t$0 --getntpserver\n"
        printf "\t$0 --setntpserver ip\n"
        echo 
        printf "\t$0 --getdns\n"
        printf "\t$0 --setdns dns1[, dns2]\n"
        printf "\t$0 --gethost\n"
        printf "\t$0 --sethost hostname\n"
        printf "\t$0 --cleardns\n"
        echo 
        printf "\t$0 --getupstime\n"
        printf "\t$0 --getupspower\n"
        printf "\t$0 --setupstime 60\n"
        printf "\t$0 --setupspower 5\n"
        printf "\t$0 --getlinkstate\n"
        echo 
        printf "\t$0 --getservice service\n"
        printf "\t$0 --setserviceauto service on|off\n"
        printf "\t$0 --setservicestate service start|stop\n"
        echo 
        printf "\t$0 --remotecopy ip user password src dest\n"
        printf "\t$0 --remotecommand ip user password command\n"
        printf "\t$0 --testconnection ip user password\n"
        printf "\t$0 --copyinstallpacket ip user password\n"
        printf "\t$0 --installagent ip user password\n"
        printf "\t$0 --uninstallagent ip user password\n"
        printf "\t$0 --installindex ip user password\n"
        printf "\t$0 --uninstallindex ip user password\n"
        printf "\t$0 --installdata ip user password\n"
        printf "\t$0 --uninstalldata ip user password\n"
        printf "\t$0 --backupconfig defaultconfig backupconfig\n"
        printf "\t$0 --remotetest ip user password\n"
        printf "\t$0 --fbsinstalltest ip user password\n"
        echo 
        printf "\t$0 --setmysqlserver ip\n"
        printf "\t$0 --setistriserver ip\n"
        printf "\t$0 --getdatausage\n"
        printf "\t$0 --getindexusage\n"
        echo
        printf "\t$0 --getddfslicense\n"
        printf "\t$0 --importddfslicense\n"
        printf "\t$0 --exportddfslicense\n"
        printf "\t$0 --getistrilicense ip user password\n"
        printf "\t$0 --importistrilicense ip user password\n"
        printf "\t$0 --exportistrilicense ip user password\n"
        echo
        printf "\t$0 --remotemount ip user password src mp\n"
        printf "\t$0 --remoteumount ip user password mp\n"

        echo 
        printf "\t$0 --logutils logfile\n"
        exit 1
        esac
}
main $*
