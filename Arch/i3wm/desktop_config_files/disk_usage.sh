#!/bin/sh

partition=$1
space_usage=$(df -m | grep $partition)

free_space=$(echo $space_usage | awk '{print $4}')

free_gb=$(echo $free_space | awk '{ printf "%.2f", $1/1024 }')

echo "$free_gb GiB"
