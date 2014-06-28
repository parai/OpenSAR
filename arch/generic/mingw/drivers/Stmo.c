
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "Stmo.h"
// ====================== LOCAL TYPE  =========================
typedef struct
{
	Stmo_DegreeType Current;
	Stmo_DegreeType Command;
}Stmo_ControllerType;
// ====================== LOCAL MACRO =========================

#define VALIDATE_ID(Id,_api)		\
	if((Id) > STMO_CFG_NUM)			\
	{								\
		goto cleanup;				\
	}

#define GET_STMO_SOFTWARE_ZERO(Id)	\
		(pConfig->Channels[Id].SoftwareZero)

// ====================== LOCAL DATA ==========================

static Stmo_ControllerType  StmoCtrl[STMO_CFG_NUM];

static const Stmo_ConfigType* pConfig = NULL;

// ====================== LOCAL FUNCTION ======================

// ====================== GLOBAL FUNCTION ======================
void Stmo_Init(const Stmo_ConfigType *Config)
{
	pConfig = Config;
	memset(StmoCtrl,0,sizeof(StmoCtrl));
}

Std_ReturnType Stmo_SetPosDegree(Stmo_IdType Id,Stmo_DegreeType Degree)
{
	Std_ReturnType ercd = E_NOT_OK;
	VALIDATE_ID(Id,Stmo_SetPosDegree);

	if(Degree <= STMO_MAX_DEGREE)
	{
		StmoCtrl[Id].Command = Degree+GET_STMO_SOFTWARE_ZERO(Id);
		ercd = E_OK;
	}

cleanup:
	return ercd;
}

// Period In 1ms
void Stmo_MainFunction(void)
{
	for(int i=0;i<STMO_CFG_NUM;i++)
	{
		if(StmoCtrl[i].Command != StmoCtrl[i].Current)
		{
			if(StmoCtrl[i].Command > StmoCtrl[i].Current)
			{
				if(StmoCtrl[i].Command > (StmoCtrl[i].Current+STMO_ONE_STEP))
				{
					StmoCtrl[i].Current= StmoCtrl[i].Current+STMO_ONE_STEP;
				}
				else
				{
					StmoCtrl[i].Current = StmoCtrl[i].Command;
				}
			}
			else if(StmoCtrl[i].Command < StmoCtrl[i].Current)
			{
				if(StmoCtrl[i].Command < (StmoCtrl[i].Current-STMO_ONE_STEP))
				{
					StmoCtrl[i].Current= StmoCtrl[i].Current-STMO_ONE_STEP;
				}
				else
				{
					StmoCtrl[i].Current = StmoCtrl[i].Command;
				}
			}
			else
			{
				StmoCtrl[i].Current = StmoCtrl[i].Command;
			}
		}
	}
}
