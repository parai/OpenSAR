#include "Std_Types.h"
#include "irq_types.h"
#include "Os.h"
#include "isr.h"
void Os_SysTickInit( void ) {
	ISR_INSTALL_ISR2("OsTick", OsTick, SysTick_IRQn, 6, 0);
}
void Os_SysTickStart(uint32_t period_ticks) {
}
