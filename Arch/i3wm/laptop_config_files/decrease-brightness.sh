#!/bin/bash

bright_path="/sys/class/backlight/intel_backlight/brightness"

current=$(cat $bright_path)
echo $((current-100)) > $bright_path
