#!/bin/bash
for LV in `lvs --unit m --noheading --separator ',' -o lv_name,vg_name,lv_size`
do
   printf $LV,
   MAPPER=`echo $LV | awk -F ',' '{print $2 "-" $1}'`
#   echo $MAPPER
   expr `df -TP /dev/mapper/$MAPPER | grep -v Available | awk -F ' ' '{print $5}'` / 1024
done
