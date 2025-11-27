#include <p18cxxx.h>
#include "interrupt.h"

//PROTOTYPES
void HighPriorityInterrupt();
void LowPriorityInterrupt();

extern void MainApp(void);


/////////////////////////////////////////////////////////////////////
/// VECTOR REMAPPING ////////////////////////////////////////////////

#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void High_ISR (void)
{
     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
}
#pragma code LOW_INTERRUPT_VECTOR = 0x18
void Low_ISR (void)
{
     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
}


#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
void resetRemmaped (void)
{
	_asm goto MainApp _endasm
}

#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
void Remapped_High_ISR (void)
{
     _asm goto HighPriorityInterrupt _endasm
}

#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
void Remapped_Low_ISR (void)
{
     _asm goto LowPriorityInterrupt _endasm
}

//These are your actual interrupt handling routines.
#pragma interrupt HighPriorityInterrupt
void HighPriorityInterrupt()
{
	//Check which interrupt flag caused the interrupt.
	//Service the interrupt
	//Clear the interrupt flag
	//Etc.
}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
#pragma interruptlow LowPriorityInterrupt
void LowPriorityInterrupt()
{
	//Check which interrupt flag caused the interrupt.
	//Service the interrupt
	//Clear the interrupt flag
	//Etc.

}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

/// END VECTOR REMAPPING ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
