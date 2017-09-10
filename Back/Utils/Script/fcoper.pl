#!/usr/bin/perl

$path = $ARGV[0];

if ($path eq "") {exit(-1);}

if (!open(SCST, "$path")) {
        exit(-1);
}

my %device;
my $temp;
my %target;

while(<SCST>) {
        if (/DEVICE (\w+)/) {
                $temp = $1;
        }
        if (/filename ([\w\/]+)/) {
                $device{"$temp"} = $1;
        }
        if (/TARGET\s+([\w\:]+)/) {
                $target{$1} = "";
                $temp = $1;
        }
        if (/LUN\s+(\w+)\s+(\w+)/) {
                $target{$temp} .= "$1,$2,$device{$2},";
        }
}

while ( ($a, $b) = each(%target) ) {
        print "$a,$b\n";
}
