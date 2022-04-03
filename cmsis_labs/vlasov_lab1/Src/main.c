#include <stdint.h>

#define PERIPH_BASE 0x40000000
#define PERIPH_BB_BASE 0x42000000
#define AHB1_BASE (PERIPH_BASE + 0x020000)
#define GPIOA_BASE (AHB1_BASE + 0x0000)
#define GPIOB_BASE (AHB1_BASE + 0x0400)
#define GPIOC_BASE (AHB1_BASE + 0x0800)

typedef struct
{
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDER;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;

} GPIO_TypeDef;

#define  GPIOC ((GPIO_TypeDef *) GPIOC_BASE)
#define PC9 *((uint32_t *)(PERIPH_BB_BASE + ((uint32_t)&(GPIOC->ODR) - PERIPH_BASE)*32 + 9*4))

int main(void)
{
	*((uint32_t *) 0x40023830) |= 1 | 4;

	uint32_t BLUE_LED_PIN = 0x8;
	uint8_t RED_LED_PIN = 9;
	uint32_t BLUE_BUTTON_PIN = 0x6;
	uint32_t RED_BUTTON_PIN = 0x5;

	GPIOC->MODER |= (1<<BLUE_LED_PIN*2);
	GPIOC->MODER |= (1<<RED_LED_PIN*2);

	GPIOC->MODER &= ~(1<<BLUE_BUTTON_PIN*2);
	GPIOC->PUPDR |= (0<<BLUE_BUTTON_PIN*2);
	GPIOC->MODER &= ~(1<<RED_BUTTON_PIN*2);
	GPIOC->PUPDR |= (0<<RED_BUTTON_PIN*2);

	GPIOC->ODR |= (1<<BLUE_LED_PIN);
	PC9 = 0;

	uint8_t blue_button_state = 1;
	uint8_t red_button_state = 1;

	uint8_t variable = 0b00;

	while(1){
		blue_button_state = GPIOC->IDR & (1<<BLUE_BUTTON_PIN);
		red_button_state = GPIOC->IDR & (1<<RED_BUTTON_PIN);

		if (!blue_button_state){
			if (variable > 0){
				variable -= 1;
				uint32_t count = 0;
				while (count<200000) {
					count += 1;
				}
			}
		}

		if (!red_button_state){
			if (variable < 3){
				variable += 1;
				uint32_t count = 0;
				while (count<200000) {
					count += 1;
				}
			}
		}

		if (variable & 0b10){
			PC9 = 1;
		} else {
			PC9 = 0;
		}

		if (variable & 0b01){
			GPIOC->ODR |= (1<<BLUE_LED_PIN);
		} else {
			GPIOC->ODR &= ~(1<<BLUE_LED_PIN);
		}
	}
}
