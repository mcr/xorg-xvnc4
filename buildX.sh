#!/bin/sh -e
echo "***************** START Xvnc BUILD ************************************"
if [ ! -d buildtemp ] ; then
    mkdir buildtemp
    cd buildtemp
    echo "*** Copy VNC xc source dir"
    cp -a ../unix/xc .
    cp -a ../unix/vncconfig .
    cp -a ../common/rfb .
    cp -a ../common/rdr .
    cp -a ../common/network .
    cp -a ../common/Xregion .
    cp -a ../unix/depend.mk .
    cp -a ../unix/Makefile .
    cp -a ../unix/tx .
    cp -a ../unix/vncmkdepend .
#    echo "*** Patching X source"
#    patch -Np0 < ../xc.patch
#    patch -p1 < ../xsrc/xccompilefix.patch
#    patch -p1 < ../xsrc/vncsofpicfix.patch
else
    cd buildtemp
fi
cd xc
echo "*** Build X source"
make World
#echo "" > config/cf/version.def
#make Makefile.boot
#make Makefiles
#make includes
#make depend
#cd config/util
#make
#cd ../../programs/Xserver/vnc
#make
## Now vnc.o is created
#cd ..
#make Xvnc
## Now Xvnc should be created
