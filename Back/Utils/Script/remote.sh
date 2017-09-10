#!/usr/bin/expect
###############################################################################
#
# @ File Name  : remote.sh
# @ Date       : 2013-08-09
# @ Author     : gaofeilong <gaofeilonglcu@gmail.com>
# @ Description: 网关系统辅助脚本，测试连接、远程执行命令、远程拷贝文件
# @ Usage      : remote.sh ssh      ip user password cmd[arg1, arg2...]
#                remote.sh scp_send ip user password src dest_on_remote_host
#                remote.sh scp_recv ip user password src_on_remote_host dest
# @ History    : 2013-08-09：创建
#
##############################################################################/


set type     [lindex $argv 0]
set ip       [lindex $argv 1]
set user     [lindex $argv 2]
set password [lindex $argv 3]
set cmd      ""
set ret      0

# argument checking
if {$type != "ssh" && $type != "scp_send" && $type != "scp_recv"} {
        send_user "unknown remote cmd type: $type\n"
        exit 1
}
if {$type == "ssh"} {
        for {set i 4} {$i < $argc} {incr i} {
                set arg [lindex $argv $i]
                if {$cmd != ""} {
                        set cmd "$cmd $arg"
                } else {
                        set cmd "$arg"
                }
        }
} else {
        set src  [lindex $argv 4]
        set dest [lindex $argv 5]
}

# debug info
# send_user "TYPE:   $type\n"
# send_user "IP:     $ip\n"
# send_user "USER:   $user\n"
# send_user "PASSWD: $password\n"
# if {$type == "ssh"} {
#         send_user "CMD:    $cmd\n"
# } else {
#         send_user "SRC:    $src\n"
#         send_user "DEST:   $dest\n"
# }

set timeout 90
for {} {1} {} { 
        if {$type == "ssh"} {
                spawn ssh $user@$ip $cmd
        } else {
                if {$type == "scp_send"} {
                        spawn scp -r $src $user@$ip:$dest
                } else {
                        spawn scp -r $user@$ip:$src $dest
                }
        }

        expect {
                "password:" { 
                        send "$password\r"; 
                        expect {
                                "password:" { 
                                        send_user "\ninvalid password\n"
                                        exit 1 
                                } -re "(&|#|\\$) " {
                                        send "quit\r"
                                        send_user "\n"
                                        exit 0
                                } "No such file or directory" {
                                        send_user "\nno such file or directory\n"
                                        exit 1 
                                } eof {
                                        set result [ wait result ]
                                        exit [lindex $result 3]
                                }
                        }
                } "Host key verification failed." {
                        set ret 100
                        expect eof
                } -re "(&|#|\\$) " {
                        send "quit\r"
                        send_user "\n"
                        exit 0
                } "(yes/no)?" { 
                        send "yes\r"
                        exp_continue 
                } "Permission denied*" { 
                        send_user "permission denied\n"
                        exit 1 
                } "*No route to host" { 
                        send_user "no route to host\n"
                        exit 1 
                } timeout { 
                        send_user "timeout\n"
                        exit 1 
                } eof { 
                        #send_user "command end successfully\n"
                        exit 0
                }

                if {$ret == 100} {
                        exec sed -i "/^${ip}/d" /root/.ssh/known_hosts
                        set ret 0
                } else {
                        exit 0
                }
        }
}
