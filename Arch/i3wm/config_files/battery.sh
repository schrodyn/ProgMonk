#!/bin/sh

charge_curr=$(cat /sys/class/power_supply/BAT1/charge_now)
charge_full=$(cat  /sys/class/power_supply/BAT1/charge_full)

procent=$(echo "$charge_curr $charge_full" | awk '{printf "%.2f", $1*100/$2 }')

power_supply_current_now=$(cat /sys/class/power_supply/BAT1/current_now)
power_supply_charge_now=$(cat /sys/class/power_supply/BAT1/charge_now)

minutes=$(echo "$power_supply_current_now $power_supply_charge_now" | awk '{printf "%d", $2*60/$1}')

rem_time=$(echo $minutes | awk '{ printf "%d:%02d", $1 / 60, $1 % 60}')

echo "BAT $procent% $rem_time"


