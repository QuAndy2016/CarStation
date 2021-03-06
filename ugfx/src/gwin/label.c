/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file	include/gwin/label.h
 * @brief	GWIN label widget header file.
 *
 * @defgroup Label Label
 * @ingroup GWIN
 *
 * @{
 */

#include "gfx.h"

#if GFX_USE_GWIN && GWIN_NEED_LABEL

#include "gwin/class_gwin.h"

// macros to assist in data type conversions
#define gh2obj					((GLabelObject *)gh)

// flags for the GLabelObject
#define GLABEL_FLG_WAUTO		(GWIN_FIRST_CONTROL_FLAG << 0)
#define GLABEL_FLG_HAUTO		(GWIN_FIRST_CONTROL_FLAG << 1)
#define GLABEL_FLG_BORDER		(GWIN_FIRST_CONTROL_FLAG << 2)

// simple: single line with no wrapping
static coord_t getwidth(const char *text, font_t font, coord_t maxwidth) {
	(void) maxwidth;

	return gdispGetStringWidth(text, font)+2;		// Allow one pixel of padding on each side
}

// simple: single line with no wrapping
static coord_t getheight(const char *text, font_t font, coord_t maxwidth) {
	(void) text;
	(void) maxwidth;

	return gdispGetFontMetric(font, fontHeight);
}

static void gwinLabelDefaultDraw(GWidgetObject *gw, void *param) {
	coord_t				w, h;
	(void)				param;

	w = (gw->g.flags & GLABEL_FLG_WAUTO) ? getwidth(gw->text, gw->g.font, gdispGGetWidth(gw->g.display) - gw->g.x) : gw->g.width;
	h = (gw->g.flags & GLABEL_FLG_HAUTO) ? getheight(gw->text, gw->g.font, gdispGGetWidth(gw->g.display) - gw->g.x) : gw->g.height;

	if (gw->g.width != w || gw->g.height != h) {
		gwinResize(&gw->g, w, h);

		return;
	}

	// render the text
	gdispGFillStringBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, gw->text, gw->g.font,
			(gw->g.flags & GWIN_FLG_ENABLED) ? gw->pstyle->enabled.text : gw->pstyle->disabled.text, gw->pstyle->background,
			justifyLeft);

	// render the border (if any)
	if (gw->g.flags & GLABEL_FLG_BORDER)
		gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, (gw->g.flags & GWIN_FLG_ENABLED) ? gw->pstyle->enabled.edge : gw->pstyle->disabled.edge);
}

static const gwidgetVMT labelVMT = {
	{
		"Label",				// The class name
		sizeof(GLabelObject),	// The object size
		_gwidgetDestroy,		// The destroy routine
		_gwidgetRedraw, 		// The redraw routine
		0,						// The after-clear routine
	},
	gwinLabelDefaultDraw,		// default drawing routine
	#if GINPUT_NEED_MOUSE
		{
			0,						// Process mose down events (NOT USED)
			0,						// Process mouse up events (NOT USED)
			0,						// Process mouse move events (NOT USED)
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			0,						// No toggle role
			0,						// Assign Toggles (NOT USED)
			0,						// Get Toggles (NOT USED)
			0,						// Process toggle off event (NOT USED)
			0,						// Process toggle on event (NOT USED)
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,						// No dial roles
			0,						// Assign Dials (NOT USED)
			0, 						// Get Dials (NOT USED)
			0,						// Procees dial move events (NOT USED)
		},
	#endif
};

GHandle gwinGLabelCreate(GDisplay *g, GLabelObject *widget, GWidgetInit *pInit) {
	uint16_t flags = 0;

	// auto assign width
	if (pInit->g.width <= 0) {

		flags |= GLABEL_FLG_WAUTO;
		pInit->g.width = getwidth(pInit->text, gwinGetDefaultFont(), gdispGGetWidth(g) - pInit->g.x);
	}
 
	// auto assign height
	if (pInit->g.height <= 0) {
		flags |= GLABEL_FLG_HAUTO;
		pInit->g.height = getheight(pInit->text, gwinGetDefaultFont(), gdispGGetWidth(g) - pInit->g.x);
	}

	if (!(widget = (GLabelObject *)_gwidgetCreate(g, &widget->w, pInit, &labelVMT)))
		return 0;

	// no borders by default
	flags &=~ GLABEL_FLG_BORDER;

	widget->w.g.flags |= flags;	
	gwinSetVisible(&widget->w.g, pInit->g.show);

	return (GHandle)widget;
}

void gwinLabelSetBorder(GHandle gh, bool_t border) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&labelVMT)
		return;
	
	if (border)
		gh2obj->w.g.flags |= GLABEL_FLG_BORDER;
	else
		gh2obj->w.g.flags &=~ GLABEL_FLG_BORDER;
}

#endif // GFX_USE_GWIN && GFX_NEED_LABEL
/** @} */

