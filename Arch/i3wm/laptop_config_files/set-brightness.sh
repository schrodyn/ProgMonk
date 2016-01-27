#!/bin/sh

bright_path="/sys/class/backlight/radeon_bl0/brightness"

chmod 666 $bright_path
echo 155 > $bright_path
