/* Copyright (C) 2007 XDS Canada Inc.  All Rights Reserved.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <X11/cursorfont.h>
#include <X11/XWDFile.h>

#include "xorg-config.h"
#include "inputstr.h"
#include "cursorstr.h"
#include "mipointer.h"
#include "scrnintstr.h"
#include "vnc.h"

#ifdef RANDR
#include "randrstr.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#ifdef RANDR

#define DEFAULT_REFRESH_RATE 50

struct sizes {
	unsigned int x,y;
};

/*
 * NOTES:
 *   - a 1024x768 websnap is 1004 x 744 desktop, so 20 x 24 smaller.
 *
 */
int stock_size_reduce_X[]= {20, 16, 8, 0};
int stock_size_reduce_Y[]= {24, 16, 8, 0};
#define STOCK_SIZE_COUNT (sizeof(stock_size_reduce_X)/sizeof(int))

struct sizes stock_sizes[]={
	{ .x =  640, .y =  480 },
	{ .x =  720, .y =  400 },
	{ .x =  800, .y =  600 },
	{ .x =  832, .y =  624 },
	{ .x = 1024, .y =  768 },
	{ .x=  1152, .y =  768 },
	{ .x=  1152, .y =  864 },
	{ .x = 1152, .y =  900 },
	{ .x = 1280, .y =  768 },
	{ .x = 1280, .y =  800 },
	{ .x = 1280, .y =  960 },
	{ .x = 1280, .y = 1024 },
	{ .x = 1280, .y = 1024 },
	{ .x = 1400, .y = 1050 }, 
};


static void vncRandRGetOneInfo(ScreenPtr pScreen,
			       Bool selected,
			       unsigned int x,   unsigned int y,
			       unsigned int mmx, unsigned int mmy)
{
	RRScreenSizePtr pSize;

	/* start by creating one for the actual size */
	pSize = RRRegisterSize (pScreen,
				x,y, mmx, mmy);

	RRRegisterRate (pScreen, pSize, DEFAULT_REFRESH_RATE);

	if(selected) {
		/* tells it which one is current! */
		RRSetCurrentConfig (pScreen, RR_Rotate_0,
				    DEFAULT_REFRESH_RATE, pSize);
	}
}
			  
static Bool
vncRandRGetInfo (ScreenPtr pScreen,
		 Rotation  *rotations)
{
	int i, randrCount, totalpixels;
	struct sizes *s;
	vfbScreenInfo *vfb0;

	*rotations = RR_Rotate_0;
	vfb0 = vfbFirstScreen();
	randrCount = 0;
	totalpixels = vfb0->maxHeight * vfb0->maxWidth;

	vncRandRGetOneInfo(pScreen, vfb0->randrSelected == randrCount,
			   vfb0->maxWidth, vfb0->maxHeight,
			   pScreen->mmWidth, pScreen->mmHeight);

	for(i = (sizeof(stock_sizes)/sizeof(struct sizes))-1;
	    i>=0;
	    i--) {
		int nx, ny;
		int reduced;

		s = &stock_sizes[i];
		nx = s->x;
		ny = s->y;

		if((nx * ny) > totalpixels) continue;

		/* create screens which are smaller by a bit */
		for(reduced=STOCK_SIZE_COUNT-1; reduced >= 0; reduced--) {
			nx = s->x - stock_size_reduce_X[reduced];
			ny = s->y - stock_size_reduce_Y[reduced];

			if(nx == vfb0->maxWidth
			   && ny == vfb0->maxHeight) continue;

			randrCount++;
			vncRandRGetOneInfo(pScreen,
					   vfb0->randrSelected == randrCount,
					   nx, ny,
					   pScreen->mmWidth,
					   pScreen->mmHeight);

		} 
	}

	return TRUE;
}

static Bool
vncRandRSetConfig (ScreenPtr	    pScreen,
		    Rotation	    rotations,
		    int		    rate,
		    RRScreenSizePtr pSize)
{
	vfbScreenInfo *vfb0;
	int h,w;

	vfb0 = vfbFirstScreen();

	h= pSize->height;
	w= pSize->width;
	if( h > vfb0->maxHeight) h = vfb0->maxHeight;
	if( w > vfb0->maxWidth)  w = vfb0->maxWidth;

	vfb0->height = h;
	vfb0->width  = w;
	vfb0->randrSelected = pSize->id;

	pScreen->width =w;
	pScreen->height=h;

	vncResizeServer(0);

	fprintf(stderr, "resized to %ux%u\n", w,h);
	return TRUE;
}

Bool vncRandRInit (ScreenPtr pScreen)
{
    rrScrPrivPtr pScrPriv;

    if (!RRScreenInit (pScreen))
	return FALSE;

    pScrPriv = rrGetScrPriv (pScreen);
    pScrPriv->rrGetInfo   = vncRandRGetInfo;
    pScrPriv->rrSetConfig = vncRandRSetConfig;

    return TRUE;
}

#endif
