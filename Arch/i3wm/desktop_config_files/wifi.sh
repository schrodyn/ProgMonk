#!/bin/sh

device="$1"

ipaddr=$(ip addr show $device | grep '^.*inet.*brd' | awk '{print $2}' | sed 's/\/.*$//')

#ssid=$(iw dev $device link | grep SSID | sed 's/^.*SSID: //')

signal=$(iwconfig $device| grep Signal | awk '{ printf "%s dBm", $4 }' | sed 's/^.*level=//')

if [ -z $signal ] 
then 
 echo "W: down"
else
 echo "W: $ipaddr ($signal)"
fi
