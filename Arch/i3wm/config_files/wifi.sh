#!/bin/sh

device="$1"

ipaddr=$(ip addr show $device | grep '^.*inet.*brd' | awk '{print $2}' | sed 's/\/.*$//')

#ssid=$(iw dev $device link | grep SSID | sed 's/^.*SSID: //')

signal=$(iw dev $device link | grep signal | sed 's/^.*signal: //')

if [ -z $signal ] 
then 
 echo "W: down"
else
 echo "W: $ipaddr ($signal)"
fi
