#include <stdio.h>
#include <stdint.h>
#include <stm32f446xx.h>
#include <math.h>

//define main clock value
#define HCLK 140000000
//define System Tick Timer frequency
#define SysTicksClk 15000
//define System Tick Timer prescaler
#define SysTicks HCLK/SysTicksClk
uint32_t counter=0;
uint8_t RED_LED_PIN = 9;
uint8_t var=0;
uint8_t state = 0;

#define APB1_FREQ 35000000
#define BAUDRATE 115200

void ClocksInit(){
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
	RCC->PLLCFGR |= (18 << 0);
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


	// GPIOC_EN and GPIOA_EN
	RCC->AHB1ENR |= (1 << 2) | (1 << 0);
	GPIOC->MODER |= (1 << RED_LED_PIN*2);
	GPIOC->ODR &= ~(1 << RED_LED_PIN);

	SysTick_Config(SysTicks); //SysTick Enable and configuration
}

void USART2_IRQHandler(void){
	//4.	Create a function - USART interrupts handler.

	uint8_t read_char = USART2->DR;
	//const char c = 49;
	//const char b = 50;
/*
	if (read_char == 'E'){
		USART2->DR = c;
		state = 1;
	} else if (read_char == 'N'){
		USART2->DR = b;
	}*/

	switch (read_char) {
	case 'N':
		if (state == 0){
			state = 1;
			//USART2->DR = read_char;
		}
		break;
	case 'x':
		if (state == 1){
			state = 2 ;
			//USART2->DR = read_char;
		}
		break;
	case '=':
		if (state == 2){
			state = 3 ;
			//USART2->DR = read_char;
		}
		break;
	case 'E':
		if (state == 5){
			state = 0;

			//char output[] = "\nf(x)=12.345\r";
			//sprintf(output, "%f", 1.1);

			char output[12];
			//float num = exp(var);
			float num = 1 + var + pow(var, 2)/2.0f + pow(var, 3)/6.0f + pow(var, 4)/24.0f + pow(var, 5)/120.0f + pow(var, 6)/720.0f
					+ pow(var, 7)/5040.0f;

			sprintf(output, "\nf(x)=%.3f\r", num);

			for (int i = 0; i < sizeof(output)-1; i++){
				USART2->DR = output[i];
				while (!(USART2->SR & (1<<6)));
			}
		}
		break;

	default:
		if (state == 3){
			read_char = read_char - '0';
			var = (uint8_t)read_char * 10;
			state = 4;
		} else if (state == 4){
			read_char = read_char - '0';
			var = var + (uint8_t)read_char;
			state = 5;
		}
		break;
	}
}

int main(void)
{

	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));

	// 0. Set Clocks
	ClocksInit();

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	GPIOA->MODER |= 2 << GPIO_MODER_MODER2_Pos;
	GPIOA->MODER |= 2 << GPIO_MODER_MODER3_Pos;

	GPIOA->AFR[0] |= 7 << GPIO_AFRL_AFSEL2_Pos;
	GPIOA->AFR[0] |= 7 << GPIO_AFRL_AFSEL3_Pos;

	// 1. Set USART2 baudrate
	USART2->BRR = APB1_FREQ / BAUDRATE;
	/* 2. Configure USART to receive and
	* transmit data, generate the required
	* interrupt. */
	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE/* | USART_CR1_TXEIE*/;
	// 3. Enable interrupts in the interrupt controller.
	NVIC_EnableIRQ(USART2_IRQn);

	while(1){

	}
}
