#!/usr/bin/perl

$cmd = "rm /var/lib/ldap/* -rf";
system($cmd);

$cmd = "cp /etc/openldap/DB_CONFIG.example /var/lib/ldap/DB_CONFIG";
system($cmd);

$string = "
# create base
dn: dc=my-domain,dc=com
objectClass: dcObject
objectClass: organization
dc: my-domain
o: my-domain
description: Posix and Samba LDAP Identity Database

#create People
dn: ou=People,dc=my-domain,dc=com
ou: People
objectClass: top
objectClass: organizationalUnit

#create Group
dn: ou=Group,dc=my-domain,dc=com
ou: Group
objectClass: top
objectClass: organizationalUnit

#Machine
dn: ou=Machines,dc=my-domain,dc=com
ou: Machines
objectClass: top
objectClass: organizationalUnit

#Domains
dn: ou=Domains,dc=my-domain,dc=com
ou: Domains
objectClass: top
objectClass: organizationalUnit";

$cmd="echo -e \"$string\" | /usr/sbin/slapadd -b \"dc=my-domain,dc=com\"  -v";
system($cmd);

if ($? != 0) 
{
	print "fail \n";
	exit(-1);
}

$cmd="chown -R ldap:ldap /var/lib/ldap";
system($cmd);

if ($? != 0) 
{
	print "fail \n";
	exit(-1);
}
print "success \n";
exit(0);


