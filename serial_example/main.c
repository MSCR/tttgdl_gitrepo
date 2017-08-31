#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void startUp(void);
void initSerial(void);
void print(byte string);

void startUp(void){
	SOPT1 = 0x02;       //Disable Watchdog
	initSerial();     	//Init SCI
	EnableInterrupts; 	/* enable interrupts */
}

void initSerial(void){
	SCI1BDH = 0x00;     
	SCI1BDL = 0x1A;     //Baud Rate de 9600
	SCI1C1 = 0x00;      //No Loop Mode, no stop SCI en wait mode, 9 bits, Paridad habilitada, Paridad impar
	SCI1C2 = 0x0C;      //Interrupcion en transmision completa, interrupcion para receiver,Transmitter y
						//Receiver habilitado
}

void print(byte dato)    // Se ejecuta la transmisión de dato por encuesta
{   
	byte bandera;
	while (SCI1S1_TDRE == 0);	// No finaliza ciclo hasta que esté libre el registro
								// en el que se guardará el dato que se enviará.
		bandera= SCI1S1;		// Aclara la bandera.
	SCI1D = dato;         		// Enviando dato. 
}


void main(void) {
	startUp();
	print('o'); 

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
