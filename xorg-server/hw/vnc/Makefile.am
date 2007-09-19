noinst_LTLIBRARIES = libvnccommon.la
libvnccommon_la_SOURCES = \
		RegionHelper.h     \
		buildtime.c       \
		vncExtInit.cc     \
		vncExtInit.h      \
		vncHooks.cc       \
		vncHooks.h        \
		Xvnc-randr.c      \
		XserverDesktop.cc \
		XserverDesktop.h

libvnccommon_la_CXXFLAGS = \
		-DVENDOR_RELEASE="$(VENDOR_RELEASE)" \
		-DVENDOR_STRING="\"$(VENDOR_STRING)\"" \
		-DGC_HAS_COMPOSITE_CLIP \
		-DVNC_USE_FB \
		-I/usr/include/X11 \
		-I../../mfb \
		-I../../cfb \
		-I../../../common \
		-I../../../unix/vncconfig

bin_PROGRAMS = Xvnc

Xvnc_SOURCES = \
		xvnc.cc \
		$(top_srcdir)/Xext/dpmsstubs.c \
		$(top_srcdir)/Xi/stubs.c \
		$(top_srcdir)/mi/miinitext.c \
		$(top_srcdir)/fb/fbcmap.c

Xvnc_LDADD =	$(XORG_CORE_LIBS) \
		$(XORG_LIBS) \
		$(XSERVER_LIBS) \
		../../fb/libfb.la \
		../../mi/libminimi.la \
		../../render/librender.la \
		../../xkb/libxkbstubs.la \
		../../dbe/libdbe.la \
		../../Xext/libXext.la \
		../../XTrap/libxtrap.la \
		../../record/librecord.la \
		../xfree86/os-support/libxorgos.la \
		../../os/libos.la \
		../xfree86/common/libcommon.la \
		-lX11 \
		libvnccommon.la \
		../../../common/rfb/librfb.a \
		../../../common/rdr/librdr.a \
		../../../common/network/libnetwork.a \
		../../../common/Xregion/libXregion.a

Xvnc_CFLAGS =  -DHAVE_DIX_CONFIG_H \
		-DNO_HW_ONLY_EXTS \
		-DNO_MODULE_EXTS \
		-DXFree86Server -DVNCEXT

Xvnc_CXXFLAGS = $(Xvnc_CFLAGS) \
		-DVENDOR_RELEASE="$(VENDOR_RELEASE)" \
		-DVENDOR_STRING="\"$(VENDOR_STRING)\"" \
		-DVNC_USE_FB \
		-I../../mfb \
		-I../../cfb \
		-I../../../common \
		-I../../../unix/vncconfig \
		-I../../mi \
		-I ../../render \
		-I/usr/include/X11

libvnc_la_LTLIBRARIES = libvnc.la
libvnc_la_LDFLAGS = -module -avoid-version
libvnc_ladir = $(moduledir)/extensions
libvnc_la_SOURCES = xf86vncModule.cc
libvnc_la_LIBADD = \
		libvnccommon.la \
		../../../common/rfb/librfb.a \
		../../../common/rdr/librdr.a \
		../../../common/network/libnetwork.a \
		../../../common/Xregion/libXregion.a
libvnc_la_CXXFLAGS = \
		-I../../../common \
		-I$(top_srcdir)/hw/xfree86/common \
		-I$(top_srcdir)/hw/xfree86/os-support \
		-I$(top_srcdir)/hw/xfree86/os-support/bus \
		-DXFree86Module -DXFree86LOADER -DIN_MODULE

INCLUDES = $(XORG_INCS)

AM_CXXFLAGS =	$(CFLAGS) \
		-DVENDOR_RELEASE="$(VENDOR_RELEASE)" \
		-DVENDOR_STRING="\"$(VENDOR_STRING)\""

