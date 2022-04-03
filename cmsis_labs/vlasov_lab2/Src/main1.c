#include "stm32f446xx.h"

/*

Variant 5
CPU freq = 140 MHz
System Timer freq = 15 kHz



*/
/*
#define PLLM 8
#define PLLN 280
#define PLLP 2
#define PLLSRC 1 // HSE
//define main clock value
#define HCLK 			140000000
//define System Tick Timer frequency
#define SysTicksClk 	15000
//define System Tick Timer prescaler
#define SysTicks    	 HCLK/SysTicksClk

void SysTick_Handler(void)
{
	//SysTick Interrupt handler
}


int main(void)
{

	SysTick_Config(SysTicks);

	for(;;);
}
*/
