#!/bin/sh

if [ $# -eq 0 ]
then
	echo "usage: ssid-scan [device]"
	exit 1
fi

dev=$1

ip link set $dev up
ssid_info=$(iw dev $dev scan | grep "SSID\|Auth\|signal")

echo "${ssid_info}"
