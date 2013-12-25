
#ifndef COMM_CFG_H_H_H
#define COMM_CFG_H_H_H

#define COMM_VERSION_INFO_API STD_ON
#define COMM_DEV_ERROR_DETECT STD_ON
#define COMM_NO_COM STD_OFF
#define COMM_MODE_LIMITATION_ENABLED STD_ON
#define COMM_T_MIN_FULL_COM_MODE_DURATION 1000

#define COMM_CHANNEL_COUNT 2
#define COMM_CHL_LS 0
#define COMM_CHL_HS 1

typedef enum
{
	COMM_LS_USER,
	COMM_HS_USER,
	COMM_USER_COUNT
}ComM_UserHandleType;

extern const ComM_ConfigType ComM_Cfg;

#endif /* COMM_CFG_H_H_H */

