
#include "app.h"

static void Refresh(void)
{
	uint16 TachoSpeed;
	uint16 VehicleSpeed;
	Stmo_DegreeType Degree;

	Com_ReceiveSignal(COM_SID_TachoSpeed,&TachoSpeed);
	Com_ReceiveSignal(COM_SID_VehicleSpeed,&VehicleSpeed);

	// Range of Signal[0, 28000]km, unit in 0.01 km
	Degree = ((uint32)VehicleSpeed*(uint32)285/(uint32)280);
	Stmo_SetPosDegree(STMO_ID_SPEED,Degree);
	Degree = ((uint32)TachoSpeed*(uint32)250/(uint32)100);
	Stmo_SetPosDegree(STMO_ID_TACHO,Degree);

}
void app_gauge_20ms_runnable(void)
{
	Refresh();
}

