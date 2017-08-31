#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

word timeout1;

void startUp(void){
	SOPT1 = 0x02;       //Disable Watchdog
	EnableInterrupts; /* enable interrupts */
}

void initPorts(){
	PTBDD = 0xFF;         // Set DEMOQE led ports as outputs
	PTCDD = 0x3F;
	PTEDD = 0xC0;
	   
	PTBD = 0x20;
	PTCD = 0x3F;
	PTED = 0xC0;  	
	
}

void initTimers(){
	TPM1SC = 0x06;        // Timer 1 - Cleared + Stopped.
						// Clicks once every 64 BUS Cycles
							  // Bus Clock Selected
	
	TPM1C0VH = 0x02;      // Set Output Compare to happen 622T clicks ($26e)
	TPM1C0VL = 0x6e;      // after we start the timer. (~10ms). The
						// timer interrupt will set OC for another ~10ms.
	
	TPM1C0SC = 0x54;      // Timer 1 Channel 0
						// Set for Output Compare operation.
	
	TPM1SC = 0x0E;        // Start the timer    
}

void main(void) {
	startUp();
	initTimers();
	initPorts();
	
	for(;;) {
		__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */

}

interrupt VectorNumber_Vtpm1ch0 
void TPM_ISR(void){
	TPM1C0SC = TPM1C0SC&0x7F;
	TPM1C0VL = TPM1C0VL + 0x6E;
	TPM1C0VH = TPM1C0VH + 0x02;
	if (timeout1!=0) {
		timeout1--;
	} else {
		PTBD = PTBD^0x20;
		PTCD = PTCD^0x3F;
		PTED = PTED^0xC0;      // Setup next t1_handler event, ~150ms 
		timeout1 = 0x0F;
	   
	}
}
