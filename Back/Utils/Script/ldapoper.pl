#!/usr/bin/perl

local $ldap_usersuffix = "ou=People";
local $ldap_groupsuffix = "ou=Group";

#local $ldap_password = "111111";
#local $ldap_basedn   = "dc=my-domain,dc=com";
#local $ldap_rootdn   = "cn=Manager,dc=my-domain,dc=com";
#$node = 0;

local $ldap_rootdn   = $ARGV[0];
local $ldap_basedn   = $ARGV[1];
local $ldap_password = $ARGV[2];
$node = 3;

local $ldap_tls = "";

#$ldap_tls = " -ZZ ";
$func = $ARGV[$node];

if ($func eq "grouplist")
{
        &LdapGroup();
}

if ($func eq "userlist")
{
        &getLDAPuser();
}

if ($func eq "groupuser")
{
        my $para1 = $ARGV[$node + 1];
        &getGroupUser($para1);
}
#删除用户 参数: uname
if ($func eq "deluser")
{
        my $para1 = $ARGV[$node + 1];
        &delUser($para1);
}
#添加用户 user,uid,gid,pwd,tags
if ($func eq "adduser")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        $para3 = $ARGV[$node + 3];
        $para4 = $ARGV[$node + 4];
        $para5 = $ARGV[$node + 5];
        &addUser($para1, $para2, $para3, $para4,$para5);
}
#添加组 group,gid,tags
if ($func eq "addgroup")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        $para3 = $ARGV[$node + 3];
        &addGroup($para1, $para2, $para3);
}
#设置密码: user,newpwd
if ($func eq "chpwd")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        &setPwd($para1, $para2);
}
#删除组 group
if ($func eq "delgroup")
{
        $para1 = $ARGV[$node + 1];
        &delGroup($para1);
}

if ($func eq "addgroupuser")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        &addGroupUser($para1, $para2);
}

if ($func eq "delgroupuser")
{
        $para1 = $ARGV[$node + 1];
        $para2 = $ARGV[$node + 2];
        &delGroupUser($para1, $para2);
}

#列出所有的group: name,id,tags
sub LdapGroup()
{
	$searchstring = "objectclass=posixgroup";
	$cmd          = "/usr/bin/ldapsearch $ldap_tls -x -D \" $ldap_rootdn \" -w \"$ldap_password\" -S gidNumber -b \"$ldap_groupsuffix,$ldap_basedn\" \"$searchstring\" -LLL |";
        
        my %result;
        local $pos = 1;
        local $id = 0;
	open(FILE, $cmd);
        while(<FILE>) {
		if (/^#/) {
			next;
		}
		if (/^\n/ && $pos == 0) {
			%{$result{$id}} = %temp;
                        $id ++;
                        %temp = ('name'=>'', 'gid'=>'0', 'tags'=>'');
			$pos = 1;
		}
		if (/^(\w+):\s*([\w=,-_]*)/) {
			if ($1 eq "dn") {
				$pos = 0;
			}
			if ($1 eq "description") {
				$temp{'tags'} = $2;
			}
			if ($1 eq "gidNumber") {
				$temp{'gid'} = $2;
			}
			if ($1 eq "cn") {
				$temp{'name'} = $2;
			}
		}
                if (/^\s*([^:]+)\n$/) {
                        $temp{'tags'} .= $1; 
                }       
        }
	close(FILE);
        while ( ($key, $value) = each (%result) ) {
                print "$value->{'name'}:$value->{'gid'}:$value->{'tags'}:\n";
        }
}

#列出所有的user: name,id,gid,tags
sub getLDAPuser()
{
	my $searchstring = "objectclass=posixaccount";
	my $cmd          = "/usr/bin/ldapsearch $ldap_tls -x -D \"$ldap_rootdn\" -w \"$ldap_password\" -b \"$ldap_usersuffix,$ldap_basedn\" \"$searchstring\" uid uidNumber objectClass description gidNumber -LLL|";

        my %result;
        $id = 0;
        local $pos = 1;
        open(FILE, $cmd);
        while(<FILE>) {
		if (/^#/) {
			next;
		}
                if (/^\n/ && $pos == 0) {
                        $len            = @result;
                        %{$result{$id}} = %temp;
                        $id++;
                        %temp = ('name'=>'', 'uid'=>'', 'gid'=>'0', 'tags'=>'');
                        $pos = 1;
                }
                if (/^(\w+):\s*([\w=,-_]*)/) {
                        if ($1 == "dn") {
                                $pos = 0;
                        }
                        if ($1 eq "description") {
                                $temp{'tags'} = $2;
                        }
                        if ($1 eq "uidNumber") {
                                $temp{'uid'} = $2;
                        }
                        if ($1 eq "uid") {
                                $temp{'name'} = $2;
                        }
                        if ($1 eq "gidNumber") {
                                $temp{'gid'} = $2;
                        }
                }
                 if (/^\s*([^:]+)\n$/) { 
                         $temp{'tags'} .= $1;
                 }
        }
	close(FILE);
        while ( ($key, $value) = each (%result) ) {
                if ($value->{'uid'} ne '') {
                        print "$value->{'name'}:$value->{'uid'}:$value->{'gid'}:$value->{'tags'}:\n";
                }
        }
}

