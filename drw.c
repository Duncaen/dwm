/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#include "drw.h"
#include "util.h"

Drw *
drw_create(Display *dpy, int screen, Window root, unsigned int w, unsigned int h)
{
	Drw *drw = ecalloc(1, sizeof(Drw));

	drw->dpy = dpy;
	drw->screen = screen;
	drw->root = root;
	drw->w = w;
	drw->h = h;
	drw->drawable = XCreatePixmap(dpy, root, w, h, DefaultDepth(dpy, screen));
	drw->gc = XCreateGC(dpy, root, 0, NULL);
	XSetLineAttributes(dpy, drw->gc, 1, LineSolid, CapButt, JoinMiter);

	return drw;
}

void
drw_resize(Drw *drw, unsigned int w, unsigned int h)
{
	if (!drw)
		return;

	drw->w = w;
	drw->h = h;
	if (drw->drawable)
		XFreePixmap(drw->dpy, drw->drawable);
	drw->drawable = XCreatePixmap(drw->dpy, drw->root, w, h, DefaultDepth(drw->dpy, drw->screen));
}

void
drw_free(Drw *drw)
{
	XFreePixmap(drw->dpy, drw->drawable);
	XFreeGC(drw->dpy, drw->gc);
	free(drw);
}

/* This function is an implementation detail. Library users should use
 * drw_fontset_create instead.
 */
static Fnt *
xfont_create(Drw *drw, const char *fontname)
{
	Fnt *font;
	int n;
	char *def, **missing;

	printf("create font '%s'\n", fontname);

	font = ecalloc(1, sizeof(Fnt));

	font->set = XCreateFontSet(drw->dpy, fontname, &missing, &n, &def);
	if (missing) {
		while(n--)
			fprintf(stderr, "drw: missing fontset: %s\n", missing[n]);
		XFreeStringList(missing);
	}

	if (font->set) {
		XFontStruct **xfonts;
		char **font_names;
		XExtentsOfFontSet(font->set);
		n = XFontsOfFontSet(font->set, &xfonts, &font_names);
		while (n--) {
			font->ascent = MAX(font->ascent, (*xfonts)->ascent);
			font->descent = MAX(font->descent, (*xfonts)->descent);
			xfonts++;
		}
	} else {
		if (!(font->xfont = XLoadQueryFont(drw->dpy, fontname))
		    && !(font->xfont = XLoadQueryFont(drw->dpy, "fixed")))
			die("error, cannot load font: '%s'\n", fontname);
		font->ascent = font->xfont->ascent;
		font->descent = font->xfont->descent;
	}

	font->h = font->ascent + font->descent;
	font->dpy = drw->dpy;

	return font;
}

static void
xfont_free(Fnt *font)
{
	if (!font)
		return;
	if (font->set)
		XFreeFontSet(font->dpy, font->set);
	else
		XFreeFont(font->dpy, font->xfont);
	free(font);
}

Fnt*
drw_fontset_create(Drw* drw, const char *fonts[], size_t fontcount)
{
	Fnt *cur, *ret = NULL;
	size_t i;

	if (!drw || !fonts)
		return NULL;

	for (i = 1; i <= fontcount; i++) {
		if ((cur = xfont_create(drw, fonts[fontcount - i]))) {
			cur->next = ret;
			ret = cur;
		}
	}
	return (drw->fonts = ret);
}

void
drw_fontset_free(Fnt *font)
{
	if (font) {
		drw_fontset_free(font->next);
		xfont_free(font);
	}
}

void
drw_clr_create(Drw *drw, Clr *dest, const char *clrname)
{
	if (!drw || !dest || !clrname)
		return;

	if (!XAllocNamedColor(drw->dpy, DefaultColormap(drw->dpy, drw->screen),
	    clrname, dest, dest))
		die("error, cannot allocate color '%s'", clrname);
}

/* Wrapper to create color schemes. The caller has to call free(3) on the
 * returned color scheme when done using it. */
