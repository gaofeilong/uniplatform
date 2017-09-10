#!/usr/bin/perl

my $func = $ARGV[0];
my $unit_id = $ARGV[1];
my $diskpre = "/dev/sd";
my (%partition);
if (open(PARTS, "/proc/partitions")) {
	while(<PARTS>) {
	        my $line = $_;
	        chomp($line);	        
	        if($line =~ /sd(\w)(\d+)\s*$/)
	        {
	                my $curdisk = $diskpre.$1;
	                my $curpartition = "sd".$1.$2;
	                
	                if(defined $partition{$curdisk})
	                {
	                        $partition{$curdisk} = $partition{$curdisk}.",".$curpartition;
	                }
	                else
	                {
	                        $partition{$curdisk} = $curpartition;
	                }
	        }	
        }
        close(PARTS);
}

my $unit_disk = $diskpre.chr(ord(a)+$unit_id);

if($func eq 'chkdelraid')
{        
        if(defined $partition{$unit_disk})
        {
                print "false\n";
                exit(0);
        }
        print "true\n";
}
