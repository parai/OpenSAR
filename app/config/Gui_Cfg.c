#include "Gui.h"

#include "Lcd_Res/Lcd_Res.c"


static GuiWidgetContext_Type Context[2];
static const GuiWidget_Type widgets[2]=
{
	{
		.pContext=&Context[0],
		.defaultContext.layer = 0,
		.defaultContext.area.top_left.x = 0,
		.defaultContext.area.top_left.y = 0,
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 30,
		.center.x                   = 97,
		.center.y                   = 90,
		.image                      = &IMG0_image,  //SpeedG
	},
	{
		.pContext=&Context[1],
		.defaultContext.layer = 0,
		.defaultContext.area.top_left.x = 250,
		.defaultContext.area.top_left.y = 0,
		.defaultContext.area.width = 0,  // If Zero, use the default of Image
		.defaultContext.area.height = 0, // If Zero, use the default of Image
		.defaultContext.degree      = 0,
		.center.x                   = 107,
		.center.y                   = 104,
		.image                      = &IMG1_image,  //TachoG
	}
};
const GuiConfig_Type GuiConfigData=
{
	.widgets = widgets,
	.number  = sizeof(widgets)/sizeof(GuiWidget_Type)
};
