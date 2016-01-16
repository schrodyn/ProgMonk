#!/bin/bash

TMPBG='/tmp/screen.png'
scrot $TMPBG
convert $TMPBG -scale 10% -scale 1000% $TMPBG
i3lock -i $TMPBG
