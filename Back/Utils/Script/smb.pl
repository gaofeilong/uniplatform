#!/usr/bin/perl

$func = $ARGV[0];
$gname = $ARGV[1];

$gname = "\@".$gname;

my $found = 0;
if ($func eq "addgroup")
{       
        open(SMB_R, "/etc/samba/smb.conf");
        open(SMB_W, ">/etc/samba/smb.conf_tmp");
        while (<SMB_R>) {
        	my $line = $_;
        	chomp($line);
        	if($line =~ /invalid users/)
        	{
        	        if($line =~ /\@/)
        	        {
        	                print SMB_W "$line, $gname\n";
        	        }
                        else
                        {
                                print SMB_W "$line $gname\n";
                        }
                        $found = 1;
        	}
        	else
        	{
        	        print SMB_W "$line\n";
        	}
        }      	
        
        close(SMB_W);
        close(SMB_R);
        rename("/etc/samba/smb.conf_tmp", "/etc/samba/smb.conf");        
}
elsif ($func eq "delgroup")
{       
        open(SMB_R, "/etc/samba/smb.conf");
        open(SMB_W, ">/etc/samba/smb.conf_tmp");
        while (<SMB_R>) 
        {
        	my $line = $_;
        	chomp($line);
        	if($line =~ /$gname$/)
        	{
        	        $line =~ s/\,*\s*$gname$//;
        	        $found = 1;
        	}
        	elsif($line =~ /$gname\,/)
        	{
        	        $line =~ s/$gname\,\s*//;
        	        $found = 1;
        	}
        	
        	print SMB_W "$line\n";
        }      	
        
        close(SMB_W);
        close(SMB_R);
        rename("/etc/samba/smb.conf_tmp", "/etc/samba/smb.conf");        
}

if($found == 1)
{
        system("service smb restart &>/dev/null");
}
