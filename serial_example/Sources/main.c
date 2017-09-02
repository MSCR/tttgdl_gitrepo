#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

//#define SCI_interrupt

enum IO_definitions {INPUT, OUTPUT};
enum enable_definitions {DISABLE, ENABLE};
enum SCI_buffer {FULL, EMPTY};

void startUp(void);
void initSerial(void);
void print(char *);

void startUp(void){
	SOPT1 = 2;       //Disable Watchdog
	initSerial();     	//Init SCI
	EnableInterrupts; 	/* enable interrupts */
}

void initSerial(void){
	SCI1BD = 26;		// Set Baud Rate value to 4M/(16xSCI1BD) = 9600
	SCI1C2_TE = ENABLE;      // Enable transmitter
	//SCI1C2_RE = ENABLE;		// Enable Receiver
#ifdef SCI_interrupt
	SCI1C2_TIE = ENABLE;	// Enable transmitter interrupt when transmitter is empty
	//SCI1C2_TCIE = ENABLE;	// Enable transmitter interrupt when transfer is completed
	SCI1C2_RIE = ENABLE;	// Enable receiver interrupt when buffer is full
#endif
}

void print(char *dato)    // Send data thru Serial port
{   
	byte flag;
	do{
		while (SCI1S1_TDRE == FULL);	// Wait for transfer completion 
		flag = SCI1S1;					// Clear flag when read.
		SCI1D = *dato++;				// Send data. 		
	} while( *dato );
	SCI1D = '\r';						// Send data.
}


void main(void) {
	startUp();
	print("Hello World\n"); 

	for(;;) {
		__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */

}

interrupt VectorNumber_Vsci1tx
void Transmitter_SCI(void){
	SCI1C2 = 0x08;
	SCI1S1 = 0x00;         //Reiniciar las banderas despues de la transmision
}

interrupt VectorNumber_Vsci1rx 
void Receiver_SCI(void){

}

interrupt VectorNumber_Vsci1err 
void Error_SCI(void){
	
}
