/*
 * Gui.h
 *
 *  Created on: 2014Äê2ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef GUI_H_
#define GUI_H_

// ==================================== INCLUDEs =========================
#include "Std_Types.h"
#include "Lcd.h"

// ==================================== MACROs   =========================
#define GUI_NOT_VISIBLE   0xFF

// ==================================== TYPEs    =========================
typedef SGIMPImage GuiImage_Type ;

typedef struct
{
	uint32 x;
	uint32 y;
}GuiPosition_Type;

typedef struct
{
	GuiPosition_Type top_left;
	uint32           width;
	uint32           height;
}GuiRectangle_Type;

typedef struct
{
	/* 0xFF: not displayed,
	 * other values: the layer with bigger value is above the layers with smaller value
	 * For example: layer 1 is above layer 0.*/
	uint8              layer;
	GuiRectangle_Type  area;

	const GuiImage_Type*     image;
}GuiWidget_Type;

typedef struct
{
	GuiWidget_Type*  const widgets;
	const uint32           number;
}GuiConfig_Type;

#include "Gui_Cfg.h"
// ==================================== DATAs    =========================

// ==================================== FUNCTIONs=========================
void Gui_Init(const GuiConfig_Type* config);
Std_ReturnType Gui_SetWidgetImage(uint32 id,GuiImage_Type* image);
Std_ReturnType Gui_SetWidgetArea(uint32 id,uint32 x,uint32 y,uint32 width,uint32 height);
Std_ReturnType Gui_SetWidgetLayer(uint32 id,uint8 layer);
void Gui_MainFunction(void);

#endif /* GUI_H_ */
