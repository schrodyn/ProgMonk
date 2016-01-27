#!/bin/bash

TMPBG='/tmp/screen.png'
logo='$HOME/.config/i3/pictures/mandala.png'
scrot $TMPBG
convert $TMPBG -scale 10% -scale 1000% $TMPBG
convert $TMPBG $logo -gravity center -composite -matte $TMPBG
i3lock -i $TMPBG