#列出该group中的所有成员 $gid
sub getGroupUser()
{
        $gid          = $_[0];
	$searchstring = "objectclass=posixgroup";
	$cmd          = "/usr/bin/ldapsearch $ldap_tls -x -D \" $ldap_rootdn \" -w \"$ldap_password\" -S gidNumber -b \"$ldap_basedn\" \"$searchstring\" \"$searchstring\" gidNumber memberUid -LLL |";
        $pos    = 1;
        $has = 0;
	open(FILE, $cmd);
        while(<FILE>) {
		if (/^#/) {
			next;
		}
                if (/^dn/) {
                        $pos = 1;
			next;
		}
		if (/^(\w+):\s*([\w = ,]*)/) {
			if ($1 eq "gidNumber") {
                                if ($2 eq $gid) {
                                        $has = 1;
                                        $pos = 0;
                                }
                                next;
			}
			if ($1 eq "memberUid" && $pos == 0) {
				print "$2:";
			}
		}
        }

	close(FILE);
}

sub ldapmodify()
{
        $string = $_[0];

	$cmd="echo -e \"$string\" | /usr/bin/ldapmodify $ldap_tls -x -D \"$ldap_rootdn\" -w $ldap_password";
        
        system($cmd);

	if ($? == 0) 
        {
                print "success\n";
                exit(0);
        }
	print "fail\n";
        exit(1);
}

#删除用户 参数: uname
sub delUser()
{
        $user = $_[0];
	$LDIF="dn: uid=$user,$ldap_usersuffix,$ldap_basedn
	changetype: delete";

	&ldapmodify($LDIF);

}

#添加用户 user,uid,gid, tags
sub addUser()
{
#$_[] : 0     = user, 1=uid, 2=gid
        $user = $_[0];
        $uid  = $_[1];
        $gid  = $_[2];        
        $passwd = $_[3];
        $tags = $_[4];

	$string = "dn: uid=$user,$ldap_usersuffix,$ldap_basedn
objectClass: posixAccount
objectClass: inetOrgPerson
objectClass: organizationalPerson
objectClass: person
homeDirectory: /home/$user
loginShell: /bin/false
uid: $user
cn: $user
uidNumber: $uid\n";
        if($tags ne '')
        {
                $string .= "description: $tags\n";
        }
        $string .= "gidNumber: $gid\n";
        $string .= "sn: $user\n";

	$cmd="echo -e \"$string\" | /usr/bin/ldapadd $ldap_tls -x -D \"$ldap_rootdn\" -w $ldap_password";
        system($cmd);

	if ($? == 0) 
        {
                print "success \n";
                &setPwd($user,$passwd);
                exit(0);
        }
        else {
                print "fail \n";
        }
        exit(1);
}

#添加组 group,gid,tags
sub addGroup()
{
#$_[] : 0 = group, 1=gid
	
        $group = $_[0];
        $gid   = $_[1];
        $tags  = $_[2];

	$string = "";

	if ($tags eq '') {
		$string = "dn: cn=$group, $ldap_groupsuffix, $ldap_basedn
		gidNumber: $gid
		objectClass: posixGroup
		cn: $group";
	}
	else {
		$string = "dn: cn=$group, $ldap_groupsuffix, $ldap_basedn
		gidNumber: $gid
		objectClass: posixGroup
		description: $tags
		cn: $group";
	}
        
	$cmd="echo -e \"$string\" | /usr/bin/ldapadd $ldap_tls -x -D \"$ldap_rootdn\" -w $ldap_password";
        print "$cmd";
        system($cmd);

	if ($? == 0) 
        {
                print "success \n";
                exit(0);
        }
        exit(1);
}

#设置密码: user,newpwd
sub setPwd()
{
#$_[] : 0=group, 1=gid
        $user = $_[0];
        $pwd = $_[1];
        
	$cmd = "ldappasswd $ldap_tls -x -w $ldap_password -D \"$ldap_rootdn\" -s $pwd  uid=$user,\"$ldap_usersuffix\",\"$ldap_basedn\"";

        print "$cmd";
        system($cmd);

	if ($? == 0) 
        {
                print "success \n";
                exit(0);
        }
        exit(1);
}

#删除组 group
sub delGroup()
{
        $group = $_[0];
	$LDIF="dn: cn=$group,$ldap_groupsuffix,$ldap_basedn
	changetype: delete";
	&ldapmodify($LDIF);
}

#$group, $user
sub addGroupUser()
{
        $group = $_[0];
        $user = $_[1];
	$LDIF="dn: cn=$group,$ldap_groupsuffix,$ldap_basedn
	add: memberuid
	memberuid: $user";
	
	&ldapmodify($LDIF);
}

#$group, $user
sub delGroupUser()
{
        $group = $_[0];
        $user = $_[1];
	$LDIF="dn: cn=$group,$ldap_groupsuffix,$ldap_basedn
	delete: memberuid
	memberuid: $user";
	
	&ldapmodify($LDIF);
}
