#!/usr/bin/perl

#列出属于该vgname的所有的pv及lv

$vgname = $ARGV[0];
if ($vgname eq '')
{
        exit();
}

local (%listall, %freepv);

$str = "vgs --unit m --noheading --separator ',' -o vg_name,vg_size,vg_free,lv_count $vgname |";

open(VGLIST, $str);

while (<VGLIST>)
{
        if (/\s+([\w]*),([\d.]*)M,([\d.]*)M/) {
                print("VG,$vgname,$2,$3\n");
        }
}

close (VGLIST);

$str = "lvs --unit m --noheading --separator ',' -o lv_name,lv_size,lv_attr $vgname |";

open(LVLIST, $str);

local @lvs;

while (<LVLIST>)
{
        if (/\s+([\w]*),([\d.]+)M,[\w-]{5}([\w-])/)
        {
                local $lv;
                $lv->{'name'} = "/dev/" . $vgname . "/" . $1;
                $lv->{'size'} = $2;
                
                my $str1 = "df -TP | grep $vgname-$1 |";
                open(TP, $str1);
                while(<TP>)
                {
                        if (/\s*[\w\/]*\s+([\w-]+)\s+(\d+)\s+(\d+)\s+(\d+)/) {
                                #print "$1,$2,$3,$4\n";
                                $lv->{'fsize'} = ($3 + $4) / 1024;
                                $lv->{'ffree'} = $4/1024;
                        }
                }

                close(TP);

                push(@lvs, $lv);
        }
}

close (LVLIST);

#格式：
# 名称：大小：空闲：PE大小
# 名称：大小：剩余大小

for ($i = 0; $i < @lvs; $i++) {
        $tmp = $lvs[$i];
        print "$tmp->{'name'},$tmp->{'size'},$tmp->{'fsize'},$tmp->{'ffree'},\n";
}
