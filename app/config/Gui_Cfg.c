#include "Gui.h"

#include "Lcd_Res/Lcd_Res.c"

typedef struct
{
	uint8  head;
	uint8  tail;
	uint32 length;
	uint32 color;
}Gui_PointerType;

void DrawPointer(const GuiWidget_Type* widget)
{
	GuiWidgetContext_Type* pContext = widget->pContext;
	uint32 degree = (pContext->degree%360);
	const Gui_PointerType* pointer = widget->data;
	for(uint32 r=pointer->head;r<pointer->head+7;r++)
	{
		LCDD_DrawCircle(widget->center.x,widget->center.y,r,pointer->color);
		LCDD_DrawCircle(widget->center.x,widget->center.y+1,r,pointer->color);
	}

	for(uint32 r=pointer->head+5;r<=pointer->length;r++)
	{   //                      oY - sY
		// Y = k(X-sX) + sX = ----------- (X - sX) + sY
		//                      oX - sX

		uint32 y =  (r-(pointer->head+5))*(pointer->head - pointer->tail)/2;
		y = y/(pointer->length-(pointer->head+5));
		y = pointer->head/2 - y;
		uint32 X = r+widget->center.x;
		for(uint32 Y = widget->center.y-y;Y<=widget->center.y+y;Y++)
		{
			int tX,tY;
			tX=X;tY=Y;
			Gui_Calc(&tX,&tY,&(widget->pContext->area.top_left),&(widget->center),degree);

			LCDD_DrawPixel(tX,tY,pointer->color);
			LCDD_DrawPixel(tX,tY+1,pointer->color);
		}
	}
}

static GuiWidgetContext_Type Context[4];

static const Gui_PointerType Pointers[2] =
{
	{
		.head = 10,
		.tail = 2,
		.length =70,
		.color = 0xD63441
	},
	{
		.head = 10,
		.tail = 2,
		.length =80,
		.color = 0x7453A2
	}
};
static const GuiWidget_Type widgets[4]=
{
	{
		.pContext=&Context[0],
		.defaultContext.layer = 0,
		.defaultContext.area.top_left.x = 0,
		.defaultContext.area.top_left.y = 0,
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 0,
		.center.x                   = 97,
		.center.y                   = 90,
		.data                      = &IMG0_image,  //SpeedG
		.draw                       = NULL	// use the default to Draw the image
	},
	{
		.pContext=&Context[1],
		.defaultContext.layer = 0,
		.defaultContext.area.top_left.x = 300,
		.defaultContext.area.top_left.y = 0,
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 0,
		.center.x                   = 107,
		.center.y                   = 104,
		.data                      = &IMG1_image,  //TachoG
		.draw                       = NULL	// use the default to Draw the image
	},
	{
		.pContext=&Context[2],
		.defaultContext.layer = 1,
		.defaultContext.area.top_left.x = 0,  // must be
		.defaultContext.area.top_left.y = 0,  // must be
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 0,
		.center.x                   = 100,
		.center.y                   = 92,
		.data                       = &Pointers[0],         //PointerG
		.draw                       = DrawPointer
	},
	{
		.pContext=&Context[3],
		.defaultContext.layer = 1,
		.defaultContext.area.top_left.x = 0,  // must be
		.defaultContext.area.top_left.y = 0,  // must be
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 0,
		.center.x                   = 109+300,
		.center.y                   = 105,
		.data                       = &Pointers[1],         //PointerG
		.draw                       = DrawPointer
	}
};
const GuiConfig_Type GuiConfigData=
{
	.widgets = widgets,
	.number  = sizeof(widgets)/sizeof(GuiWidget_Type)
};
