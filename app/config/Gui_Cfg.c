#if defined(USE_GUI)
#include "Gui.h"
#include "app.h"

#include "Lcd_Res/Lcd_Res.c"


static void DrawSpeedString(const SgWidget* widget);


static const cSgPointer Pointers[2] =
{
	{
		.head = 10,
		.tail = 2,
		.length =70,
		.start = 123-10,
		.color = 0xD63441
	},
	{
		.head = 10,
		.tail = 2,
		.length =80,
		.start = 144-10,
		.color = 0x7453A2
	}
};

static const SgConst sgConstList [] =
{
	{  // Background Speed Gauge
		.type    = SG_IMAGE,
		.layer   = 0,
		.area.pos.x  = 0,
		.area.pos.y  = 0,
		.area.width  = 250,
		.area.height = 250,
		.data        = &SpeedG_bmp_image,
		.draw        = Sg_DrawImage
	},
	{  // Background Tacho Gauge
		.type    = SG_IMAGE,
		.layer   = 0,
		.area.pos.x  = 250,
		.area.pos.y  = 0,
		.area.width  = 250,
		.area.height = 250,
		.data        = &TachoG_bmp_image,
		.draw        = Sg_DrawImage
	},
	{  // Pointer Of Speed Gauge
		.type    = SG_POINTER,
		.layer   = 1,
		.area.pos.x  = 125,
		.area.pos.y  = 125,
		.data        = &Pointers[0],
		.draw        = Sg_DrawPointer
	},
	{  // Pointer Tacho Gauge
		.type    = SG_POINTER,
		.layer   = 1,
		.area.pos.x  = 375,
		.area.pos.y  = 125,
		.data        = &Pointers[1],
		.draw        = Sg_DrawPointer
	},
	{  // Speed String
		.type    = SG_USER_DEFINE,
		.layer   = 1,
		.area.pos.x  = 90,
		.area.pos.y  = 190,
		.data        = NULL,
		.draw        = DrawSpeedString
	},
	{  // Dotmap
		.type    = SG_DOTMAP,
		.layer   = 1,
		.area.pos.x  = 0,
		.area.pos.y  = 0,
		.area.width  = 500,
		.area.height = 500,
		.data        = &guo_bmp_dotmap,
		.draw        = Sg_DrawDotMap
	},

};
static SgContext Context[6];
static const SgWidget widgets[]=
{
	{
		.pContext = &Context[0],
		.pConst   = &sgConstList[0],
	},
	{
		.pContext = &Context[1],
		.pConst   = &sgConstList[1],
	},
	{
		.pContext = &Context[2],
		.pConst   = &sgConstList[2],
	},
	{
		.pContext = &Context[3],
		.pConst   = &sgConstList[3],
	},
	{
		.pContext = &Context[4],
		.pConst   = &sgConstList[4],
	},
	{
		.pContext = &Context[5],
		.pConst   = &sgConstList[5],
	}
};
const SgConfig GuiConfigData=
{
	.widgets  = widgets,
	.number   = sizeof(widgets)/sizeof(SgWidget),
	.maxLayer = 3
};

static void DrawSpeedString(const SgWidget* widget)
{
	uint16 VehicleSpeed;
	uint8 text[32];
	Com_ReceiveSignal(COM_SID_VehicleSpeed,&VehicleSpeed);
	sprintf((char*)text,"%3d",VehicleSpeed/100);
	LCDD_DrawString(widget->pConst->area.pos.x,widget->pConst->area.pos.y,text,COLOR_RED);
}

#endif // USE_GUI
