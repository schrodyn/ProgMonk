#!/bin/sh

default_bright=105
lower_bright=42

check=$(check-second-workspace.sh)

if [ $check -eq 0  ]
then
	set-brightness-param.sh $default_bright
else
	set-brightness-param.sh $lower_bright
fi
