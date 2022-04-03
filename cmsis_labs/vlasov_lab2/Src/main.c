#include <stdint.h>
#include <stm32f446xx.h>

//define main clock value
#define HCLK 140000000
//define System Tick Timer frequency
#define SysTicksClk 15000
//define System Tick Timer prescaler
#define SysTicks HCLK/SysTicksClk

uint32_t counter=0;
uint8_t RED_LED_PIN = 9;
uint8_t var=0;

void SysTick_Handler(void)
{
	counter++;
	if(counter>=SysTicksClk){
		counter=0;
		if (var == 1){
			var = 0;
			GPIOC->ODR |= (1<<RED_LED_PIN);
		} else {
			var = 1;
			GPIOC->ODR &= ~(1<<RED_LED_PIN);
		}
	}

}
int main()
{
	/*1. Set flash latency*/
	FLASH->ACR=FLASH_ACR_LATENCY_4WS;

	/*2. Enable hse and wait until it's ready*/
	RCC->CR |= (1 << 16); // 16s bit
	while( !(RCC->CR & (1 << 17)) ){} // 17s bit

	/*3. Set AHB, APB1 & APB2 prescaler*/
	//HPRE
	//RCC->CFGR |= (1 << 4); //4 bit
	RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk); // AHB_presc = 1
	//PPRE1
	RCC->CFGR |= (0b101 << 10); //presc = 4
	//PPRE2
	RCC->CFGR |= (0b100 << 13); //presc = 2

	/*4. Config PLL*/
	// Set PLL source (HSE)
	RCC->PLLCFGR |= (1 << 22);
	// Set PLLM
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk);
	RCC->PLLCFGR |= (20 << 0);
	// Set PLLN
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_Msk);
	RCC->PLLCFGR |= (280 << 6);
	// Set PLLP
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP_Msk);
	RCC->PLLCFGR |= (0 << 16); // presc = 2
	//Enable PLL and wait until it starts
	RCC->CR |= (1 << 24);
	while( !(RCC->CR & (1 << 25)) ){}

	/*5. Set main clock switch to PLL source.
	 * Check, that the switch moves to the correct source.*/
	RCC->CFGR |= (2 << 0);
	while( !(RCC->CFGR & (2 << 2) )){}


	// GPIOC_EN
	RCC->AHB1ENR |= (1 << 2);
	GPIOC->MODER |= (1 << RED_LED_PIN*2);
	GPIOC->ODR &= ~(1 << RED_LED_PIN);

	SysTick_Config(SysTicks); //SysTick Enable and configuration

	while(1){

	}

}
