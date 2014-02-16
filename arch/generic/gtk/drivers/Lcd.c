// ===================================== INCLUDEs  =======================================
#include "Lcd.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

// ===================================== MACROs    =======================================
// 0 --> use GtkImage
// 1 --> use GtkDrawingArea  : this is more powerful, so use this
#define LCD_IMAGE        0
#define LCD_DRAWING_AREA 1
#define cfgLcdHandle   LCD_DRAWING_AREA
#define LCD_WIDTH   1024
#define LCD_HEIGHT  350
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
static GtkWidget*       pLcd        = NULL;
static GdkPixbuf*       pLcdImage   = NULL;
const Font gFont = {10, 14};
static tLcd sLcd;

// ===================================== FUNCTIONs =======================================
extern void arch_update_statusbar(guchar* text);

#if(cfgLcdHandle == LCD_DRAWING_AREA)
static gboolean scribble_draw (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
	gdk_cairo_set_source_pixbuf (cr, pLcdImage, 0, 0);
	cairo_paint (cr);
	return TRUE;
}
static gboolean
scribble_motion_notify_event (GtkWidget      *widget,
                              GdkEventMotion *event,
                              gpointer        data)
{
  int x, y;
  guchar text[256];
  GdkModifierType state;

  /* This call is very important; it requests the next motion event.
   * If you don't call gdk_window_get_pointer() you'll only get
   * a single motion event. The reason is that we specified
   * GDK_POINTER_MOTION_HINT_MASK to gtk_widget_set_events().
   * If we hadn't specified that, we could just use event->x, event->y
   * as the pointer location. But we'd also get deluged in events.
   * By requesting the next event as we handle the current one,
   * we avoid getting a huge number of events faster than we
   * can cope.
   */

  gdk_window_get_device_position (event->window, event->device, &x, &y, &state);

// if (state & GDK_BUTTON1_MASK)
  sprintf((char*)text,"X=%d,Y=%d",x,y);
  arch_update_statusbar(text);

  /* We've handled it, stop processing */
  return TRUE;
}
#endif
static gboolean Refresh(gpointer data)
{
	uint32_t x,y;
	int width, height, rowstride, n_channels;
	guchar *pixels, *p;

	n_channels = gdk_pixbuf_get_n_channels (pLcdImage);

	g_assert (gdk_pixbuf_get_colorspace (pLcdImage) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (pLcdImage) == 8);

	g_assert (!gdk_pixbuf_get_has_alpha (pLcdImage));
	g_assert (n_channels == 3);

	width = gdk_pixbuf_get_width (pLcdImage);
	height = gdk_pixbuf_get_height (pLcdImage);

	g_assert (LCD_WIDTH==width);
	g_assert (LCD_HEIGHT==height);

	rowstride = gdk_pixbuf_get_rowstride (pLcdImage);
	pixels = gdk_pixbuf_get_pixels (pLcdImage);

	for(x=0;x<LCD_WIDTH;x++)
	{
		for(y=0;y<LCD_HEIGHT;y++)
		{
			uint32_t color = sLcd.P[x][y].color;
			p = pixels + y * rowstride + x * n_channels;
			p[0] = (color>>16)&0xFF; // red
			p[1] = (color>>8 )&0xFF; // green
			p[2] = (color>>0 )&0xFF; // blue
		}
	}
#if(cfgLcdHandle == LCD_DRAWING_AREA)
	gtk_widget_queue_draw (pLcd);
#else
	gtk_image_set_from_pixbuf(GTK_IMAGE(pLcd),pLcdImage);
#endif

	return TRUE;
}

GtkWidget* Lcd(void)
{
	GtkWidget* pBox;
	pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_set_homogeneous(GTK_BOX(pBox),FALSE);

	memset(&sLcd,0,sizeof(&sLcd));

	g_timeout_add(40,Refresh,NULL); // Refresh LCD 25 times each 1s

	pLcdImage = gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,LCD_WIDTH,LCD_HEIGHT);

#if(cfgLcdHandle == LCD_DRAWING_AREA)
	pLcd = gtk_drawing_area_new ();
	gtk_widget_set_size_request (pLcd, LCD_WIDTH, LCD_HEIGHT);
	g_signal_connect (pLcd, "draw",
	                        G_CALLBACK (scribble_draw), NULL);

	g_signal_connect (pLcd, "motion-notify-event",
	                        G_CALLBACK (scribble_motion_notify_event), NULL);
	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to
	 */
	gtk_widget_set_events (pLcd, gtk_widget_get_events (pLcd)
							 /*| GDK_LEAVE_NOTIFY_MASK
							 | GDK_BUTTON_PRESS_MASK*/
							 | GDK_POINTER_MOTION_MASK
							 | GDK_POINTER_MOTION_HINT_MASK);
#else
	pLcd = gtk_image_new();
#endif
	gtk_box_pack_start(GTK_BOX(pBox),pLcd,FALSE,FALSE,0);

	return pBox;
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
	LCDD_DrawRectangleWithFill(0,0,LCD_WIDTH,LCD_HEIGHT,color);
}

void LCDD_DrawPixel( uint32_t x, uint32_t y, uint32_t color )
{
	if((x<LCD_WIDTH) && (y<LCD_HEIGHT))
	{
		sLcd.P[x][y].color = color;
	}
}

uint32_t LCDD_ReadPixel( uint32_t x, uint32_t y )
{
	if((x<LCD_WIDTH) && (y<LCD_HEIGHT))
	{
		return sLcd.P[x][y].color;
	}

	return COLOR_BLACK;
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
	for(uint32_t X=dwX;X<(dwX+dwWidth);X++)
	{
		for(uint32_t Y=dwY;Y<(dwY+dwHeight);Y++)
		{
			LCDD_DrawPixel(X,Y,dwColor);
		}
	}
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

void LCDD_DrawStringWithBGColor( uint32_t x, uint32_t y, const uint8_t *pString, uint32_t fontColor, uint32_t bgColor )
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
void LCDD_DrawImage( uint32_t dwX, uint32_t dwY, const uint8_t *pImage, uint32_t dwWidth, uint32_t dwHeight )
{
    uint32_t dwColor ;

	for(uint32_t Y=dwY;Y<(dwY+dwHeight);Y++)
	{
		for(uint32_t X=dwX;X<(dwX+dwWidth);X++)
		{
			dwColor = ((uint32_t)pImage[0]<<16) + ((uint32_t)pImage[1]<<8) + (pImage[2]);
			LCDD_DrawPixel(X,Y,dwColor);
			pImage = pImage+3;
		}
	}
}

void LCDD_DrawGIMPImage( uint32_t dwX, uint32_t dwY, const SGIMPImage* pGIMPImage )
{
    LCDD_DrawImage(dwX,dwY,pGIMPImage->pucPixel_data,pGIMPImage->dwWidth,pGIMPImage->dwHeight);
}
