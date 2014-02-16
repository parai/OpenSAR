
// ==================================== INCLUDEs =========================
#include "Gui.h"
#include <math.h>
#include <assert.h>

// ==================================== MACROs   =========================

// ==================================== TYPEs    =========================

// ==================================== DATAs    =========================
static const SgConfig* pConfig = NULL;
// ==================================== FUNCTIONs=========================
// [[[[[[[[[[[[[[[[[[[[[[[ LOCAL START ]]]]]]]]]]]]]]]]]]]]]]]]
// Input [widget,size]; Output [top_left]
static void GetTopLeft(SgWidget* widget,const SgSize* size,SgPoint* top_left)
{
	// Widget area must be bigger than widget size

	SgAlign align= widget->pContext->align;
	top_left->x  = widget->pContext->area.pos.x;
	if(align&SG_H_CENTER_ALIGN)
	{
		top_left->x += ((int)widget->pContext->area.width - (int)size->width)/2;
	}
	else if(align&SG_RIGHT_ALIGN)
	{
		top_left->x += ((int)widget->pContext->area.width - (int)size->width);
	}

	top_left->y  = widget->pContext->area.pos.y;
	if(align&SG_V_CENTER_ALIGN)
	{
		top_left->y += ((int)widget->pContext->area.height - (int)size->height)/2;
	}
	else if(align&SG_BOTTOM_ALIGN)
	{
		top_left->y += ((int)widget->pContext->area.height - (int)size->height);
	}

}

