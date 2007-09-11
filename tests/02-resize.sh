#!/bin/sh

tdir=/tmp/vnc-$$
mkdir $tdir
mkdir -p NEW DIFF

../xorg-server/hw/vnc/Xvnc --SecurityTypes=none :30 &
xvncpid=$!
sleep 3

export DISPLAY=:30

xrandr -s 4 >NEW/02-xrandr-set.txt 2>&1
xrandr -q   >NEW/02-xrandr-get.txt 2>&1
xdpyinfo    >NEW/02-xdpyinfo.txt 2>&1

kill $xvncpid

if diff NEW/02-xrandr-set.txt 02-xrandr-set.txt >DIFF/02-xrandr-set.txt
then :; else echo FAILED on set; exit 1; fi

if diff NEW/02-xdpyinfo.txt 02-xdpyinfo.txt >DIFF/02-xdpyinfo.txt
then :; else echo FAILED on dpy; exit 1; fi

if diff NEW/02-xrandr-get.txt 02-xrandr-get.txt >DIFF/02-xrandr-get.txt
then
	echo PASSED.
else
	echo FAILED on get
	exit 1
fi

rm -rf $tdir

