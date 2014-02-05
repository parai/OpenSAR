#include "Gui.h"

static GuiWidget_Type widgets[2];
const GuiConfig_Type GuiConfigData=
{
	.widgets = widgets,
	.number  = sizeof(widgets)/sizeof(GuiWidget_Type)
};
