// delay.c
#include <stdint.h>
#include "delay.h"
#include "../inc/tm4c123gh6pm.h"

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;         		// disable SysTick during setup
	NVIC_ST_RELOAD_R = 80000 - 1;   // Reload 1ms delay for 80MHz clock
  NVIC_ST_CURRENT_R = 0;      		// any write to current clears it
  NVIC_ST_CTRL_R = 0x0007; 				// enable SysTick with core clock and interrupts
}
void SysTick_Wait(uint32_t delay){
	while(delay > 0){
		while((NVIC_ST_CTRL_R & 0x010000) == 0);
		delay -= 1;
	}
}