Clr *
drw_scm_create(Drw *drw, const char *clrnames[], size_t clrcount)
{
	size_t i;
	Clr *ret;

	/* need at least two colors for a scheme */
	if (!drw || !clrnames || clrcount < 2 || !(ret = ecalloc(clrcount, sizeof(Clr))))
		return NULL;

	for (i = 0; i < clrcount; i++)
		drw_clr_create(drw, &ret[i], clrnames[i]);
	return ret;
}

void
drw_setfontset(Drw *drw, Fnt *set)
{
	if (drw)
		drw->fonts = set;
}

void
drw_setscheme(Drw *drw, Clr *scm)
{
	if (drw)
		drw->scheme = scm;
}

void
drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert)
{
	if (!drw || !drw->scheme)
		return;
	XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
	if (filled)
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
	else
		XDrawRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w - 1, h - 1);
}

int
drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert)
{
	char buf[1024];
	size_t olen;
	int render = x || y || w || h;
	unsigned int ew, eh;
	int i, ty, len;

	if (!drw || (render && !drw->scheme) || !text || !drw->fonts)
		return 0;

	if (!render) {
		w = ~w;
	} else {
		XSetForeground(drw->dpy, drw->gc, drw->scheme[!invert ? ColBg : ColFg].pixel);
		/* XSetBackground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel); */
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);

		x += lpad;
		w -= lpad;
	}

	olen = strlen(text);
	drw_font_getexts(&drw->fonts[0], text, olen, &ew, &eh);
	/* shorten text if necessary */
	for (len = MIN(olen, sizeof buf); len && ew > w; len--)
		drw_font_getexts(&drw->fonts[0], text, len, &ew, &eh);

	if (len) {
		memcpy(buf, text, len);
		/* buf[len] = 0; */
		if (len < olen)
			for (i = len; i && i > len - 3; buf[--i] = '.')
				; /* NOP */

		if (render) {
			ty = y + (h - drw->fonts[0].h) / 2 + drw->fonts[0].ascent;
			XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColBg : ColFg].pixel);
			if (drw->fonts[0].set) {
#ifdef X_HAVE_UTF8_STRING
				Xutf8DrawString(drw->dpy, drw->drawable, drw->fonts[0].set, drw->gc, x, ty, buf, len);
#else
				XmbDrawString(drw->dpy, drw->drawable, drw->fonts[0].set, drw->gc, x, ty, buf, len);
#endif
			} else {
				XDrawString(drw->dpy, drw->drawable, drw->gc, x, ty, buf, len);
			}
		}
		x += ew;
		w -= ew;
	}

	return x + (render ? w : 0);
}

void
drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h)
{
	if (!drw)
		return;

	XCopyArea(drw->dpy, drw->drawable, win, drw->gc, x, y, w, h, x, y);
	XSync(drw->dpy, False);
}

unsigned int
drw_fontset_getwidth(Drw *drw, const char *text)
{
	if (!drw || !drw->fonts || !text)
		return 0;
	return drw_text(drw, 0, 0, 0, 0, 0, text, 0);
}

void
drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h)
{
	XRectangle r;

	if (!font || !text)
		return;

	if(font->set) {
#ifdef X_HAVE_UTF8_STRING
		Xutf8TextExtents(font->set, text, len, NULL, &r);
#else
		XmbTextExtents(font->set, text, len, NULL, &r);
#endif
		*w = r.width;
		*h = r.height;
	} else {
		*w = XTextWidth(font->xfont, text, len);
		*h = font->h;
	}
}

Cur *
drw_cur_create(Drw *drw, int shape)
{
	Cur *cur;

	if (!drw || !(cur = ecalloc(1, sizeof(Cur))))
		return NULL;

	cur->cursor = XCreateFontCursor(drw->dpy, shape);

	return cur;
}

void
drw_cur_free(Drw *drw, Cur *cursor)
{
	if (!cursor)
		return;

	XFreeCursor(drw->dpy, cursor->cursor);
	free(cursor);
}
