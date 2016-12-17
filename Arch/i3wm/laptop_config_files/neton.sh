#!/bin/sh

if [ $# -lt 1 ]
then
	echo "$0 [config]"
	exit -1
fi

CFG_DIR="/etc/neton"
CONFIG="$CFG_DIR/$1"

DEV=""
WPA_CFG=""
ADDR=""
GW=""
IP=""

while read line
do
	#split line on two components using internal field separator (IFS)
	IFS==; read -a args <<< $line;
	arg="${args[0]}"
	value="${args[1]}"
	
	case $arg in
		dev)
			DEV=$value
			;;
		wpa_conf)
			WPA_CFG=$value
			;;
		addr)
			ADDR=$value
			;;
		gw)
			GW=$value
			;;
		ip)
			IP=$value
			;;
	esac
done <$CONFIG

if [[ -z $DEV ]]; then
	echo "dev option missed!"
	exit -1
fi

if [[ -z $WPA_CFG ]]; then
	echo "wpa_cfg option missed!"
	exit -1
fi

if [[ -z $IP ]]; then
	echo "ip option missed!"
	exit -1
fi

case $IP in
	static)
		if [ -z $ADDR ]; then
			echo "addr option missed!"
		fi
		if [ -z $GW ]; then
			echo "gw option missed!"
		fi
		;;
	dynamic)
		;;
	*)
		echo "ip option wrong!"
		exit -1
		;;
esac

sudo ip link set dev $DEV up

sudo wpa_supplicant -i $DEV -D nl80211 -c $WPA_CFG & 

if [ $IP == "static" ]; then
	sudo ip addr add $ADDR dev $DEV 
	sudo ip route add default via $GW
else
	sudo dhcpcd $DEV
fi




