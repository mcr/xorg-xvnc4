#!/bin/sh

tdir=/tmp/vnc-$$
mkdir $tdir
mkdir -p NEW DIFF

../xorg-server/hw/vnc/Xvnc --SecurityTypes=none :30 &
xvncpid=$!
sleep 3

xterm -display :30 -e 'echo started >'$tdir'/started.txt; exit' 

DISPLAY=:30 export DISPLAY

xdpyinfo -display :30 >NEW/01-xdpyinfo-out.txt 2>&1
xrandr >NEW/01-xrandr-out.txt 2>&1

kill $xvncpid

if diff NEW/01-xdpyinfo-out.txt 01-xdpyinfo-out.txt >DIFF/01-xdpyinfo-out.txt
then
	:
else	
	echo FAILED on dpyinfo
	exit 1
fi

if diff NEW/01-xrandr-out.txt 01-xrandr-out.txt >DIFF/01-xrandr-out.txt
then
	echo PASSED.
else
	echo FAILED on xrandr
	exit 1
fi

rm -rf $tdir

