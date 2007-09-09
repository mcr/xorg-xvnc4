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

cp $tdir/xdpyinfo.txt NEW/start1-xdpyinfo-out.txt
diff $tdir/xdpyinfo.txt start1-xdpyinfo-out.txt >DIFF/start1-xdpyinfo-out.txt

cp $tdir/xrandr.txt   NEW/start1-xrandr-out.txt
diff $tdir/xrandr.txt start1-xrandr-out.txt >DIFF/start1-xrandr-out.txt

rm -rf $tdir

