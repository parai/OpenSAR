// In Car, Every thing is static, so call it ST_GUI --> SG -->Sg
#ifndef GUI_H_
#define GUI_H_

// ==================================== INCLUDEs =========================
#include "Std_Types.h"
#include "Lcd.h"
#include <string.h>

// ==================================== MACROs   =========================
#define GUI_NOT_VISIBLE   ((SgLayer)(0xFF))
// ==================================== TYPEs    =========================
typedef enum
{
	SG_IMAGE,
	SG_DOTMAP,
	SG_POINTER,
	SG_USER_DEFINE
}SgWidgetType;

typedef uint16 SgLegth;

typedef uint8  SgAlign;

typedef uint16 SgDegree; // [0,360]

typedef uint32 SgColor;

typedef uint8  SgLayer;

typedef struct
{
	int x;
	int y;
}SgPoint;

typedef struct
{
	SgLegth width;
	SgLegth height;
}SgSize;

typedef struct
{
	SgPoint  pos;
	SgLegth  width;
	SgLegth  height;
}SgRectange;

typedef struct
{
	SgPoint center;
	SgLegth radius;
}SgCycle;

enum
{
	SG_TOP_ALIGN      = 0x01,
	SG_V_CENTER_ALIGN = 0x02,
	SG_BOTTOM_ALIGN   = 0x04,

	SG_LEFT_ALIGN     = 0x10,
	SG_H_CENTER_ALIGN = 0x20,
	SG_RIGHT_ALIGN    = 0x40,
};

typedef struct
{
	SgLayer      layer; // see GUI_NOT_VISIBLE
	SgRectange   area;
	SgAlign      align;
	SgDegree     degree;
	SgColor      color;
	void*        data;  // defined by the widget type
}SgContext;

typedef struct
{
	const SgWidgetType type;   // widget type
	const SgLayer      layer;  // default layer used when Sg_Init()
	const SgRectange   area;   // default area  used when Sg_Init()
	void             (*draw)(void* data);
	void* const        data;   // defined by the widget type
}SgConst;

typedef struct
{
	SgContext*      pContext;  // Widget Context Data in RAM
	const SgConst*  pConst;    // Widget Constant Data in ROM
}SgWidget;

// Const Config Data of widget
typedef struct
{
	SgLegth    head;
	SgLegth    tail;
	SgDegree   start; // start degree
	SgLegth    length;
	SgColor    color;
}cSgPointer;

typedef struct
{
	SgSize size;
	uint8* pixels; // each pixel is (R,G,B)
}cSgImage;

typedef struct
{
	SgSize size;
	uint8* dots;
}cSgDotmap;

// ---------------- Config
typedef struct
{
	SgWidget* widgets;
	uint32    number;
	SgLayer   maxLayer;
}SgConfig;
#include "Gui_Cfg.h"
// ==================================== DATAs    =========================

// ==================================== FUNCTIONs=========================
void Sg_Init(const SgConfig* config);
void Sg_Calc(int *pX,int *pY,const SgPoint* center,uint32 degree);
// basic draw
void Sg_DrawPixel(int x,int y,SgColor color);
void Sg_DrawLine(int x0,int y0,int x1,int y1,SgColor color);
void Sg_FillArea(int x, int y, int cx, int cy, SgColor color);
void Sg_DrawCircle(int x, int y, int radius, SgColor color);
void Sg_FillCircle(int x, int y, int radius, SgColor color);
void Sg_DrawEllipse(int x, int y, int a, int b, SgColor color);
void Sg_FillEllipse(int x, int y, int a, int b, SgColor color);
// high level draw
void Sg_DrawImage(SgWidget* widget);
void Sg_DrawDotMap(SgWidget* widget);
void Sg_DrawPointer(SgWidget* widget);

Std_ReturnType Sg_SetWidgetArea(uint32 id,uint32 x,uint32 y,uint32 width,uint32 height);
Std_ReturnType Sg_SetWidgetDegree(uint32 id,uint32 degree);
Std_ReturnType Sg_SetWidgetLayer(uint32 id,SgLayer layer);
void Sg_MainFunction(void);

#endif /* GUI_H_ */
