
#ifndef APP_TIME_H_
#define APP_TIME_H_

typedef struct
{
	uint16 year;
	uint8  month;
	uint8  day;
	uint8  hour;
	uint8  minute;
	uint8  second;
}App_TimeType;

extern void app_time_1000ms_runnable(void);

#endif /* APP_TIME_H_ */
