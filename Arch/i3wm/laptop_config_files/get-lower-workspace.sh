#!/bin/sh

workspaces=$(i3-msg -t get_workspaces)
script="
import json,sys
r=json.load(sys.stdin);
for workspace in r:
	if workspace['visible']:
		if workspace['name'].startswith('2:') or \
		   workspace['name'].startswith('5:'):
			print 1
		else:
			print 0
		sys.exit(0)
print 0"

echo $workspaces | python2 -c "$script"
