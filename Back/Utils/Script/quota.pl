#!/usr/bin/perl

$node = 0;
$func = $ARGV[$node];

$ldap_perl = "/etc/nas/Script/ldapoper.pl";
$usertype_conf = "/etc/nas/Conf/usertype.conf";
$usertype = 0;

$ldap_rootdn   = "";
$ldap_basedn   = "";
$ldap_password = "";

if($func eq "group" || $func eq "user" || $func eq "allquotainfo")
{
        open(FILE, $usertype_conf);
        while (<FILE>) {
                if (/\s*usertype\s*=\s*(\d)/) {
                        $usertype = $1;
                        last;
                }
        }
        close(FILE);
}
if($usertype == 2)
{
        open(FILE, $usertype_conf);
        while (<FILE>) {
                if (/\s*rootdn\s*=\s*(\S+)/) {
                        $ldap_rootdn = $1;
                }
                elsif (/\s*basedn\s*=\s*(\S+)/) {
                        $ldap_basedn = $1;
                }
                elsif (/\s*pwd\s*=\s*(\S+)/) {
                        $ldap_password = $1;
                }
        }
        close(FILE);
}
# in : path
# out : gid,gname,type,bused,bsoft,bhard,fused,fsoft,fhard
if ($func eq "group")
{
        $para1 = $ARGV[$node + 1];
        &PrintGroupQuota($para1);
}
# in : path
# out : gid,gname,type,bused,bsoft,bhard,fused,fsoft,fhard
elsif ($func eq "user")
{
        $para1 = $ARGV[$node + 1];
        &PrintUserQuota($para1);
}
#in uid,bsoft,bhard,fsoft,fhard,file
#out success, fail
elsif ($func eq "setuser")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        $para3 = $ARGV[$node + 3];
        $para4 = $ARGV[$node + 4];
        $para5 = $ARGV[$node + 5];
        $para6 = $ARGV[$node + 6];

        &setUserQuota($para1, $para2,$para3, $para4, $para5, $para6);
}
#in uid,bsoft,bhard,fsoft,fhard,file
#out success, fail
elsif ($func eq "setgroup")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        $para3 = $ARGV[$node + 3];
        $para4 = $ARGV[$node + 4];
        $para5 = $ARGV[$node + 5];
        $para6 = $ARGV[$node + 6];

        &setGroupQuota($para1, $para2,$para3, $para4, $para5, $para6);
}
elsif ($func eq "disk"){
        open(DD, "grep quota /etc/fstab |");
        while (<DD>) {
                if (/(^[\w\/]+)\s+([\w\/]+)/) {
                        print "$2:";
                }
        }
        close(DD);
        print "\n";
}
elsif ($func eq "allquotainfo")
{
        %path;
        $cmd = "mount | grep usrquota";
        open(INFO, "$cmd | ");
        foreach my $line(<INFO>)
        {
                chomp($line);
                if($line =~ /^\s*\S+\s+on\s+(\S+)/)
                {
                        $path{$1} = 1;
                }
        }
        close(INFO);

        %list;
        foreach my $cur_path(keys %path)
        {
                my $grouplist = &GetGroupQuota($cur_path);
                while (($id, $value ) = each(%{$grouplist}))
                {
                        ($type1,$type2, $type) = (0,0,0);
                        #type 1:空间使用量超过软配额
                        #type 2:文件数超过软配额
                        #type 3:1和2
                        if($value->{'bused'} >= $value->{'bsoft'} && $value->{'bsoft'} > 0)
                        {
                                $type1 = 1;
                                $type = 1;
                        }
                        if($value->{'fused'} >= $value->{'fsoft'} && $value->{'fsoft'} > 0)
                        {
                                $type2 = 2;
                                $type = 2;
                        }

                        if($type1 == 0 && $type2 == 0)
                        {
                                next;
                        }
                        if($type1 > 0 && $type2 > 0)
                        {
                                $type = 3;
                        }
                        print "group:$cur_path:$id:$value->{'group'}:$type:$value->{'bused'}:$value->{'bsoft'}:$value->{'fused'}:$value->{'fsoft'}\n";
                }

                my $userlist = &GetUserQuota($cur_path);
                while (($id, $value ) = each(%{$userlist}))
                {
                        ($type1,$type2, $type) = (0,0,0);
                        #type 1:空间使用量超过软配额
                        #type 2:文件数超过软配额
                        #type 3:1和2
#                        if($value->{'bused'} >= $value->{'bsoft'} && $value->{'bsoft'} > 0)
#                        {
#                                $type1 = 1;
#                                $type = 1;
#                        }
#                        if($value->{'fused'} >= $value->{'fsoft'} && $value->{'fsoft'} > 0)
#                        {
#                                $type2 = 2;
#                                $type = 2;
#                        }
#
#                        if($type1 == 0 && $type2 == 0)
#                        {
#                                next;
#                        }
#                        if($type1 > 0 && $type2 > 0)
#                        {
#                                $type = 3;
#                        }
                        print "user:$cur_path:$id:$value->{'user'}:$type:$value->{'bused'}:$value->{'bsoft'}:$value->{'fused'}:$value->{'fsoft'}\n";
                }
        }
}
else
{
        exit(1);
}

