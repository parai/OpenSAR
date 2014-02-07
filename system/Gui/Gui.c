
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
static void GetImageTopLeft(SgWidget* widget,SgPoint* top_left)
{

	// Widget area must be bigger than Image Area
	const SGIMPImage*  image = widget->pConst->data;
	SgAlign align= widget->pContext->align;
	top_left->x  = widget->pContext->area.pos.x;
	if(align&SG_H_CENTER_ALIGN)
	{
		top_left->x += (widget->pContext->area.width - image->dwWidth)/2;
	}
	else if(align&SG_RIGHT_ALIGN)
	{
		top_left->x += (widget->pContext->area.width - image->dwWidth);
	}

	top_left->y  = widget->pContext->area.pos.y;
	if(align&SG_V_CENTER_ALIGN)
	{
		top_left->y += (widget->pContext->area.height - image->dwHeight)/2;
	}
	else if(align&SG_BOTTOM_ALIGN)
	{
		top_left->y += (widget->pContext->area.height - image->dwHeight);
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

		pX[0]= (int)((double)(X)*COS - (double)(Y)*SIN);
		pY[0]= (int)((double)(X)*SIN + (double)(Y)*COS);

		pX[0] += center->x*(1-COS) + center->y*SIN;
		pY[0] -= center->x*SIN     - center->y*(1-COS);
	}
}
void Sg_DrawImage(SgWidget* widget)
{

	const SGIMPImage*  image = widget->pConst->data;
	uint32 degree = (widget->pContext->degree%360);
	SgPoint top_left;

	assert(SG_IMAGE==widget->pConst->type);

	GetImageTopLeft(widget,&top_left);

	uint8* pImage= image->pucPixel_data;
	SgPoint center;
	center.x = top_left.x + image->dwWidth/2;
	center.y = top_left.y + image->dwHeight/2;

	for(int Y=top_left.y;Y<(top_left.y+image->dwHeight);Y++)
	{
		for(int X=top_left.x;X<(top_left.x+image->dwWidth);X++)
		{
			int tX,tY;
			tX=X;tY=Y;
			Sg_Calc(&tX,&tY,&center,degree);
			if(PointWithInArea(tX,tY,&(widget->pContext->area)))
			{
				uint32 dwColor = ((uint32_t)pImage[0]<<16) + ((uint32_t)pImage[1]<<8) + (pImage[2]);
				LCDD_DrawPixel(tX,tY,dwColor);
				LCDD_DrawPixel(tX,tY+1,dwColor);
			}
			pImage = pImage+3;
		}
	}
}
void Sg_DrawPointer(SgWidget* widget)
{
	const cSgPointer* pointer = widget->pConst->data;
	uint32 degree    = (widget->pContext->degree+pointer->start)%360;
	SgPoint* center  = &(widget->pContext->area.pos);
	assert(SG_POINTER==widget->pConst->type);
	for(int r=pointer->head;r<pointer->head+7;r++)
	{
		LCDD_DrawCircle(center->x,center->y,r,pointer->color);
		LCDD_DrawCircle(center->x,center->y+1,r,pointer->color);
	}

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

			LCDD_DrawPixel(tX,tY,pointer->color);
			LCDD_DrawPixel(tX-1,tY+1,pointer->color);
		}
	}
}
void Sg_Init(const SgConfig* config)
{
	pConfig = config;
	for(uint32 W=0;W<(pConfig->number);W++)
	{
		const SgWidget* pWidget = &(pConfig->widgets[W]);
		pWidget->pContext->degree = 0;
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
