#ifndef __DELAY_H__
#define __DELAY_H__
#include <stdint.h>

void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);

#endif