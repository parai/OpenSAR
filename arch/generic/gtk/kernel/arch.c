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
extern OsTaskVarType Os_TaskVarList[OS_TASK_CNT];
// ====================================== TYPEs ====================================
static GTimer* pSysTimer;
// ====================================== DATAs ====================================


// ====================================== LOCAL FUNCTIONs ====================================
static void arch_task_runnable(void)
{
	// In this arch, no support of ECC
	if((NULL!=Os_Sys.currTaskPtr) && (ST_RUNNING == Os_Sys.currTaskPtr->state))
	{	// TODO: support of WAITING is not supported
		Os_Sys.currTaskPtr->constPtr->entry();
	}
}
static void arch_system_timer(void)
{
	gdouble elapsed;
	gulong  elapsed_microseconds;

	elapsed = g_timer_elapsed(pSysTimer,NULL); // unit in
	elapsed_microseconds = (elapsed*1000);
	while(elapsed_microseconds>0)
	{
		elapsed_microseconds--;
		OsTick();
		if(0==elapsed_microseconds)
		{
			g_timer_start(pSysTimer);
		}
	}
}
static gboolean arch_daemon(gpointer data)
{
	gdk_threads_enter();
	arch_system_timer();
	arch_task_runnable();
	gdk_threads_leave();
	return TRUE;
}
static void arch_init_daemon(void)
{
	start_main();

	g_thread_init(NULL);
	//
	pSysTimer = g_timer_new();
	g_idle_add(arch_daemon,NULL);

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

	arch_init_daemon();
	gtk_main ();
	return ( 0 ) ;
}
