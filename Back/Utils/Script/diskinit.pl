#!/usr/bin/perl


# 读取 /proc/partitions 信息，实现查找相关的块设备信息
local (@devs, $d);
if (open(PARTS, "/proc/partitions")) {
	# The list of all disks can come from the kernel
	local $sc = 0;
	while(<PARTS>) {
		if ((/\d+\s+\d+\s+\d+\s+sd(\S)\s/ || /\d+\s+\d+\s+\d+\s+sd(\S\D)\s/) ||
		    /\d+\s+\d+\s+\d+\s+(scsi\/host(\d+)\/bus(\d+)\/target(\d+)\/lun(\d+)\/disc)\s+/) {
			# New or old style SCSI device
			local $d = $1;
			local ($host, $bus, $target, $lun) = ($2, $3, $4, $5);
			if (!$dscsi_mode && $pscsi[$sc] =~ /USB-FDU/) {
				# USB floppy with scsi emulation!
				splice(@pscsi, $sc, 1);
				next;
			}

			if ($host ne '') {
				local $scsidev = "/dev/$d";
				if (!-r $scsidev) {
					push(@devs, "/dev/sd".chr(97+$sc));
				}

				else {
					push(@devs, $scsidev);
				}
			}

			else {
				push(@devs, "/dev/sd$d");
			}

			$sc++;
		}
		elsif (/\d+\s+\d+\s+\d+\s+hd(\S)\s/) {
			# IDE disk (but skip CDs)
			local $n = $1;
			if (open(MEDIA, "/proc/ide/hd$n/media")) {
				local $media = <MEDIA>;
				close(MEDIA);
				if ($media =~ /^disk/ && !$_[0]) {
					push(@devs, "/dev/hd$n");
				}
				if (-d "/dev/i2o") {
					push(@devs, "/dev/i2o/hd$n");
				}
			}
		}
		elsif (/\d+\s+\d+\s+\d+\s+(ide\/host(\d+)\/bus(\d+)\/target(\d+)\/lun(\d+)\/disc)\s+/) {
			# New-style IDE disk
			local $idedev = "/dev/$1";
			local ($host, $bus, $target, $lun) = ($2, $3, $4, $5);
			if (!-r $idedev) {
				push(@devs, "/dev/hd".chr(97 + $target +
							       $bus*2 +
							       $host*4));
				}
			else {
				push(@devs, "/dev/$1");
			}
		}
		elsif (/\d+\s+\d+\s+\d+\s+xvd(\S)\s/) {
		  # Xen Virtual disk
		  local $d = $1;
		  push(@devs, "/dev/xvd$d");
		}
		elsif (/\d+\s+\d+\s+\d+\s+(rd\/c(\d+)d\d+)\s/) {
			# Mylex raid device
			push(@devs, "/dev/$1");
		}
		elsif (/\d+\s+\d+\s+\d+\s+(ida\/c(\d+)d\d+)\s/) {
			# Compaq raid device
			push(@devs, "/dev/$1");
		}

		elsif (/\d+\s+\d+\s+\d+\s+(cciss\/c(\d+)d\d+)\s/) {
			# Compaq Smart Array RAID
			push(@devs, "/dev/$1");
		}
		elsif (/\d+\s+\d+\s+\d+\s+(ataraid\/disc(\d+)\/disc)\s+/) {
			# Promise raid controller
			push(@devs, "/dev/$1");
		}
	}
	close(PARTS);

	@devs = sort { ($b =~ /\/hd[a-z]$/ ? 1 : 0) <=>
		       ($a =~ /\/hd[a-z]$/ ? 1 : 0) } @devs;
}

# AOE changes: J Landman 26-Sept-2006 landman at scalableinformatics.com
# 查找 AOE设备
if (-e '/usr/sbin/aoe-stat') {
  my $rc=`/usr/sbin/aoe-discover &>/dev/null; sleep 1`;
  my @aoe_volumes = `/usr/sbin/aoe-stat`;
  chomp(@aoe_volumes);
  foreach (@aoe_volumes) {
     #printf "Found aoe volume: %s\n",$_;
     my @vols = split(/\s+/,$_);
     #printf "Found volume: %s\n",$vols[1];
     push @devs,(sprintf "/dev/etherd/%s",$vols[1]);
  }
}

#刷新设备，感觉没什么用处，可能有什么特殊用法吧！
#open(LINES, "partprobe -d -s 2>/dev/null | grep sd | cut -f1 -d \":\" | cut -f3 -d \"/\"  |");
#local @lines = <LINES>;
#close(LINES);
#local @pprobescsi;
#foreach  (@lines)
#{
#	if( /sd(\S)\s/ || /sd(\S\D)\s/)
#	{
#		push (@pprobescsi , "/dev/sd$1");
#	}
#}

local @cdstat = stat("/dev/cdrom");
if (@cdstat && !$_[0]) {
	@devs = grep { (stat($_))[1] != $cdstat[1] } @devs;
}

# 未明白完全的作用
#local (@disks);
#foreach $d (@devs) {
#	open(FDISK, "LANG=C parted -s $d unit cyl print 2>/dev/null |");
#	local ($disk);
#	my $found              = 0;
#	while(<FDISK>){
#		if(/^Model:/)
#		{
#			$found = 1;
#			break;
#		}
#	}
#	if($found == 0)
#	{
#		system("parted -s $d mklabel &>/dev/null");
#	}
#}

# 是否是pv
open(PVS, "LANG=C pvs --noheading --separator \',' -o pv_name |");
my $pvlist;
while (<PVS>) {
	#printf "++<$_>++\n";
	$pvlist = $pvlist . $_ ;
}

# 输出的格式 @devs = ('/dev/sda', '/dev/sdb', '/dev/sdc');

my $disk = "volume";

IHATEPERL: foreach $d (@devs) {
	# skip devices held by device mapper
	# TODO: handle other device types (e.g cciss)
	#system("parted -s $d mklabel &>/dev/null");
	$mydev = substr $d, 5;
	$holders_dir = "/sys/block/" . $mydev . "/holders";
	if ( -d $holders_dir ) {
		if (opendir DH, $holders_dir) {
			@holders = readdir DH;
			@foundHolder = grep(/dm-/, @holders);
			if (scalar(@foundHolder) > 0) {
				next IHATEPERL;
			}
		}
	}

	# 获取 VG 名字中最大的值
	my $num	= 0;

	open(PVS, "LANG=C vgs --noheading --separator \',\' -o vg_name | ");
	while (<PVS>) {
		if (/$disk(\d+)/) {
			if ($num < $1) {
				$num = $1;
			}
		}
	}

	$num = $num + 1;

	if ($pvlist =~ /\s*$d\s*/) {}
	else {

		if ($num >= 10) {
			#print "vgcreate $disk","$num $d > /dev/null","\n";
			system "pvcreate $d > /dev/null";
			$rt = system "vgcreate $disk"."$num $d > /dev/null";
		}
		else {
			#print  "vgcreate $disk","0$num $d > /dev/null","\n";
			system "pvcreate $d > /dev/null";
			$rt = system "vgcreate $disk"."0$num $d > /dev/null";
		}
	}
}
