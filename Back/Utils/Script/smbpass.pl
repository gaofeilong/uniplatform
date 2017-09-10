#!/bin/sh

smbpasswd -s -a $1 << DD
$2
$2
DD
    
