// ==================================== INCLUDEs =========================
#include "Gui.h"
#include <math.h>
#include <assert.h>

// ==================================== MACROs   =========================

// ==================================== TYPEs    =========================

// ==================================== DATAs    =========================

// ==================================== FUNCTIONs=========================

void Sg_DrawPixel(int X,int Y,SgColor color)
{
	LCDD_DrawPixel(X,Y,color);
}

void Sg_DrawLine(int x0,int y0,int x1,int y1,SgColor color)
{
	int dy, dx;
	int addx, addy;
	int P, diff, i;


	// speed improvement if vertical or horizontal
	if (x0 == x1) {
		if (y1 > y0)
			Sg_FillArea(x0, y0, 1, y1-y0+1, color);
		else
			Sg_FillArea(x0, y1, 1, y0-y1+1, color);
		return;
	}
	if (y0 == y1) {
		if (x1 > x0)
			Sg_FillArea(x0, y0, x1-x0+1, 1, color);
		else
			Sg_FillArea(x1, y0, x0-x1+1, 1, color);
		return;
	}

	if (x1 >= x0) {
		dx = x1 - x0;
		addx = 1;
	} else {
		dx = x0 - x1;
		addx = -1;
	}
	if (y1 >= y0) {
		dy = y1 - y0;
		addy = 1;
	} else {
		dy = y0 - y1;
		addy = -1;
	}

	if (dx >= dy) {
		dy *= 2;
		P = dy - dx;
		diff = P - dx;

		for(i=0; i<=dx; ++i) {
			Sg_DrawPixel(x0, y0, color);
			if (P < 0) {
				P  += dy;
				x0 += addx;
			} else {
				P  += diff;
				x0 += addx;
				y0 += addy;
			}
		}
	} else {
		dx *= 2;
		P = dx - dy;
		diff = P - dy;

		for(i=0; i<=dy; ++i) {
			Sg_DrawPixel(x0, y0, color);
			if (P < 0) {
				P  += dx;
				y0 += addy;
			} else {
				P  += diff;
				x0 += addx;
				y0 += addy;
			}
		}
	}
}

void Sg_FillArea(int x, int y, int cx, int cy, SgColor color)
{
		int x0, x1, y1;

		x0 = x;
		x1 = x + cx;
		y1 = y + cy;
		for(; y < y1; y++)
			for(x = x0; x < x1; x++)
				Sg_DrawPixel(x, y, color);
}

void Sg_DrawCircle(int x, int y, int radius, SgColor color) {
	int a, b, P;

	a = 0;
	b = radius;
	P = 1 - radius;

	do {
		Sg_DrawPixel(x+a, y+b, color);
		Sg_DrawPixel(x+b, y+a, color);
		Sg_DrawPixel(x-a, y+b, color);
		Sg_DrawPixel(x-b, y+a, color);
		Sg_DrawPixel(x+b, y-a, color);
		Sg_DrawPixel(x+a, y-b, color);
		Sg_DrawPixel(x-a, y-b, color);
		Sg_DrawPixel(x-b, y-a, color);
		if (P < 0)
			P += 3 + 2*a++;
		else
			P += 5 + 2*(a++ - b--);
	} while(a <= b);
}

void Sg_FillCircle(int x, int y, int radius, SgColor color)
{
	int a, b, P;

	a = 0;
	b = radius;
	P = 1 - radius;

	do {
		Sg_DrawLine(x-a, y+b, x+a, y+b, color);
		Sg_DrawLine(x-a, y-b, x+a, y-b, color);
		Sg_DrawLine(x-b, y+a, x+b, y+a, color);
		Sg_DrawLine(x-b, y-a, x+b, y-a, color);
		if (P < 0)
			P += 3 + 2*a++;
		else
			P += 5 + 2*(a++ - b--);
	} while(a <= b);
}

void Sg_DrawEllipse(int x, int y, int a, int b, SgColor color) {
	int  dx = 0, dy = b; /* im I. Quadranten von links oben nach rechts unten */
	long a2 = a*a, b2 = b*b;
	long err = b2-(2*b-1)*a2, e2; /* Fehler im 1. Schritt */

	do {
		Sg_DrawPixel(x+dx, y+dy, color); /* I. Quadrant */
		Sg_DrawPixel(x-dx, y+dy, color); /* II. Quadrant */
		Sg_DrawPixel(x-dx, y-dy, color); /* III. Quadrant */
		Sg_DrawPixel(x+dx, y-dy, color); /* IV. Quadrant */

		e2 = 2*err;
		if(e2 <  (2*dx+1)*b2) {
			dx++;
			err += (2*dx+1)*b2;
		}
		if(e2 > -(2*dy-1)*a2) {
			dy--;
			err -= (2*dy-1)*a2;
		}
	} while(dy >= 0);

	while(dx++ < a) { /* fehlerhafter Abbruch bei flachen Ellipsen (b=1) */
		Sg_DrawPixel(x+dx, y, color); /* -> Spitze der Ellipse vollenden */
		Sg_DrawPixel(x-dx, y, color);
   }
}

void Sg_FillEllipse(int x, int y, int a, int b, SgColor color)
{
	int  dx = 0, dy = b; /* im I. Quadranten von links oben nach rechts unten */
	long a2 = a*a, b2 = b*b;
	long err = b2-(2*b-1)*a2, e2; /* Fehler im 1. Schritt */

	do {
		Sg_DrawLine(x-dx,y+dy,x+dx,y+dy, color);
		Sg_DrawLine(x-dx,y-dy,x+dx,y-dy, color);

		e2 = 2*err;
		if(e2 <  (2*dx+1)*b2) {
			dx++;
			err += (2*dx+1)*b2;
		}
		if(e2 > -(2*dy-1)*a2) {
			dy--;
			err -= (2*dy-1)*a2;
		}
	} while(dy >= 0);

	while(dx++ < a) { /* fehlerhafter Abbruch bei flachen Ellipsen (b=1) */
		Sg_DrawPixel(x+dx, y, color); /* -> Spitze der Ellipse vollenden */
		Sg_DrawPixel(x-dx, y, color);
   }
}




