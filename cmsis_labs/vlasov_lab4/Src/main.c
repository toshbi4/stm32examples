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

uint8_t tx_data[10];
uint8_t rx_data[10];

#define APB1_FREQ 35000000
#define APB1_TIM_FREQ (35000000 * 2)
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
	RCC->PLLCFGR |= (4 << 0);
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

void TIM2_IRQHandler(void){

	TIM2->SR &= ~(TIM_SR_UIF);
	// Enable transmit
	DMA1_Stream6->NDTR = 10;
	DMA1->HIFCR |= (0x1UL << DMA_HIFCR_CTCIF6_Pos) | DMA_HIFCR_CHTIF6;
	DMA1_Stream6->CR |= DMA_SxCR_EN;
	//USART2->DR = 'c';

}

void DMA1_Stream5_IRQHandler(void){

	tx_data[0] = rx_data[0];
	DMA1_Stream5->NDTR = 10;
	DMA1->HIFCR |= (0x1UL << DMA_HIFCR_CTCIF5_Pos) | DMA_HIFCR_CHTIF5;
	DMA1_Stream5->CR |= DMA_SxCR_EN;
}

int main(void)
{
	// float operations
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
	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE /*| USART_CR1_TXEIE*/;

	//Lab4
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 35000 - 1;
	//TIM2->CNT = 2000;
	TIM2->ARR = 2000;
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 &= ~(TIM_CR1_DIR); //up-counter mode
	TIM2->CR1 |= TIM_CR1_ARPE; // Enable auto-preload buffer
	TIM2->DIER |= TIM_DIER_UIE; //Enable update interrupt

	USART2->CR3 |= USART_CR3_DMAT;
	USART2->CR3 |= USART_CR3_DMAR;

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// Mem to periph, mem address is incremented,
	DMA1_Stream6->CR |= DMA_SxCR_DIR_0 | DMA_SxCR_MINC | DMA_SxCR_CHSEL_2;
	DMA1_Stream6->PAR = (uint32_t) &USART2->DR;
	DMA1_Stream6->M0AR = (uint32_t) &tx_data;
	tx_data[0] = 1;

	DMA1->HIFCR |= (0x1UL << DMA_HIFCR_CTCIF6_Pos) | DMA_HIFCR_CHTIF6;
	DMA1_Stream6->NDTR = 10;
	DMA1_Stream6->CR |= DMA_SxCR_EN;


	// Mem to periph, mem address is incremented,
	DMA1_Stream5->CR |= DMA_SxCR_MINC | DMA_SxCR_CHSEL_2 | DMA_SxCR_TCIE;
	DMA1_Stream5->PAR = (uint32_t) &USART2->DR;
	DMA1_Stream5->M0AR = (uint32_t) &rx_data;

	DMA1->HIFCR |= (0x1UL << DMA_HIFCR_CTCIF5_Pos) | DMA_HIFCR_CHTIF5;
	DMA1_Stream5->NDTR = 10;
	DMA1_Stream5->CR |= DMA_SxCR_EN;

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	//NVIC_EnableIRQ(USART2_IRQn);

	while(1){

	}
}
