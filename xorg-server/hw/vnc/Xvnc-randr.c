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

#ifdef RANDR
#include "randrstr.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#ifdef RANDR

static Bool
vndRandRGetInfo (ScreenPtr pScreen,
		 Rotation  *rotations)
{
    RRScreenSizePtr pSize;

    *rotations = RR_Rotate_0;

#if 0
    if (randrExtension)
    {
	XRRScreenConfiguration *xconfig;
	XRRScreenSize	       *sizes;
	int		       nSizes, currentSize = 0;
	short		       *rates, currentRate;
	int		       nRates, i, j;

	XGLX_SCREEN_PRIV (pScreen);

	xconfig	    = XRRGetScreenInfo (xdisplay, pScreenPriv->root);
	sizes	    = XRRConfigSizes (xconfig, &nSizes);
	currentRate = XRRConfigCurrentRate (xconfig);

	if (pScreenPriv->fullscreen)
	{
	    Rotation rotation;

	    currentSize = XRRConfigCurrentConfiguration (xconfig, &rotation);

	    for (i = 0; i < nSizes; i++)
	    {
		pSize = RRRegisterSize (pScreen,
					sizes[i].width,
					sizes[i].height,
					sizes[i].mwidth,
					sizes[i].mheight);

		rates = XRRConfigRates (xconfig, i, &nRates);

		for (j = 0; j < nRates; j++)
		{
		    RRRegisterRate (pScreen, pSize, rates[j]);

		    if (i == currentSize && rates[j] == currentRate)
			RRSetCurrentConfig (pScreen, RR_Rotate_0, currentRate,
					    pSize);
		}
	    }
	}
	else
	{
	    pSize = RRRegisterSize (pScreen,
				    pScreen->width,
				    pScreen->height,
				    pScreen->mmWidth,
				    pScreen->mmHeight);

	    for (i = 0; i < nSizes; i++)
	    {
		rates = XRRConfigRates (xconfig, i, &nRates);

		for (j = 0; j < nRates; j++)
		{
		    RRRegisterRate (pScreen, pSize, rates[j]);

		    if (rates[j] == currentRate)
			RRSetCurrentConfig (pScreen, RR_Rotate_0, currentRate,
					    pSize);
		}
	    }
	}

	XRRFreeScreenConfigInfo (xconfig);
    }
    else
    {
	pSize = RRRegisterSize (pScreen,
				pScreen->width,
				pScreen->height,
				pScreen->mmWidth,
				pScreen->mmHeight);

	RRRegisterRate (pScreen, pSize, DEFAULT_REFRESH_RATE);
	RRSetCurrentConfig (pScreen, RR_Rotate_0, DEFAULT_REFRESH_RATE, pSize);
    }

#endif

    return TRUE;
}

static Bool
vncRandRSetConfig (ScreenPtr	    pScreen,
		    Rotation	    rotations,
		    int		    rate,
		    RRScreenSizePtr pSize)
{
    if (randrExtension)
    {
	XRRScreenConfiguration *xconfig;
	XRRScreenSize	       *sizes;
	int		       nSizes, currentSize;
	int		       i, size = -1;
	int		       status = RRSetConfigFailed;
	Rotation	       rotation;

	XGLX_SCREEN_PRIV (pScreen);

	xconfig	    = XRRGetScreenInfo (xdisplay, pScreenPriv->root);
	sizes	    = XRRConfigSizes (xconfig, &nSizes);
	currentSize = XRRConfigCurrentConfiguration (xconfig, &rotation);

	for (i = 0; i < nSizes; i++)
	{
	    if (pScreenPriv->fullscreen)
	    {
		if (sizes[i].width   == pSize->width   &&
		    sizes[i].height  == pSize->height  &&
		    sizes[i].mwidth  == pSize->mmWidth &&
		    sizes[i].mheight == pSize->mmHeight)
		{
		    size = i;
		    break;
		}
	    }
	    else
	    {
		short *rates;
		int   nRates, j;

		rates = XRRConfigRates (xconfig, i, &nRates);

		for (j = 0; j < nRates; j++)
		{
		    if (rates[j] == rate)
		    {
			size = i;
			if (i >= currentSize)
			    break;
		    }
		}
	    }
	}

	if (size >= 0)
	    status = XRRSetScreenConfigAndRate (xdisplay,
						xconfig,
						pScreenPriv->root,
						size,
						RR_Rotate_0,
						rate,
						CurrentTime);

	XRRFreeScreenConfigInfo (xconfig);

	if (status == RRSetConfigSuccess)
	{
	    PixmapPtr pPixmap;

	    pPixmap = (*pScreen->GetScreenPixmap) (pScreen);

	    if (pScreenPriv->fullscreen)
	    {
		XGL_PIXMAP_PRIV (pPixmap);

		xglSetRootClip (pScreen, FALSE);

		XResizeWindow (xdisplay, pScreenPriv->win,
			       pSize->width, pSize->height);

		glitz_drawable_update_size (pPixmapPriv->drawable,
					    pSize->width, pSize->height);

		pScreen->width    = pSize->width;
		pScreen->height   = pSize->height;
		pScreen->mmWidth  = pSize->mmWidth;
		pScreen->mmHeight = pSize->mmHeight;

		(*pScreen->ModifyPixmapHeader) (pPixmap,
						pScreen->width,
						pScreen->height,
						pPixmap->drawable.depth,
						pPixmap->drawable.bitsPerPixel,
						0, 0);

		xglSetRootClip (pScreen, TRUE);
	    }

	    return TRUE;
	}
    }

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
