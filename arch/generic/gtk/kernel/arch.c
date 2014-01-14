#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "Os.h"
#include "internal.h"
#include "sys.h"
#include "irq_types.h"

// ====================================== MACROs ====================================

// ==================================== EXTENDs ===================================
extern void start_main(void);
// ====================================== TYPEs ====================================

// ====================================== DATAs ====================================


// ====================================== LOCAL FUNCTIONs ====================================
static void arch_task_runnable(OsTaskVarType* pcb)
{
	int taskid = pcb->constPtr->pid;
	if(ST_READY == pcb->state)
	{	// TODO: support of WAITING is not supported
		if( pcb->constPtr->proc_type == PROC_EXTENDED ) {
			Os_TaskStartExtended();
		} else if( pcb->constPtr->proc_type == PROC_BASIC ) {
			Os_TaskStartBasic();
		}
		else
		{
			g_assert(0);
		}
	}
}
static GTimeVal g_time_old;
static void arch_system_timer(void)
{

	GTimeVal time_new;
	g_get_current_time(&time_new);
	if(time_new.tv_usec>(g_time_old.tv_usec+1000))
	{
		OsTick();
		memcpy(&g_time_old,&time_new,sizeof(GTimeVal));
	}
	else if((0xFFFFFFFFL-g_time_old.tv_usec+time_new.tv_usec) > 1000)//maybe overflow
	{
		OsTick();
		memcpy(&g_time_old,&time_new,sizeof(GTimeVal));
	}
}

static void arch_init_daemon(void)
{
	start_main();
	// --------------- create runnable
	g_idle_add(arch_system_timer,NULL);

	for(int i=0;i<OS_TASK_CNT;i++)
	{
		g_idle_add(arch_task_runnable,&Os_TaskVarList[i]);
	}
	g_get_current_time(&g_time_old);
}

// ====================================== FUNCTIONs ====================================
void OsIdle(void)
{
}
imask_t arch_save_int(void)
{
	return 0;
}
void arch_restore_int(imask_t flags)
{
}
void arch_disable_int(void)
{

}
void arch_enable_int(void)
{

}

void Os_ArchInit(void) {
}
void Os_ArchFirstCall( void ) {
	// TODO: make switch here... for now just call func.
	Irq_Enable();
	Os_Sys.currTaskPtr->constPtr->entry();
}
unsigned int Os_ArchGetScSize( void ) {
	return 0u;
}
void Os_ArchSetTaskEntry(OsTaskVarType *pcbPtr ) {
}

void Os_ArchSetupContext( OsTaskVarType *pcb ) {

}
void Os_ArchSetSpAndCall(void *sp, void (*f)(void) ) {

}
void Os_ArchSwapContext(OsTaskVarType *old,OsTaskVarType *new) {

}

void Os_ArchSwapContextTo(OsTaskVarType *old,OsTaskVarType *new){

}
void *Os_ArchGetStackPtr( void ) {

	return NULL; // Simualte on Win32
}
int main( int argc, char *argv[] )
{
	GtkWidget *window;
	gtk_init (&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_show (window);

	// threads start
	arch_init_daemon();
	// threads end
	gtk_main ();
	return ( 0 ) ;
}
