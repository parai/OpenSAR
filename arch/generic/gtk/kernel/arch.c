#include <gtk/gtk.h>
#include "Os.h"
#include "internal.h"
#include "sys.h"
#include "irq_types.h"
#define cArchIsrEnabled   0u
#define cArchIsrDisnabled 1u

// Manager State
#define cMgrIdle		  0u
#define cMgrSwapContext   1u
#define cMgrSwapContext2  2u
#define cMgrIsrReq        3u // service ISR

typedef struct
{
	volatile imask_t 	imask; // cArchIsrEnabled & cArchIsrDisnabled
//	HANDLE 				cirticalM; // critical section protect
//	HANDLE 				mgrEvent;
	volatile uint32_t   mgrState;
	uint32 pendIsrB;  		// each bit correspond a ISR;
	OsTaskVarType*      old;
	OsTaskVarType*      new;
	boolean		 		isMgrRunning; // True & False

}Aach_Type;

static Aach_Type sArch;

void OsIdle(void)
{
}
imask_t arch_save_int(void)
{
	imask_t mask = sArch.imask;
	sArch.imask = cArchIsrDisnabled;
	return mask;
}
void arch_restore_int(imask_t flags)
{
	sArch.imask = flags;
}
void arch_disable_int(void)
{
	sArch.imask = cArchIsrDisnabled;
}
void arch_enable_int(void)
{
	sArch.imask = cArchIsrEnabled;
}

void Os_ArchInit(void) {
	memset(&sArch,0u,sizeof(sArch));
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
	// printf("Os_ArchSetTaskEntry\n");
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
	gtk_main ();
	return ( 0 ) ;
}
