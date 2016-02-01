#!/bin/sh

workspaces=$(i3-msg -t get_workspaces)
script="
import json,sys
r=json.load(sys.stdin);
if r[1]['visible']:
	print 1
else:
	print 0"

echo $workspaces | python2 -c "$script"
