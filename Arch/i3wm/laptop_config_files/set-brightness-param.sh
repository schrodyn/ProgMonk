#!/bin/sh

if [ $# -eq 0 ]
then
    echo "usage: set-brightness-param [brightness_level]"
fi

echo $1 > /sys/class/backlight/radeon_bl0/brightness
