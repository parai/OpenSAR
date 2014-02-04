// ===================================== INCLUDEs  =======================================
#include "Lcd.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <assert.h>
#include <string.h>

// ===================================== MACROs    =======================================
#define LCD_WIDTH   800
#define LCD_HEIGHT  600
// ===================================== TYPEs     =======================================
typedef struct
{
	uint32_t color;
}tPixel;

typedef struct
{
	tPixel P[LCD_WIDTH][LCD_HEIGHT];
}tLcd;
// ===================================== DATAs     =======================================
/* Pixmap for scribble area, to store current scribbles */
static cairo_surface_t *pLcdSurface = NULL;
static GtkWidget*       pLcd        = NULL;
const Font gFont = {10, 14};
static tLcd sLcd;

// ===================================== FUNCTIONs =======================================
/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
scribble_configure_event (GtkWidget         *widget,
                          GdkEventConfigure *event,
                          gpointer           data)
{
  GtkAllocation allocation;
  cairo_t *cr;

  if (pLcdSurface)
    cairo_surface_destroy (pLcdSurface);

  gtk_widget_get_allocation (widget, &allocation);
  pLcdSurface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               allocation.width,
                                               allocation.height);

  /* Initialize the surface to white */
  cr = cairo_create (pLcdSurface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

/* Redraw the screen from the surface */
static gboolean
scribble_draw (GtkWidget *widget,
               cairo_t   *cr,
               gpointer   data)
{
  cairo_set_source_surface (cr, pLcdSurface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}
static void DrawPixel( uint32_t x, uint32_t y, uint32_t color )
{
	GdkRectangle update_rect;
	cairo_t *cr;

	update_rect.x = (x>0?(x-1):0);
	update_rect.y = (y>0?(y-1):0);
	update_rect.width = 1;
	update_rect.height = 1;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (pLcdSurface);
	cairo_set_source_rgb (cr, (double)((color>>16)&0xFF)/(double)255,
							  (double)((color>>8 )&0xFF)/(double)255,
							  (double)((color>>0 )&0xFF)/(double)255);
	gdk_cairo_rectangle (cr, &update_rect);
	cairo_fill (cr);

	cairo_destroy (cr);

	/* Now invalidate the affected region of the drawing area. */
	gdk_window_invalidate_rect (gtk_widget_get_window (pLcd),
							  &update_rect,
							  FALSE);
}

static gboolean Refresh(gpointer data)
{
	uint32_t x,y;

	for(x=0;x<10;x++)
	{
		for(y=0;y<10;y++)
		{
			DrawPixel(x,y,sLcd.P[x][y].color);
		}
	}
	return TRUE;
}

GtkWidget* Lcd(void)
{
	GtkWidget* frame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);

    pLcd = gtk_drawing_area_new ();

    /* set a minimum size */
    gtk_widget_set_size_request (pLcd, LCD_WIDTH, LCD_HEIGHT);

    gtk_container_add (GTK_CONTAINER (frame), pLcd);

	/* Signals used to handle backing surface */

	g_signal_connect (pLcd, "draw",
				   G_CALLBACK (scribble_draw), NULL);
	g_signal_connect (pLcd,"configure-event",
				   G_CALLBACK (scribble_configure_event), NULL);

//	g_timeout_add(40,Refresh,NULL); // Refresh LCD 25 times each 1s
//	memset(&sLcd,0,sizeof(&sLcd));
    return frame;
}
// =========================== DATAs =================================

// =========================== FUNCTIONs =============================
void LCDD_On(void)
{
	LCDD_Fill(COLOR_BLACK);
}
void LCDD_Off(void)
{
	LCDD_Fill(COLOR_BLUE);
}

void LCDD_Fill(uint32_t color)
{
	GdkRectangle update_rect;
	cairo_t *cr;

	update_rect.x = 0;
	update_rect.y = 0;
	update_rect.width = LCD_WIDTH;
	update_rect.height = LCD_HEIGHT;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (pLcdSurface);
	cairo_set_source_rgb (cr, (double)((color>>16)&0xFF)/(double)255,
							  (double)((color>>8 )&0xFF)/(double)255,
							  (double)((color>>0 )&0xFF)/(double)255);
	gdk_cairo_rectangle (cr, &update_rect);
	cairo_fill (cr);

	cairo_destroy (cr);

	/* Now invalidate the affected region of the drawing area. */
	gdk_window_invalidate_rect (gtk_widget_get_window (pLcd),
							  &update_rect,
							  FALSE);
}

void LCDD_DrawPixel( uint32_t x, uint32_t y, uint32_t color )
{
	GdkRectangle update_rect;
	cairo_t *cr;

	update_rect.x = (x>0?(x-1):0);
	update_rect.y = (y>0?(y-1):0);
	update_rect.width = 1;
	update_rect.height = 1;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (pLcdSurface);
	cairo_set_source_rgb (cr, (double)((color>>16)&0xFF)/(double)255,
							  (double)((color>>8 )&0xFF)/(double)255,
							  (double)((color>>0 )&0xFF)/(double)255);
	gdk_cairo_rectangle (cr, &update_rect);
	cairo_fill (cr);

	cairo_destroy (cr);

	/* Now invalidate the affected region of the drawing area. */
	gdk_window_invalidate_rect (gtk_widget_get_window (pLcd),
							  &update_rect,
							  FALSE);
}

void LCDD_DrawLine( uint32_t sX, uint32_t sY, uint32_t oX, uint32_t oY, uint32_t color )
{   //                      oY - sY
	// Y = k(X-sX) + sX = ----------- (X - sX) + sY
	//                      oX - sX
	if(oX != sX)
	{
		double K = (double)(oY - sY)/(double)(oX - sX);
		if(sX<oX)
		{
			for(uint32_t X=sX;X<oX;X++)
			{
				uint32_t Y = (uint32_t)(K*(X-sX) + sY);
				LCDD_DrawPixel(X,Y,color);
			}
		}
		else
		{
			for(uint32_t X=oX;X<sX;X++)
			{
				uint32_t Y = (uint32_t)(K*(X-sX) + sY);
				LCDD_DrawPixel(X,Y,color);
			}
		}
	}
	else
	{
		if(sY<oY)
		{
			for(uint32 Y=sY;Y<oY;Y++)
			{
				LCDD_DrawPixel(sX,Y,color);
			}
		}
		else
		{
			for(uint32 Y=oY;Y<sY;Y++)
			{
				LCDD_DrawPixel(sX,Y,color);
			}
		}

	}
}
void LCDD_DrawRectangle( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor )
{
	LCDD_DrawLine(dwX,        dwY,         dwX+dwWidth,dwY,         dwColor);
	LCDD_DrawLine(dwX+dwWidth,dwY,         dwX+dwWidth,dwY+dwHeight,dwColor);
	LCDD_DrawLine(dwX+dwWidth,dwY+dwHeight,dwX,        dwY+dwHeight,dwColor);
	LCDD_DrawLine(dwX,        dwY+dwHeight,dwX,        dwY,         dwColor);
}
void LCDD_DrawRectangleWithFill( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor )
{
	GdkRectangle update_rect;
	cairo_t *cr;

	update_rect.x = dwX;
	update_rect.y = dwY;
	update_rect.width = dwWidth;
	update_rect.height = dwHeight;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (pLcdSurface);
	cairo_set_source_rgb (cr, (double)((dwColor>>16)&0xFF)/(double)255,
							  (double)((dwColor>>8 )&0xFF)/(double)255,
							  (double)((dwColor>>0 )&0xFF)/(double)255);
	gdk_cairo_rectangle (cr, &update_rect);
	cairo_fill (cr);

	cairo_destroy (cr);

	/* Now invalidate the affected region of the drawing area. */
	gdk_window_invalidate_rect (gtk_widget_get_window (pLcd),
							  &update_rect,
							  FALSE);
}

void LCDD_DrawCircle( uint32_t x, uint32_t y, uint32_t r, uint32_t color )
{
    signed int    d;    /* Decision Variable */
    uint32_t  curX; /* Current X Value */
    uint32_t  curY; /* Current Y Value */

    d = 3 - (r << 1);
    curX = 0;
    curY = r;

    while (curX <= curY)
    {
        LCDD_DrawPixel(x + curX, y + curY, color);
        LCDD_DrawPixel(x + curX, y - curY, color);
        LCDD_DrawPixel(x - curX, y + curY, color);
        LCDD_DrawPixel(x - curX, y - curY, color);
        LCDD_DrawPixel(x + curY, y + curX, color);
        LCDD_DrawPixel(x + curY, y - curX, color);
        LCDD_DrawPixel(x - curY, y + curX, color);
        LCDD_DrawPixel(x - curY, y - curX, color);

        if (d < 0) {
            d += (curX << 2) + 6;
        }
        else {
            d += ((curX - curY) << 2) + 10;
            curY--;
        }
        curX++;
    }
}

void LCDD_ClearWindow( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor )
{
	LCDD_DrawRectangleWithFill(dwX,dwY,dwWidth,dwHeight,dwColor);
}

void LCDD_DrawChar( uint32_t x, uint32_t y, uint8_t c, uint32_t color )
{
    uint32_t row, col ;

    assert( (c >= 0x20) && (c <= 0x7F) ) ;

    for ( col = 0 ; col < 10 ; col++ )
    {
        for ( row = 0 ; row < 8 ; row++ )
        {
            if ( (pCharset10x14[((c - 0x20) * 20) + col * 2] >> (7 - row)) & 0x1 )
            {
                LCDD_DrawPixel( x+col, y+row, color ) ;
            }
        }

        for (row = 0; row < 6; row++ )
        {
            if ((pCharset10x14[((c - 0x20) * 20) + col * 2 + 1] >> (7 - row)) & 0x1)
            {
                LCDD_DrawPixel( x+col, y+row+8, color ) ;
            }
        }
    }
}

void LCDD_DrawCharWithBGColor( uint32_t x, uint32_t y, uint8_t c, uint32_t fontColor, uint32_t bgColor )
{
    uint32_t row, col ;

    assert( (c >= 0x20) && (c <= 0x7F) ) ;

    for (col = 0; col < 10; col++)
    {
        for (row = 0 ; row < 8 ; row++)
        {
            if ( (pCharset10x14[((c - 0x20) * 20) + col * 2] >> (7 - row)) & 0x1 )
            {
                LCDD_DrawPixel( x+col, y+row, fontColor ) ;
            }
            else
            {
                LCDD_DrawPixel( x+col, y+row, bgColor ) ;
            }
        }

        for ( row = 0 ; row < 6 ; row++ )
        {
            if ( (pCharset10x14[((c - 0x20) * 20) + col * 2 + 1] >> (7 - row)) & 0x1 )
            {
                LCDD_DrawPixel( x+col, y+row+8, fontColor ) ;
            }
            else
            {
                LCDD_DrawPixel( x+col, y+row+8, bgColor ) ;
            }
        }
    }
}

void LCDD_DrawString( uint32_t x, uint32_t y, const uint8_t *pString, uint32_t color )
{
    uint32_t xorg = x ;

    while ( *pString != 0 )
    {
        if ( *pString == '\n' )
        {
            y += gFont.height + 2 ;
            x = xorg ;
        }
        else
        {
            LCDD_DrawChar( x, y, *pString, color ) ;
            x += gFont.width + 2 ;
        }

        pString++ ;
    }
}

void LCDD_DrawStringWithBGColor( uint32_t x, uint32_t y, const char *pString, uint32_t fontColor, uint32_t bgColor )
{
    unsigned xorg = x;

    while ( *pString != 0 )
    {
        if ( *pString == '\n' )
        {
            y += gFont.height + 2 ;
            x = xorg ;
        }
        else
        {
           LCDD_DrawCharWithBGColor( x, y, *pString, fontColor, bgColor ) ;
           x += gFont.width + 2;
        }

        pString++;
    }
}

void LCDD_GetStringSize( const uint8_t *pString, uint32_t *pWidth, uint32_t *pHeight )
{
    uint32_t width = 0;
    uint32_t height = gFont.height;

    while ( *pString != 0 )
    {
        if ( *pString == '\n' )
        {
            height += gFont.height + 2 ;
        }
        else
        {
            width += gFont.width + 2 ;
        }

        pString++ ;
    }

    if ( width > 0 )
    {
        width -= 2;
    }

    if ( pWidth != NULL )
    {
        *pWidth = width;
    }

    if ( pHeight != NULL )
    {
        *pHeight = height ;
    }
}
void Lcd_Test(void)
{
	static unsigned int caller=0;
	caller ++;
	if(10 == caller)
	{
		LCDD_On();
	}
	else if(20==caller)
	{
		LCDD_DrawString(0,0,(const uint8*)"Start to learn something about LCD GUI,Welcome!",COLOR_CYAN);
		//LCDD_DrawRectangle(5,3,50,30,0x000000);
	}
	else if(30==caller)
	{
		LCDD_DrawString(0,20,(const uint8*)"~`@#$%^&*()_+-=[]{}\\|<>,./?",COLOR_CYAN);
		//LCDD_DrawRectangleWithFill(50,30,50,30,0x334455);
	}
	else if(40==caller)
	{
		//LCDD_DrawCircle(100,100,70,0x334455);
	}
	else if(50==caller)
	{
		LCDD_DrawString(50,50,(const uint8*)"Hello World!\nMy Baby!",COLOR_CYAN);
	}
	else if(2000 == caller)
	{
		LCDD_Off();
		caller = 0;
	}
}

