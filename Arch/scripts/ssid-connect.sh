#!/bin/sh

usage(){
cat << EOF
usage: $0 -i interface -s SSID [-p PASSWD]

This script connect to the specified SSID

OPTIONS:
  -h	Show this message
  -i	Interface (ex. wlp2s0)
  -s	Destination SSID
  -p	Password
EOF
}

INTERFACE=
SSID=
PASSWD=

while getopts ":hi:s:p:" OPTION
do
	case $OPTION in
		h)
			usage
			exit 1
			;;

		i)
			INTERFACE=$OPTARG
			;;
		s)
			SSID=$OPTARG
			;;
		p)
			PASSWD=$OPTARG
			;;
		?)
			echo "Invalid option: -$OPTARG">&2
			usage
			exit 1
			;;

		:)
			echo "Option -$OPTARG requires an argument.">&2
			exit 1
			;;
	esac
done

if [[ -z $INTERFACE ]] || [[ -z $SSID ]]
then
	usage
	exit 1
fi

config_file="$SSID-dynamic-config"

cat << EOF > $config_file
Interface=$INTERFACE
Connection=wireless
ESSID='$SSID'
EOF

if [ -n $PASSWD ]
then
	echo "Security=wpa" >> $config_file
	echo "Key='$PASSWD'" >> $config_file
else
	echo "Security=none" >> $config_file
fi

echo "IP=dhcp" >> $config_file

mv "$config_file" "/etc/netctl/"
