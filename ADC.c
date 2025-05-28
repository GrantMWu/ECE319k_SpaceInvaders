// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/2/2023  
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// This file is in the inc folder so it automatically will be applied to labs 8 9 and 10
// Labs 8, 9, and 10 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function using PD2 
// Input: none
// Output: none
void ADC_Init(void){ 
	// initialize PD2 for analog input
	SYSCTL_RCGCGPIO_R |= 0x08; 		// activate clock for portD
	while((SYSCTL_PRGPIO_R&0x08) != 0x08) {		// wait for stabilization
	}; 					
	GPIO_PORTD_DIR_R &= ~0x04;		// PD2 input
	GPIO_PORTD_AFSEL_R |= 0x04;		// enable alternate function on PD2
	GPIO_PORTD_DEN_R &= ~0x04;		// disable digital I/O on PD2
	GPIO_PORTD_AMSEL_R |= 0x04;		// enable analog functionality on PD2
	
	// initialize ADC0
	SYSCTL_RCGCADC_R |= 0x01; 	// activate ADC0
	while((SYSCTL_PRADC_R&0x01) != 0x01){			// wait for stabilization
	}

	
	ADC0_PC_R &= ~0x0F;					// clear ADC0 speed
	ADC0_PC_R = 0x01;						// configure for 125K samples/sec
	ADC0_SAC_R = 4;
	
	ADC0_SSPRI_R = 0x0123;			// set seq3 as highest priority
	ADC0_ACTSS_R &= ~0x08;			// Disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;			// seq3 is software trigger
	ADC0_ACTSS_R |= 0x08;				// Enable sample sequencer 3
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5;		//Ain5(PD2)
	ADC0_SSCTL3_R = 0x0006;			// END0: on sample completion, set flag. IE0: only 1 sample
	ADC0_IM_R &= ~0x08;					// Disable SS3 interrupts
	ADC0_ACTSS_R |= 0x08;				// enable sample sequencer3
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	uint32_t data;
	ADC0_PSSI_R = 0x08;										// 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};				// 2) wait for sample flag from SSCTL3
  data = ADC0_SSFIFO3_R&0xFFF; 				// 3) read result
  ADC0_ISC_R = 0x08;										// 4) clear RIS flag bit
  return data;
}


