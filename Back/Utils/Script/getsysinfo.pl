#!/usr/bin/perl

my ($fan,$case_fan,$cpu_fan,$aux_fan,$sys_temp,$cpu_temp);
my ($fan_min,$case_minfan,$cpu_minfan,$aux_minfan,$sys_maxtemp,$cpu_maxtemp);

my $func = $ARGV[0];
if($func eq 'sensors')
{
        open(SENSOR, "sensors 2>/dev/null |");
        while (<SENSOR>) 
        {
                my $line = $_;
                chomp($line);
                #Case Fan: 3308 RPM  (min =  712 RPM, div = 8)
                if($line =~ /\s*Case Fan:\s*(\d+)\s*RPM\D+(\d+)/)
                {
                        $case_fan = $1;
                        $case_minfan = $2;
                }
                #CPU Fan:     0 RPM  (min =  727 RPM, div = 64) ALARM
                elsif($line =~ /\s*CPU Fan:\s*(\d+)\s*RPM\D+(\d+)/)
                {
                        $cpu_fan = $1;
                        $cpu_minfan = $2;
                }
                #Aux Fan:  3375 RPM  (min =  712 RPM, div = 8)
                elsif($line =~ /\s*Aux Fan:\s*(\d+)\s*RPM\D+(\d+)/)
                {
                        $aux_fan = $1;
                        $aux_minfan = $2;
                }
                #Sys Temp:    +32°„C  (high =   +60°„C, hyst =   +55°„C)  [thermistor]
                elsif($line =~ /\s*Sys Temp:\s*[\+|\-]*(\S+)\SC\s*[^+|^-]+[\+|\-]*(\S+)\SC/)
                {
                        $sys_temp = $1;
                        $sys_maxtemp = $2;
                }
                #CPU Temp:  +25.5°„C  (high = +95.0°„C, hyst = +92.0°„C)  [CPU diode ]
                elsif($line =~ /\s*CPU Temp:\s*[\+|\-]*(\S+)\SC\s*[^+|^-]+[\+|\-]*(\S+)\SC/)
                {
                        $cpu_temp = $1;
                        $cpu_maxtemp = $2;
                }
        }
        close(SENSOR);
        if($cpu_fan != 0)
        {
                $fan = $cpu_fan;
                $fan_min = $cpu_minfan;
        }
        elsif($case_fan != 0 )
        {
                $fan = $case_fan;
                $fan_min = $case_minfan;
        }
        elsif($aux_fan != 0 )
        {
                $fan = $aux_fan;
                $fan_min = $aux_minfan;
        }
        
        print"$fan:$fan_min:$sys_temp:$sys_maxtemp:$cpu_temp:$cpu_maxtemp\n";

}
elsif($func eq 'netdev')
{
        my %inferfaces;
        open(INTERF,"ifconfig -a | grep '^eth' |");
        while (<INTERF>) 
        {
                my $line = $_;
                chomp($line);
                if($line =~ /^(\S+)\s*/)
                {
                        my $interface = $1;
                        $inferfaces{$interface} = 0;
                        open(UP_DOWN,"ifconfig $interface | grep RUNNING |");
                        while(<UP_DOWN>)
                        {
                                my $line2 = $_;
                                chomp($line2);
                                if($line2 =~ /RUNNING/)
                                {
                                        $inferfaces{$interface} = 1;
                                }
                        }
                        close(UP_DOWN)
                }
        }
        close(INTERF);
        my $all_info;
        foreach my $intf(keys %inferfaces)
        {       
                $all_info .= "$intf:$inferfaces{$intf};";
        }
        $all_info =~ s/;$//;
        
        print"$all_info\n";
}