sub GetGroupQuota()
{
        %list;
        if($usertype == 1)
        {
                $cmd = "awk -F \":\" \'\$3 >= 500 && \$3 <= 4294967293 {print \$3\":\"\$1}\' /etc/group";
                #	print "$cmd \n";
                open(GROUP, "$cmd | ");

                while (<GROUP>) {
                        if (/(\d+):(\S+)/) {
                                %glist = ('group' => "$2", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$1}} = %glist;
                        }
                }
                close(GROUP);
        }
        elsif($usertype == 2)
        {
                $cmd = "$ldap_perl $ldap_rootdn $ldap_basedn $ldap_password grouplist";

                open(GROUP, "$cmd | ");

                while (<GROUP>) {
                        if (/(\S+):(\d+)/) {
                                %glist = ('group' => "$1", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$2}} = %glist;
                        }
                }
                close(GROUP);
        }
        elsif($usertype == 4)
        {
                open(GROUP, "ypcat group | ");

                while (<GROUP>) {
                        if (/(\S+):[^\:]+:(\d+)/) {
                                %glist = ('group' => "$1", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$2}} = %glist;
                        }
                }
                close(GROUP);
        }

        $path = $_[0];
        open(GQUOTA, "repquota -vgn $path |");
        $pos = 0;
        while (<GQUOTA>) {
                $pos += 1;
                if ($pos < 5) {
                    next;
                }

                if (/^#(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)/
                  ||/^#(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+\d+\D+(\d+)\s+(\d+)\s+(\d+)/) {
                        if ($1 eq '0' || !defined $list{$1}) {
                            next;
                        }
#			print "$1;$2;$3;$4;$5;$6;$7;$8\n***\n";
                        $list{$1}{'bused'} = $3;
                        $list{$1}->{'bsoft'} = $4;
                        $list{$1}->{'bhard'} = $5;
                        $list{$1}->{'fused'} = $6;
                        $list{$1}->{'fsoft'} = $7;
                        $list{$1}->{'fhard'} = $8;
                }

        }
        close(GQUOTA);
        return \%list;

}
sub PrintGroupQuota()
{
        my $path = $_[0];
        my $grouplist = &GetGroupQuota($path);
        while (($d, $value ) = each(%{$grouplist})) {
                print "$d:$value->{'group'}:$value->{'type'}:$value->{'bused'}:$value->{'bsoft'}:$value->{'bhard'}:$value->{'fused'}:$value->{'fsoft'}:$value->{'fhard'}:\n";
        }
}

