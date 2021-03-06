#!/bin/sh

./configure \
                --disable-xvfb \
                --disable-xnest \
                --disable-dependency-tracking \
                --disable-xprint \
                --disable-static \
                --enable-composite \
                --with-rgb-path=/etc/X11/rgb \
                --with-xkb-output=/var/lib/xkb \
                --with-fontdir=/usr/share/fonts/X11 \
                --disable-xorgcfg \
                --disable-xorgconfig \
                --disable-dmx \
                --disable-lbx \
                --enable-xdmcp \
                --enable-randr \
                --enable-xevie \
                --disable-dri \
                --with-int10=stub \
                --prefix=/usr --mandir=/usr/share/man \
                --infodir=/usr/share/info --sysconfdir=/etc \
                --localstatedir=/var 
