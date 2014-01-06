#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "isr.h"
#include "irq_types.h"

typedef uint32_t imask_t;



#define cArchIsrEnabled   0u
#define cArchIsrDisnabled 1u

typedef struct
{
	volatile imask_t 	imask;        // cArchIsrEnabled & cArchIsrDisnabled
	HANDLE 				cirticalM;    // critical section protect
	HANDLE 				mgrEvent;
	uint32_t              pendIsrB;  	  // each bit correspond a ISR;
}Arch_Type;

static Arch_Type sArch;

imask_t arch_save_int(void)
{
	imask_t mask = sArch.imask;
	sArch.imask = cArchIsrDisnabled;
	return mask;
}

void arch_restore_int(imask_t flags)
{
	sArch.imask = flags;
	if( (cArchIsrEnabled == flags) &&
		(0u != sArch.pendIsrB)       )
	{
		SetEvent(sArch.mgrEvent);
	}
}

void arch_disable_int(void)
{
	sArch.imask = cArchIsrDisnabled;
}

void arch_enable_int(void)
{
	sArch.imask = cArchIsrEnabled;
	if(0u != sArch.pendIsrB)
	{
		SetEvent(sArch.mgrEvent);
	}
}

void arch_generate_irqn(IrqType IRQn)
{
	assert( (IRQn<NUMBER_OF_INTERRUPTS_AND_EXCEPTIONS) && (IRQn > 0));
	WaitForSingleObject(sArch.cirticalM,INFINITE);
	sArch.pendIsrB |= (1UL << IRQn);
	SetEvent(sArch.mgrEvent);
	ReleaseMutex( sArch.cirticalM );
	Sleep(0);
}

ISRType Os_IsrAdd( const OsIsrConstType * restrict isrPtr ) {
	return 0;
}

static HANDLE arch_cre_secondary_thread(void (*thread_entry)(void),PVOID thread_param)
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
		assert(0);
	}
	return handle;
}
static void arch_system_timer(void)
{
	Sleep(1);
	printf(">>>  System timer starts running!    <<<\n");
	for(;;)
	{
		Sleep(1);
		WaitForSingleObject(sArch.cirticalM,INFINITE);
		sArch.pendIsrB |= (1UL << SysTick_IRQn);
		if(cArchIsrEnabled == sArch.imask)
		{
			SetEvent(sArch.mgrEvent);
		}
		ReleaseMutex(sArch.cirticalM);
	}
}

extern void OsTick(void);
extern void Can_0_IsrEntry(void);

static void arch_mgr(void)
{
	uint32_t i;
	void *pvObjectList[ 2 ];

	/* Going to block on the mutex that ensured exclusive access to the simulated
	interrupt objects, and the event that signals that a simulated interrupt
	should be processed. */
	pvObjectList[ 0 ] = sArch.cirticalM;
	pvObjectList[ 1 ] = sArch.mgrEvent;

	Sleep(1);

	printf(">>>  System manager starts running!  <<<\n");
	for(;;)
	{
		WaitForMultipleObjects( sizeof( pvObjectList ) / sizeof( void * ), pvObjectList, TRUE, INFINITE );
		for(i=0;i<NUMBER_OF_INTERRUPTS_AND_EXCEPTIONS;i++)
		{
			if(0UL != (sArch.pendIsrB&(1UL<<i)))
			{
				switch(i)
				{
					case SysTick_IRQn:
						OsTick();
						break;
					case SysCan_0_IRQn:
						Can_0_IsrEntry();
						break;
				}
				sArch.pendIsrB &= ~(1UL<<i);
			}
		}
		ReleaseMutex( sArch.cirticalM );
	}
}
static void Os_ArchInit(void) {
	memset(&sArch,0u,sizeof(sArch));
	sArch.cirticalM = CreateMutex( NULL, FALSE, NULL );
	sArch.mgrEvent  = CreateEvent( NULL, FALSE, FALSE, NULL );
	// ============= Main Thread and Secondary Thread Process
	if( GetCurrentThread() != NULL )
	{
		SetThreadPriority( GetCurrentThread(),  THREAD_PRIORITY_NORMAL);
		SetThreadPriorityBoost( GetCurrentThread(), TRUE );
		SetThreadAffinityMask( GetCurrentThread(), 0x01 );
	}
	(void)arch_cre_secondary_thread(arch_system_timer,NULL);
}

extern void start_main(void);
static void __fork(void)
{
	HANDLE thread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) start_main, NULL, 0, NULL );
	SetThreadAffinityMask( thread, 0x01 );
	SetThreadPriorityBoost( thread, TRUE );
	SetThreadPriority( thread, THREAD_PRIORITY_IDLE );
}

int main(int argc, char const *argv[])
{	
	
	Os_ArchInit();
	__fork();
	printf(">>> Welcome to the OpenBoot's world! <<<\n");
	arch_mgr();

	return 0;
}
