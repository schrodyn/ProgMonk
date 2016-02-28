#!/bin/sh

default_bright=105
lower_bright=42

check=$(get-lower-workspace.sh)

if [ $check -eq 1 ]
then
	set-brightness-param.sh $lower_bright
else
	set-brightness-param.sh $default_bright
fi
