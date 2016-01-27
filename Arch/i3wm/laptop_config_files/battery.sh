#!/bin/sh

energy_curr=$(cat /sys/class/power_supply/BAT1/energy_now)
energy_full=$(cat  /sys/class/power_supply/BAT1/energy_full)

procent=$(echo "$energy_curr $energy_full" | awk '{printf "%.2f", $1*100/$2 }')

power_supply_current_now=$(cat /sys/class/power_supply/BAT1/power_now)
power_supply_energy_now=$(cat /sys/class/power_supply/BAT1/energy_now)

minutes=$(echo "$power_supply_current_now $power_supply_energy_now" | awk '{printf "%d", $2*60/$1}')

rem_time=$(echo $minutes | awk '{ printf "%d:%02d", $1 / 60, $1 % 60}')

echo "BAT $procent% $rem_time"


