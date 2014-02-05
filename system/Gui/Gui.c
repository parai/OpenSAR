
// ==================================== INCLUDEs =========================
#include "Gui.h"

// ==================================== MACROs   =========================

// ==================================== TYPEs    =========================

// ==================================== DATAs    =========================
static const GuiConfig_Type* pConfig = NULL;
// ==================================== FUNCTIONs=========================
void Gui_Init(const GuiConfig_Type* config)
{
	pConfig = config;
	for(uint32 W=0;W<(pConfig->number);W++)
	{
		GuiWidget_Type* pWidget = &(pConfig->widgets[W]);
		pWidget->layer = GUI_NOT_VISIBLE;
		pWidget->image = NULL;
		pWidget->area.top_left.x=0;
		pWidget->area.top_left.y=0;
		pWidget->area.width=100;
		pWidget->area.height=100;
	}
}
Std_ReturnType Gui_SetWidgetImage(uint32 id,GuiImage_Type* image)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		GuiWidget_Type* pWidget = &(pConfig->widgets[id]);
		pWidget->image = image;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}
Std_ReturnType Gui_SetWidgetArea(uint32 id,uint32 x,uint32 y,uint32 width,uint32 height)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		GuiWidget_Type* pWidget = &(pConfig->widgets[id]);
		pWidget->area.top_left.x=x;
		pWidget->area.top_left.y=y;
		pWidget->area.width=width;
		pWidget->area.height=height;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}
Std_ReturnType Gui_SetWidgetLayer(uint32 id,uint8 layer)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		GuiWidget_Type* pWidget = &(pConfig->widgets[id]);
		pWidget->layer = layer;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}
void Gui_MainFunction(void)
{
	if(NULL != pConfig)
	{
		for(uint8 layer=0;layer<GUI_MAX_LAYERS;layer++)
		{
			for(uint32 W=0;W<pConfig->number;W++)
			{
				GuiWidget_Type* pWidget = &(pConfig->widgets[W]);
				if(layer == pWidget->layer)
				{
					LCDD_DrawGIMPImage(pWidget->area.top_left.x,
								pWidget->area.top_left.y,
								pWidget->image);
				}
			}
		}
	}
}
