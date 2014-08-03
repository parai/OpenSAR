/*
 * co_os.h
 *
 *  Created on: 2014Äê8ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef CO_OS_H_
#define CO_OS_H_

#include<setjmp.h>

#define X86_VC6
#define MAX_TSK 10
typedef struct {
	void (*entry)(int argc,void *argv);
	jmp_buf env;
	int argc;
	void *argv;
}TCB;
extern TCB tcb[MAX_TSK];
extern int task_num,tskid;
void init_coos(int mainstk);
int creat_tsk(void(*entry)(int argc,void *argv),int argc,void *argv,int stksize);
#define WAITFOR(condition)	do{			\
	setjmp(tcb[tskid].env);				\
	if(!(condition)){					\
		tskid++;						\
		if(tskid>=task_num)tskid=0;		\
		longjmp(tcb[tskid].env,1);		\
	}									\
}while(0)

#endif /* CO_OS_H_ */
