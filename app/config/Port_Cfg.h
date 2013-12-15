
#if !(((PORT_SW_MAJOR_VERSION == 1) && (PORT_SW_MINOR_VERSION == 0)) )
#error Port: Configuration file expected BSW module version to be 1.0.*
#endif


#ifndef PORT_CFG_H_
#define PORT_CFG_H_

#include "Std_Types.h"

#define	PORT_VERSION_INFO_API				STD_ON

#if defined(USE_DET)
#define	PORT_DEV_ERROR_DETECT				STD_ON
#else
#define	PORT_DEV_ERROR_DETECT				STD_OFF
#endif

#define PORT_SET_PIN_MODE_API				STD_ON
#define PORT_SET_PIN_DIRECTION_API	        STD_ON

#define		PORT_BIT0			(1<<15)
#define		PORT_BIT1			(1<<14)
#define		PORT_BIT2			(1<<13)
#define		PORT_BIT3			(1<<12)
#define		PORT_BIT4			(1<<11)
#define		PORT_BIT5			(1<<10)
#define		PORT_BIT6			(1<<9)
#define		PORT_BIT7			(1<<8)
#define		PORT_BIT8			(1<<7)
#define		PORT_BIT9			(1<<6)
#define		PORT_BIT10			(1<<5)
#define		PORT_BIT11			(1<<4)
#define		PORT_BIT12			(1<<3)
#define		PORT_BIT13			(1<<2)
#define		PORT_BIT14			(1<<1)
#define		PORT_BIT15			(1<<0)

#define		PORT_WPE_BIT		PORT_BIT14
#define		PORT_WPS_BIT		PORT_BIT15
#define		PORT_SRC0			PORT_BIT12
#define		PORT_SRC1			PORT_BIT13

// For this MinGW Only IO
#define     PORT_PCR_INPUT      (PORT_PIN_IN)
#define     PORT_PCR_OUTPUT     (PORT_PIN_OUT)

#define		PORT_PCR_RESET		(PORT_PCR_INPUT)

#define		PORT_GPDO_RESET		(0)	// The default value for PIN

#define		PORT_GPDO_HIGH		(1)


typedef enum
{
	PortA_PIN0,
	PortA_PIN1,
	PortA_PIN2,
	PortA_PIN3,
	PortA_PIN4,
	PortA_PIN5,
	PortA_PIN6,
	PortA_PIN7,
	PortB_PIN0,
	PortB_PIN1,
	PortB_PIN2,
	PortB_PIN3,
	PortB_PIN4,
	PortB_PIN5,
	PortB_PIN6,
	PortB_PIN7,
	PortC_PIN0,
	PortC_PIN1,
	PortC_PIN2,
	PortC_PIN3,
	PortC_PIN4,
	PortC_PIN5,
	PortC_PIN6,
	PortC_PIN7,
	PortD_PIN0,
	PortD_PIN1,
	PortD_PIN2,
	PortD_PIN3,
	PortD_PIN4,
	PortD_PIN5,
	PortD_PIN6,
	PortD_PIN7,
	PortE_PIN0,
	PortE_PIN1,
	PortE_PIN2,
	PortE_PIN3,
	PortE_PIN4,
	PortE_PIN5,
	PortE_PIN6,
	PortE_PIN7,
	PortF_PIN0,
	PortF_PIN1,
	PortF_PIN2,
	PortF_PIN3,
	PortF_PIN4,
	PortF_PIN5,
	PortF_PIN6,
	PortF_PIN7,
	PortG_PIN0,
	PortG_PIN1,
	PortG_PIN2,
	PortG_PIN3,
	PortG_PIN4,
	PortG_PIN5,
	PortG_PIN6,
	PortG_PIN7,
	PortH_PIN0,
	PortH_PIN1,
	PortH_PIN2,
	PortH_PIN3,
	PortH_PIN4,
	PortH_PIN5,
	PortH_PIN6,
	PortH_PIN7,
	PortI_PIN0,
	PortI_PIN1,
	PortI_PIN2,
	PortI_PIN3,
	PortI_PIN4,
	PortI_PIN5,
	PortI_PIN6,
	PortI_PIN7,
	PortJ_PIN0,
	PortJ_PIN1,
	PortJ_PIN2,
	PortJ_PIN3,
	PortJ_PIN4,
	PortJ_PIN5,
	PortJ_PIN6,
	PortJ_PIN7,
	PortK_PIN0,
	PortK_PIN1,
	PortK_PIN2,
	PortK_PIN3,
	PortK_PIN4,
	PortK_PIN5,
	PortK_PIN6,
	PortK_PIN7,
	Port_PIN_NUM
}Port_PinType;

typedef struct
{
	uint16_t padCnt;
	const uint8_t *padConfig;
	uint16_t outCnt;
	const uint8_t *outConfig;
} Port_ConfigType;

extern const Port_ConfigType PortConfigData;

#endif /* PORT_CFG_H_ */
