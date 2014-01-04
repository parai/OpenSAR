#ifndef OS_CFG_H
#define OS_CFG_H

#define TASK_ID_task1ms  		(0)
#define TASK_ID_task5ms  		(1)
#define TASK_ID_task10ms  		(2)
#define TASK_ID_task50ms  		(3)
#define TASK_ID_task100ms 		(4)
#define TASK_ID_task500ms 		(5)
#define TASK_ID_task1000ms 		(6)
#define cfgOsMaxTaskId          (7)
#define OS_TASK_CNT             (cfgOsMaxTaskId)    

extern void task1ms(void);
extern void task5ms(void);
extern void task10ms(void);
extern void task50ms(void);
extern void task100ms(void);
extern void task500ms(void);
extern void task1000ms(void);

#define GEN_TASK_LIST()                	 	\
static const task_main_t  TaskList[] = { 	\
	task1ms,								\
	task5ms,								\
	task10ms,								\
	task50ms,								\
	task100ms,								\
	task500ms,								\
	task1000ms,								\
}


#define Ms2AlarmTick(ms)        (ms)

#define ALARM_ID_task1ms  		(0)
#define ALARM_ID_task5ms  		(1)
#define ALARM_ID_task10ms  		(2)
#define ALARM_ID_task50ms  		(3)
#define ALARM_ID_task100ms 		(4)
#define ALARM_ID_task500ms 		(5)
#define ALARM_ID_task1000ms 	(6)    
#define cfgOsMaxAlarmId         (7)

#define GEN_ALARM_LIST()					\
static const alarm_main_t AlarmList[] = {	\
	TASK_ID_task1ms,						\
	TASK_ID_task5ms,						\
	TASK_ID_task10ms,						\
	TASK_ID_task50ms,						\
	TASK_ID_task100ms,						\
	TASK_ID_task500ms,						\
	TASK_ID_task1000ms,						\
}								



#endif // OS_CFG_H

