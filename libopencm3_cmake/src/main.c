
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>

// PB4 - LED

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  rcc_periph_clock_enable(RCC_GPIOB); // led port

  rcc_periph_clock_enable(RCC_AFIO);
}

static void gpio_setup(void) {
  
  // LED
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, GPIO4);

  AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;

  gpio_set(GPIOB, GPIO4);
  // LED END
}

int main(void) {
  clock_setup();
  gpio_setup();

  gpio_set(GPIOB, GPIO4);

  while(1) {

  }
}
