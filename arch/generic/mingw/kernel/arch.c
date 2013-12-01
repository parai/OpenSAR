#include <windows.h>
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

/* The WIN32 simulator runs each task in a thread.  The context switching is
managed by the threads, so the task stack does not have to be managed directly,
although the task stack is still used to hold an xThreadState structure this is
the only thing it will ever hold.  The structure indirectly maps the task handle
to a thread handle. */
typedef struct
{
	/* Handle of the thread that executes the task. */
	void *pvThread;

} xThreadState;

typedef struct
{
	volatile imask_t 	imask; // cArchIsrEnabled & cArchIsrDisnabled
	HANDLE 				cirticalM; // critical section protect
	HANDLE 				mgrEvent;
	volatile uint32_t   mgrState;
	uint32 pendIsrB;  		// each bit correspond a ISR;
	OsTaskVarType*      old;
	OsTaskVarType*      new;
	boolean		 		isMgrRunning; // True & False

}Aach_Type;

static Aach_Type sArch;

void OsIdle(void)
{
	for(;;)
	{
		if(cArchIsrDisnabled == sArch.imask )
		{
			sArch.imask = cArchIsrEnabled;
			// printf("System Error, as ISR disabled\n");
		}
		Sleep(1);
	}
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

HANDLE arch_cre_secondary_thread(DWORD (*thread_entry)(PVOID),PVOID thread_param)
{
	/* Start the thread that simulates the IRQn peripheral to generate
	interrupts.  The priority is set below that of the simulated
	interrupt handler so the interrupt event mutex is used for the
	handshake / overrun protection. */
	HANDLE handle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)thread_entry, (PVOID)thread_param, 0, NULL );
	if( handle != NULL )
	{
		SetThreadPriority( handle, THREAD_PRIORITY_BELOW_NORMAL );
		SetThreadPriorityBoost( handle, TRUE );
		SetThreadAffinityMask( handle, 0x01 );
	}
	else
	{
		assert(FALSE);
	}
	return handle;
}
static DWORD arch_system_timer(PVOID param)
{
	while(FALSE == sArch.isMgrRunning)
	{
		Sleep(1); // wait till manager is running.
	}
	for(;;)
	{
		Sleep(1);
		WaitForSingleObject(sArch.cirticalM,INFINITE);
		if(cMgrIdle == sArch.mgrState)
		{
			sArch.mgrState = cMgrIsrReq;
			sArch.pendIsrB |= (1UL << SysTick_IRQn);
			SetEvent(sArch.mgrEvent);
		}
		ReleaseMutex(sArch.cirticalM);
	}
	return 0;
}
void Os_ArchInit(void) {
	memset(&sArch,0u,sizeof(sArch));
	sArch.cirticalM = CreateMutex( NULL, FALSE, NULL );
	sArch.mgrEvent  = CreateEvent( NULL, FALSE, FALSE, NULL );
	// ============= Main Thread and Secondary Thread Process
	if( GetCurrentThread() != NULL )
	{
		SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
		SetThreadPriorityBoost( GetCurrentThread(), TRUE );
		SetThreadAffinityMask( GetCurrentThread(), 0x01 );
	}
	arch_cre_secondary_thread(arch_system_timer,NULL);
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
	xThreadState *pxThreadState = NULL;
	void* pxCode;
	/* In this simulated case a stack is not initialised, but instead a thread
	is created that will execute the task being created.  The thread handles
	the context switching itself.  The xThreadState object is placed onto
	the stack that was created for the task - so the stack buffer is still
	used, just not in the conventional way.  It will not be used for anything
	other than holding this structure. */
	pxThreadState = ( xThreadState * ) ( pcb->stack.curr - sizeof( xThreadState ) );

	// see STACK_PATTERN = 0x42 in arch.h
	if( (0x42424242UL != (uint32_t)pxThreadState->pvThread)
		&& (0UL != (uint32_t)pxThreadState->pvThread) )
	{
		TerminateThread(pxThreadState->pvThread,0);
		// TODO: ChainTask is not allowed and also multiply-activation
	}

	if( pcb->constPtr->proc_type == PROC_EXTENDED ) {
		pxCode = (void*)Os_TaskStartExtended;
	} else if( pcb->constPtr->proc_type == PROC_BASIC ) {
		pxCode = (void*)Os_TaskStartBasic;
	}
	else
	{
		pxCode = (void*)Os_TaskStartBasic;
	}
	/* Create the thread itself. */
	pxThreadState->pvThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) pxCode, NULL, CREATE_SUSPENDED, NULL );
	SetThreadAffinityMask( pxThreadState->pvThread, 0x01 );
	SetThreadPriorityBoost( pxThreadState->pvThread, TRUE );
	SetThreadPriority( pxThreadState->pvThread, THREAD_PRIORITY_IDLE );

	pcb->stack.curr = ( void * ) pxThreadState;
}
void Os_ArchSetSpAndCall(void *sp, void (*f)(void) ) {
	printf("Os_ArchSetSpAndCall\n");
}
void Os_ArchSwapContext(OsTaskVarType *old,OsTaskVarType *new) {
	if(cMgrIsrReq == sArch.mgrState)
	{  // A switch In ISR requested. So what ...
		sArch.mgrState = cMgrSwapContext;
		sArch.old = old;
		sArch.new = new;
	}
	else
	{
		while(cMgrIdle != sArch.mgrState) Sleep(0);
		WaitForSingleObject(sArch.cirticalM,INFINITE);
		assert(cMgrIdle == sArch.mgrState);
		sArch.mgrState = cMgrSwapContext;
		sArch.old = old;
		sArch.new = new;
		SetEvent(sArch.mgrEvent);
		ReleaseMutex( sArch.cirticalM );
	}
}
static void arch_swap_context(OsTaskVarType *old,OsTaskVarType *new){
	xThreadState *pxThreadState = NULL;
	pxThreadState = ( xThreadState * ) old->stack.curr;
	SuspendThread(pxThreadState->pvThread);

	sArch.imask = cArchIsrEnabled;
	pxThreadState = ( xThreadState * ) new->stack.curr;
	ResumeThread(pxThreadState->pvThread);
	sArch.mgrState = cMgrIdle;
}
static void arch_swap_context_to(OsTaskVarType *old,OsTaskVarType *new){
	xThreadState *pxThreadState = NULL;
	sArch.imask = cArchIsrEnabled;
	pxThreadState = ( xThreadState * ) new->stack.curr;
	ResumeThread(pxThreadState->pvThread);
	sArch.mgrState = cMgrIdle;
}
static void arch_isr_suspend_curtsk(void)
{
	xThreadState *pxThreadState = NULL;
	OsTaskVarType *curtsk = Os_SysTaskGetCurr();

	pxThreadState = ( xThreadState * ) curtsk->stack.curr;
	SuspendThread(pxThreadState->pvThread);
}
static void arch_isr_resume_curtsk(void)
{
	xThreadState *pxThreadState = NULL;
	OsTaskVarType *curtsk = Os_SysTaskGetCurr();

	pxThreadState = ( xThreadState * ) curtsk->stack.curr;
	ResumeThread(pxThreadState->pvThread);
}
static void arch_isr_process(void)
{
	uint32_t i;
	if(cArchIsrEnabled == sArch.imask)
	{
		for(i=0;i<NUMBER_OF_INTERRUPTS_AND_EXCEPTIONS;i++)
		{
			if(0UL != (sArch.pendIsrB&(1UL<<i)))
			{
				if(SysTick_IRQn == i)
				{
					OsTick();
				}
				else
				{
					// better do suspend curtsk for make sure only isr is running
					arch_isr_suspend_curtsk();
					Os_Isr(NULL,i);
					arch_isr_resume_curtsk();
				}
				sArch.pendIsrB &= ~(1UL<<i);
			}
		}
	}
	if(cMgrIsrReq == sArch.mgrState)
	{
		sArch.mgrState = cMgrIdle;
	}
	else if( cMgrSwapContext == sArch.mgrState)
	{
		arch_swap_context(sArch.old,sArch.new);
	}
}
static void arch_os_manager(void)
{
	void *pvObjectList[ 2 ];

	/* Going to block on the mutex that ensured exclusive access to the simulated
	interrupt objects, and the event that signals that a simulated interrupt
	should be processed. */
	pvObjectList[ 0 ] = sArch.cirticalM;
	pvObjectList[ 1 ] = sArch.mgrEvent;
	for(;;)
	{
		WaitForMultipleObjects( sizeof( pvObjectList ) / sizeof( void * ), pvObjectList, TRUE, INFINITE );
		switch(sArch.mgrState)
		{
			case cMgrSwapContext2:
				arch_swap_context_to(sArch.old,sArch.new);
				break;
			case cMgrSwapContext:
				arch_swap_context(sArch.old,sArch.new);
				break;
			case cMgrIsrReq:
				arch_isr_process();
				break;
			default:
				sArch.mgrState = cMgrIdle;
				break;
		}
		ReleaseMutex( sArch.cirticalM );
	}
}
void Os_ArchSwapContextTo(OsTaskVarType *old,OsTaskVarType *new){
	// Here is the entry point to start Task
	while(cMgrIdle != sArch.mgrState) Sleep(0);
	WaitForSingleObject(sArch.cirticalM,INFINITE);
	assert(cMgrIdle == sArch.mgrState);
	sArch.mgrState = cMgrSwapContext2;
	sArch.old = old;
	sArch.new = new;
	SetEvent(sArch.mgrEvent);
	ReleaseMutex( sArch.cirticalM );
	Sleep(0);
	if(FALSE == sArch.isMgrRunning)
	{
		sArch.isMgrRunning = TRUE;
		printf("^_^ Enjoy This Tool by parai@foxmail.com ^_^\n");
		arch_os_manager();
	}
}
void *Os_ArchGetStackPtr( void ) {

	return NULL; // Simualte on Win32
}

void arch_generate_irqn(IrqType IRQn)
{
	assert( (IRQn<NUMBER_OF_INTERRUPTS_AND_EXCEPTIONS) && (IRQn > 0));
	while(cMgrIdle != sArch.mgrState) Sleep(0);
	WaitForSingleObject(sArch.cirticalM,INFINITE);
	if(cMgrIdle == sArch.mgrState)
	{
		sArch.pendIsrB |= (1UL << IRQn);
		sArch.mgrState = cMgrIsrReq;
		SetEvent(sArch.mgrEvent);
	}
	else
	{
		// so waht maybe a system error
		printf("X");
	}
	ReleaseMutex( sArch.cirticalM );
}


