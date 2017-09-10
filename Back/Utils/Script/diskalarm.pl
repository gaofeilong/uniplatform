#!/usr/bin/perl

my $percent = $ARGV[0];

if ($percent > 1 || $percent <= 0) {
        exit(-1);
}
    

open(DISK, "df -P | grep mapper |");
while (<DISK>) {
        #print("$_\n");
        if (/[\d\/-\w]+\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)%\s+([\d\/\w-]+)/) {
                #printf("($1).($2).($3).($4).($5).\n");
                my $tmp = $2 / ($2 + $3);
                #printf("($tmp)lmt($percent) \n");
                if ($tmp >= $percent) {
                        printf("$5,$tmp,$1,$2\n");
                }
        }
}
close(DISK);
