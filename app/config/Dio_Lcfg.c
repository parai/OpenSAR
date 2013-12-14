
#include "Dio.h"
#include "Dio_Cfg.h"

const Dio_ChannelType DioChannelConfigData[] = { 
	DIO_CHANNEL_NAME_LEDS_LED4,
	DIO_CHANNEL_NAME_LEDS_LED5,
	DIO_END_OF_LIST
};

const Dio_PortType DioPortConfigData[] = { 
	DIO_PORT_NAME_LED_PORT,  
	DIO_END_OF_LIST
};

const Dio_ChannelGroupType DioConfigData[] = {
	{ 
	  .port = DIO_END_OF_LIST, 
	  .offset = 0, 
	  .mask = 0, 
	}
};
