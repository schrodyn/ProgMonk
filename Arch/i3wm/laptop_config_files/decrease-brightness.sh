#!/bin/bash

bright_path="/sys/class/backlight/radeon_bl0/brightness"

current=$(cat $bright_path)
echo $((current-25)) > $bright_path
