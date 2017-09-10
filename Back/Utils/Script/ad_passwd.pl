#!/usr/bin/perl
use Expect;

if(@ARGV < 3)
{
	print "argv:domain,serverip,passwd\n";
	exit;
}
my $exp = new Expect;

my $domain = $ARGV[0];
if($domain =~ /.com$/i)
{
        $domain =~ s/.com$//i;
}
#/usr/bin/net join -w SCINAS -S 192.168.1.58 -U Administrator
$exp->spawn("/usr/bin/net join -w $domain -S $ARGV[1] -U Administrator");

$exp->expect(250,[
qr/assword:/i,
sub 
        {
        my $self = shift ;
        $self->send("$ARGV[2]\n");
        exp_continue;
        }
],
[
qr/assword:/i,
sub
        {
        my $self = shift ;
        $self->send("$ARGV[2]\n");
        exp_continue;
        }
]
);
$exp->soft_close();