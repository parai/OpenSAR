
#include "app.h"

static void Refresh(void)
{
	uint16 TachoSpeed;
	uint16 VehicleSpeed;
	uint8   speed[8];
#if defined(WIN32)    
	static Stmo_DegreeType Degree = 0;
	Degree += 100;
	if(Degree > 24000)
	{
		Degree = 0;
	}
#endif    

	Com_ReceiveSignal(COM_SID_TachoSpeed,&TachoSpeed);
	Com_ReceiveSignal(COM_SID_VehicleSpeed,&VehicleSpeed);
#if defined(WIN32)
	// Range of Signal[0, 28000]km, unit in 0.01 km
	//Degree = ((uint32)VehicleSpeed*(uint32)285/(uint32)280);
	Stmo_SetPosDegree(STMO_ID_SPEED,Degree);

	//Degree = ((uint32)TachoSpeed*(uint32)250/(uint32)100);
	Stmo_SetPosDegree(STMO_ID_TACHO,Degree);
#endif
}
void app_gauge_20ms_runnable(void)
{
	Refresh();
}

