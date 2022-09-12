#include <errno.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/common.h>

#include "common.h"

static void gpio_setup(void)
{
	// Onboard led for debugging
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	gpio_set(GPIOC, GPIO13);


	// GPIO pins for reading external value (B12-B15)
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO12|GPIO13|GPIO14|GPIO15);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO8);
	gpio_clear(GPIOA, GPIO8);
}

// const struct rcc_clock_scale rcc =
// 	{ /* 100MHz */
// 		.pllm = 25,
// 		.plln = 192,
// 		.pllp = 2,
// 		.pllq = 4,
// 		.pllr = 0,
// 		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
// 		.hpre = RCC_CFGR_HPRE_DIV_NONE,
// 		.ppre1 = RCC_CFGR_PPRE_DIV_2,
// 		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
// 		.voltage_scale = PWR_SCALE1,
// 		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
// 		.ahb_frequency  = 96000000,
// 		.apb1_frequency = 48000000,
// 		.apb2_frequency = 96000000
// 	};

static inline uint8_t read_gpio_value() {
	return ~(gpio_get(GPIOB, GPIO12|GPIO13|GPIO14|GPIO15)>>12)&0x0F;
}

int main(void)
{
	uint32_t i, k=0;
	uint8_t state = 0;

	rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

	debug_setup();
	gpio_setup();

	// Read i2c slave address from GPIO value (GND shorted pins B12-B15)
	i2c_slave_setup(I2C_ADDRESS_BASE + read_gpio_value() + 1);

	traffic_setup();

	printf("Ready!!!\n");

	while (1) {
		if (get_buffer_ready(0)) {
			state = get_buffer(0)[0];
			clear_buffer_ready(0);
			if (state < 3) {
				current_state = state;
			}
		}
		if (get_buffer_ready(1)) {
			state = get_buffer(1)[0];
			clear_buffer_ready(1);
			current_manual_state = state;
		}

		//gpio_toggle(GPIOC, GPIO13);
		//printf("TEST MESSAGE %d\n", read_gpio_value());
		for (i = 0; i < 1000000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
