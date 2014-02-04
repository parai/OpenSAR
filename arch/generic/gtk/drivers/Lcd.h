// see this is really a copy of libboard_sam3s-ek
#ifndef LCD_H_
#define LCD_H_
// ===================================== INCLUDEs  =======================================
#include "Std_Types.h"

// ===================================== MACROs    =======================================
#define COLOR_BLACK          0x000000
#define COLOR_WHITE          0xFFFFFF

#define COLOR_BLUE           0x0000FF
#define COLOR_GREEN          0x00FF00
#define COLOR_RED            0xFF0000

#define COLOR_NAVY           0x000080
#define COLOR_DARKBLUE       0x00008B
#define COLOR_DARKGREEN      0x006400
#define COLOR_DARKCYAN       0x008B8B
#define COLOR_CYAN           0x00FFFF
#define COLOR_TURQUOISE      0x40E0D0
#define COLOR_INDIGO         0x4B0082
#define COLOR_DARKRED        0x800000
#define COLOR_OLIVE          0x808000
#define COLOR_GRAY           0x808080
#define COLOR_SKYBLUE        0x87CEEB
#define COLOR_BLUEVIOLET     0x8A2BE2
#define COLOR_LIGHTGREEN     0x90EE90
#define COLOR_DARKVIOLET     0x9400D3
#define COLOR_YELLOWGREEN    0x9ACD32
#define COLOR_BROWN          0xA52A2A
#define COLOR_DARKGRAY       0xA9A9A9
#define COLOR_SIENNA         0xA0522D
#define COLOR_LIGHTBLUE      0xADD8E6
#define COLOR_GREENYELLOW    0xADFF2F
#define COLOR_SILVER         0xC0C0C0
#define COLOR_LIGHTGREY      0xD3D3D3
#define COLOR_LIGHTCYAN      0xE0FFFF
#define COLOR_VIOLET         0xEE82EE
#define COLOR_AZUR           0xF0FFFF
#define COLOR_BEIGE          0xF5F5DC
#define COLOR_MAGENTA        0xFF00FF
#define COLOR_TOMATO         0xFF6347
#define COLOR_GOLD           0xFFD700
#define COLOR_ORANGE         0xFFA500
#define COLOR_SNOW           0xFFFAFA
#define COLOR_YELLOW         0xFFFF00

#define COLOR_BGREEN         0xC7EDCC

// ===================================== TYPEs     =======================================
typedef struct _Font {
	/* Font width in pixels. */
	uint8_t width;
	/* Font height in pixels. */
	uint8_t height;
} Font;

// ===================================== DATAs     =======================================
extern const uint8_t pCharset10x14[] ;
extern const Font gFont;

// ===================================== FUNCTIONs =======================================
extern void LCDD_Initialize(void);
extern void LCDD_On(void);
extern void LCDD_Off(void);
extern void LCDD_SetBacklight (uint32_t step);

extern void LCDD_Fill( uint32_t color ) ;
extern void LCDD_DrawPixel( uint32_t x, uint32_t y, uint32_t c ) ;
extern uint32_t LCDD_ReadPixel( uint32_t x, uint32_t y ) ;
extern void LCDD_DrawLine( uint32_t sX, uint32_t sY, uint32_t oX, uint32_t oY, uint32_t color ) ;
extern void LCDD_DrawRectangle( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor ) ;
extern void LCDD_DrawRectangleWithFill( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor ) ;
extern void LCDD_DrawCircle( uint32_t x, uint32_t y, uint32_t r, uint32_t color ) ;
extern void LCDD_DrawString( uint32_t x, uint32_t y, const uint8_t *pString, uint32_t color ) ;
extern void LCDD_DrawStringWithBGColor( uint32_t x, uint32_t y, const char *pString, uint32_t fontColor, uint32_t bgColor ) ;
extern void LCDD_GetStringSize( const uint8_t *pString, uint32_t *pWidth, uint32_t *pHeight ) ;
extern void LCDD_DrawImage( uint32_t x, uint32_t y, const uint8_t *pImage, uint32_t width, uint32_t height ) ;
//extern void LCDD_DrawGIMPImage( uint32_t dwX, uint32_t dwY, const SGIMPImage* pGIMPImage );
extern void LCDD_ClearWindow( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight, uint32_t dwColor ) ;

extern void LCDD_DrawChar( uint32_t x, uint32_t y, uint8_t c, uint32_t color ) ;
extern void LCDD_DrawCharWithBGColor( uint32_t x, uint32_t y, uint8_t c, uint32_t fontColor, uint32_t bgColor ) ;


#endif /* LCD_H_ */
