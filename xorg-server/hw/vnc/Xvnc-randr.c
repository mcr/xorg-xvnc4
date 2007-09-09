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

static Bool
vncRandRGetInfo (ScreenPtr pScreen,
		 Rotation  *rotations)
{
    RRScreenSizePtr pSize;

    *rotations = RR_Rotate_0;

    pSize = RRRegisterSize (pScreen,
			    pScreen->width,
			    pScreen->height,
			    pScreen->mmWidth,
			    pScreen->mmHeight);
    
    RRRegisterRate (pScreen, pSize, DEFAULT_REFRESH_RATE);
    RRSetCurrentConfig (pScreen, RR_Rotate_0, DEFAULT_REFRESH_RATE, pSize);

    return TRUE;
}

static Bool
vncRandRSetConfig (ScreenPtr	    pScreen,
		    Rotation	    rotations,
		    int		    rate,
		    RRScreenSizePtr pSize)
{
	printf("trying to set size=%u x %u\n", pSize->width, pSize->height);
	return FALSE;
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
