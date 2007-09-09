#!/bin/sh

tdir=/tmp/vnc-$$
mkdir $tdir
mkdir -p NEW DIFF

../xorg-server/hw/vnc/Xvnc --SecurityTypes=none :30 &
xvncpid=$!
sleep 3

xterm -display :30 -e 'echo started >'$tdir'/started.txt; exit' 

xterm -display :30 -e 'xdpyinfo >'$tdir'/xdpyinfo.txt 2>&1; exit' 
xterm -display :30 -e 'xrandr >'$tdir'/xrandr.txt 2>&1; exit' 

kill $xvncpid

cp $tdir/xdpyinfo.txt NEW/01-xdpyinfo-out.txt
if diff $tdir/xdpyinfo.txt 01-xdpyinfo-out.txt >DIFF/01-xdpyinfo-out.txt
then
	:
else	
	echo FAILED on dpyinfo
	exit 1
fi

cp $tdir/xrandr.txt   NEW/01-xrandr-out.txt
if diff $tdir/xrandr.txt 01-xrandr-out.txt >DIFF/01-xrandr-out.txt
then
	echo PASSED.
else
	echo FAILED on xrandr
	exit 1
fi

rm -rf $tdir