sub GetUserQuota()
{
        %list;
        if($usertype == 1)
        {
                $cmd = "awk -F \":\" \'\$3 >= 500 && \$3 <= 4294967293 {print \$3\":\"\$1}\' /etc/passwd";
                #print "$cmd \n";
                open(USER, "$cmd | ");

                while (<USER>) {
                        if (/(\d+):(\S+)/) {
                                %ulist = ('user' => "$2", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$1}} = %ulist;
                        }
                }
                close(USER);
        }
        elsif($usertype == 2)
        {
                $cmd = "$ldap_perl $ldap_rootdn $ldap_basedn $ldap_password userlist";

                open(USER, "$cmd | ");

                while (<USER>) {
                        if (/([^:]+):(\d+):/) {
                                %ulist = ('user' => "$1", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$2}} = %ulist;
                        }
                }
                close(USER);
        }
        elsif($usertype == 3)
        {
                $cmd = "wbinfo -u | grep -v '\\\\'";
                open(USER, "$cmd | ");

                while (<USER>) {
                        if (/(\S+)/)
                        {
                                $cmd2 = "wbinfo --user-info=$1";
                                open(USER_DETAIL, "$cmd2 | ");
                                {
                                        while (<USER_DETAIL>)
                                        {
                                                if (/(\S+):\**:(\d+)/)
                                                {
                                                        %ulist = ('user' => "$1", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                                        %{$list{$2}} = %ulist;
                                                        last;
                                                }
                                        }
                                }
                                close(USER_DETAIL);
                        }
                }
                close(USER);
        }
        elsif($usertype == 4)
        {
                $cmd = "ypcat passwd";
                open(USER, "$cmd | ");

                while (<USER>) {
                        if (/(\S+):[^\:]+:(\d+):\d+:/) {
                                %ulist = ('user' => "$1", 'type' => '1', 'bused' => '0', 'bsoft' => '0', 'bhard' => '0', 'fused' => '0', 'fsoft' => '0', 'fhard' => '0');
                                %{$list{$2}} = %ulist;
                        }
                }
                close(USER);
        }
        $path = $_[0];
        open(GQUOTA, "repquota -vun $path |");
        $pos = 0;
        while (<GQUOTA>) {
                $pos += 1;
                if ($pos < 5) {
                    next;
                }
                if (/^#(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)/
                  ||/^#(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+\d+\D+(\d+)\s+(\d+)\s+(\d+)/) {
                        if ($1 eq '0' || !defined $list{$1}) {
                            next;
                        }
#			print "$1;$2;$3;$4;$5;$6;$7;$8\n***\n";
                        $list{$1}{'bused'} = $3;
                        $list{$1}->{'bsoft'} = $4;
                        $list{$1}->{'bhard'} = $5;
                        $list{$1}->{'fused'} = $6;
                        $list{$1}->{'fsoft'} = $7;
                        $list{$1}->{'fhard'} = $8;
                }

        }
        close(GQUOTA);
        return \%list;
}
sub PrintUserQuota()
{
        my $path = $_[0];
        my $userlist = &GetUserQuota($path);
        while (($d, $value ) = each(%{$userlist})) {
                print "$d:$value->{'user'}:$value->{'type'}:$value->{'bused'}:$value->{'bsoft'}:$value->{'bhard'}:$value->{'fused'}:$value->{'fsoft'}:$value->{'fhard'}:\n";
        }
}

sub setUserQuota()
{
        $uid = $_[0];
        $softblock = $_[1];
        $hardblock = $_[2];
        $softinode = $_[3];
        $hardinode = $_[4];
        $filepath = $_[5];

        $cmd = "setquota -u $uid $softblock $hardblock $softinode $hardinode $filepath";
        print "$cmd\n";
        system($cmd);
        if ($? eq 0) {
                print "success \n";
                exit(0);
        }
        else {
                print "fail \n";
                exit(1);
        }
}

sub setGroupQuota()
{
        $gid = $_[0];
        $softblock = $_[1];
        $hardblock = $_[2];
        $softinode = $_[3];
        $hardinode = $_[4];
        $filepath = $_[5];

        $cmd = "setquota -g $gid $softblock $hardblock $softinode $hardinode $filepath";
        print "$cmd \n";
        system($cmd);
        if ($? == 0) {
                print "success \n";
                exit(0);
        }
        else {
                print "fail \n";
                exit(1);
        }
}
