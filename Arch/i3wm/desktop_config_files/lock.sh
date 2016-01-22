#!/bin/bash

TMPBG='/tmp/screen.png'
logo='/home/mind/.config/i3/pictures/mandala.png'
scrot $TMPBG
convert $TMPBG -scale 10% -scale 1000% $TMPBG
#convert $TMPBG $logo -gravity center -composite -matte $TMPBG

IMG=$(file $logo | grep -o '[0-9]* x [0-9]*')
IMGX=$(echo $IMG | cut -d' ' -f 1)
IMGY=$(echo $IMG | cut -d' ' -f 3)

SR=$(xrandr --query | grep ' connected' | cut -f3 -d' ')

for RES in $SR
do
	SCRX=$(echo $RES | cut -f 1 -d'x')
	SCRY=$(echo $RES | cut -f 2 -d 'x' | cut -f 1 -d '+')
	SCROX=$(echo $RES | cut -f 2 -d'x' | cut -f 2 -d '+')
	SCROY=$(echo $RES | cut -f 2 -d'x' | cut -f 3 -d '+')
	
	PX=$(($SCROX + $SCRX/2 - $IMGX/2))
	PY=$(($SCROY + $SCRY/2 - $IMGY/2))
	
	convert $TMPBG $logo -geometry +$PX+$PY -composite -matte $TMPBG
done

i3lock -i $TMPBG