static boolean PointWithInArea(int x,int y,SgRectange *area)
{
	if( (x>=area->pos.x) &&
	    (y>=area->pos.y) &&
	    ((x-area->pos.x)<=area->width) &&
	    ((y-area->pos.y)<=area->height))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// [[[[[[[[[[[[[[[[[[[[[[[ LOCAL END ]]]]]]]]]]]]]]]]]]]]]]]]

void Sg_Calc(int *pX,int *pY,const SgPoint* center,uint32 degree)
{ // This is difficult, I think I need a lib to do this job
	if(degree == 0)
	{
		// output by default
	}
	else
	{
		double O = (double)M_PI*degree / (double)180;
		double COS = cos(O);
		double SIN = sin(O);
		int X = pX[0];
		int Y = pY[0];

		// assert X > 0 Y > 0

		pX[0]= (int)((double)(X-center->x)*COS - (double)(Y-center->y)*SIN) + center->x;
		pY[0]= (int)((double)(X-center->x)*SIN + (double)(Y-center->y)*COS) + center->y;

	}
}
void Sg_DrawImage(SgWidget* widget)
{
	SgPoint top_left;
	const cSgImage*  image = widget->pConst->data;
	uint32 degree = (widget->pContext->degree%360);

	assert(NULL != image);
	assert(SG_IMAGE==widget->pConst->type);

	GetTopLeft(widget,&(image->size),&top_left);

	uint8* pImage= image->pixels;
	SgPoint center;
	center.x = top_left.x + image->size.width/2;
	center.y = top_left.y + image->size.height/2;

	for(int Y=top_left.y;Y<(top_left.y+image->size.height);Y++)
	{
		for(int X=top_left.x;X<(top_left.x+image->size.width);X++)
		{
			int tX,tY;
			tX=X;tY=Y;
			Sg_Calc(&tX,&tY,&center,degree);
			if(PointWithInArea(tX,tY,&(widget->pContext->area)))
			{
				uint32 dwColor = ((uint32_t)pImage[0]<<16) + ((uint32_t)pImage[1]<<8) + (pImage[2]);
				Sg_DrawPixel(tX,tY,dwColor);
			}
			pImage = pImage+3;
		}
	}
}

void Sg_DrawDotMap(SgWidget* widget)
{
	SgPoint top_left;
	cSgDotmap* dotmap = widget->pConst->data;

	assert(NULL != dotmap);
	assert(SG_DOTMAP==widget->pConst->type);

	GetTopLeft(widget,&(dotmap->size),&top_left);

	for(int Y=0;Y<(dotmap->size.height);Y++)
	{
		for(int X=0;X<(dotmap->size.width);X++)
		{
			uint8 Dot = dotmap->dots[Y*((dotmap->size.width+7)/8)+X/8];
			if(Dot&(1<<(X&7)))
			{
				Sg_DrawPixel(top_left.x+X,top_left.y+Y,widget->pContext->color);
			}
		}
	}
}

void Sg_DrawPointer(SgWidget* widget)
{
	const cSgPointer* pointer = widget->pConst->data;
	uint32 degree    = (widget->pContext->degree+pointer->start)%360;
	SgPoint* center  = &(widget->pContext->area.pos);

	assert(NULL != pointer);
	assert(SG_POINTER==widget->pConst->type);

	Sg_FillCircle(center->x,center->y,pointer->head+7,pointer->color);
	Sg_FillCircle(center->x,center->y,pointer->head,COLOR_BLACK);

	for(int r=pointer->head+5;r<=pointer->length;r++)
	{   //                      oY - sY
		// Y = k(X-sX) + sX = ----------- (X - sX) + sY
		//                      oX - sX

		int y =  (r-(pointer->head+5))*(pointer->head - pointer->tail)/2;
		y = y/(pointer->length-(pointer->head+5));
		y = pointer->head/2 - y;
		int X = r+center->x;
		for(int Y = center->y-y;Y<=center->y+y;Y++)
		{
			int tX,tY;
			tX=X;tY=Y;
			Sg_Calc(&tX,&tY,center,degree);

			Sg_DrawPixel(tX,tY,pointer->color);
			Sg_DrawPixel(tX-1,tY+1,pointer->color);
		}
	}
}
void Sg_Init(const SgConfig* config)
{
	pConfig = config;
	for(uint32 W=0;W<(pConfig->number);W++)
	{
		const SgWidget* pWidget = &(pConfig->widgets[W]);
		pWidget->pContext->color  = COLOR_WHITE;    // default color
		pWidget->pContext->degree = 0;				// default degree
		pWidget->pContext->align = SG_V_CENTER_ALIGN|SG_H_CENTER_ALIGN;
		pWidget->pContext->layer = pWidget->pConst->layer;
		memcpy(&(pWidget->pContext->area),&(pWidget->pConst->area),sizeof(SgRectange));
	}
}

Std_ReturnType Sg_SetWidgetArea(uint32 id,uint32 x,uint32 y,uint32 width,uint32 height)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		SgContext* pContext = pConfig->widgets[id].pContext;
		pContext->area.pos.x=x;
		pContext->area.pos.y=y;
		pContext->area.width=width;
		pContext->area.height=height;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}

Std_ReturnType Sg_SetWidgetDegree(uint32 id,uint32 degree)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		SgContext* pContext = pConfig->widgets[id].pContext;
		pContext->degree = degree;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}
Std_ReturnType Sg_SetWidgetLayer(uint32 id,SgLayer layer)
{
	Std_ReturnType ercd = E_OK;
	if((NULL != pConfig) && (id<pConfig->number))
	{
		SgContext* pContext = pConfig->widgets[id].pContext;
		pContext->layer = layer;
	}
	else
	{
		ercd = E_NOT_OK;
	}
	return ercd;
}
void Sg_MainFunction(void)
{
	if(NULL != pConfig)
	{
		for(uint8 layer=0;layer<pConfig->maxLayer;layer++)
		{
			for(uint32 W=0;W<pConfig->number;W++)
			{
				const SgWidget* pWidget = &pConfig->widgets[W];
				if(layer == pWidget->pContext->layer)
				{
					if(NULL != pWidget->pConst->draw)
					{
						pWidget->pConst->draw((void*)pWidget);
					}
					else
					{

					}
				}
			}
		}
	}
}
