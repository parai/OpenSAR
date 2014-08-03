/*
 * co_os.c
 *
 *  Created on: 2014Äê8ÔÂ3ÈÕ
 *      Author: Administrator
 */


#include "co_os.h"

#define SAVE_SP(p) 		__asm__ ( "movl %%esp,%0":"=m" ( p ) )
#define RESTORE_SP(p)	__asm__ ( "movl %0,%%esp":"=m" ( p ) )

TCB tcb[MAX_TSK];
int task_num=1;
int tskid;
static int stktop,oldsp;
void init_coos(int mainstk){
	SAVE_SP(stktop);
	stktop=stktop+sizeof(void(*)(void))/sizeof(int)-(mainstk+sizeof(int)-1)/sizeof(int);
}
int creat_tsk(void(*entry)(int argc,void *argv),
int argc,void *argv,int stksize){
	if(task_num>=MAX_TSK)return -1;
		SAVE_SP(oldsp);
		RESTORE_SP(stktop);
	if(!setjmp(tcb[task_num].env)){
		RESTORE_SP(oldsp);
		tcb[task_num].entry=entry;
		tcb[task_num].argc=argc;
		tcb[task_num].argv=argv;
		task_num++;
		stktop-=(stksize+sizeof(int)-1)/sizeof(int);
	}
	else tcb[tskid].entry(tcb[tskid].argc,tcb[tskid].argv);
	return 0;
}
