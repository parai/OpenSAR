#include <stdlib.h>
#include "co_os.h"
void tskfunc1(int argc,void *argv);
void tskfunc2(int argc,void *argv);
void subfunc(void);
volatile int cnt,test;
int main(void){
	int i;
	init_coos(400);
	creat_tsk(tskfunc1,12,NULL,400);
	creat_tsk(tskfunc2,0,NULL,400);
	i=0;
	while(1){
		WAITFOR(cnt==8);
		while(i++<cnt)test=i;
		cnt++;
	}
}
void tskfunc1(int argc,void *argv){
	int i;
	static int creat=0;
	if(!creat){
	creat_tsk(tskfunc1,9,NULL,400);
	creat=1;
	}
	i=0;
	while(1){
		WAITFOR(cnt>argc);
		test=0x55;

		subfunc();
		while(i++<cnt)test=i^0xAA;
	}
}
void tskfunc2(int argc,void *argv){
	while(1){
		WAITFOR(++cnt>15);
		cnt=0;
	}
}
void subfunc(void){
	int i;
	WAITFOR(cnt<5);
	for(i=0;i<100;i++)test=0x10*i;
}
