

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include "common.h"

#define LED_RCC 		RCC_GPIOA
#define LED_GPIO 		GPIOA
#define LED_DIR1_RED	GPIO0
#define LED_DIR1_YELLOW GPIO1
#define LED_DIR1_GREEN 	GPIO2
#define LED_DIR2_RED 	GPIO3
#define LED_DIR2_YELLOW GPIO4
#define LED_DIR2_GREEN 	GPIO5

#define LED_DIR1_BASE 	GPIO0
#define LED_DIR2_BASE 	GPIO3

#define LED_ALL LED_DIR1_RED | LED_DIR1_YELLOW | LED_DIR1_GREEN | \
	LED_DIR2_RED | LED_DIR2_YELLOW | LED_DIR2_GREEN

// Auto mode timer values
// Time in ticks
#define STEP1_TIME 14	// Normal
#define STEP2_TIME 6	// Going to switch (blink)
#define STEP3_TIME 3	// Warning (yellow)
#define STEP4_TIME 2	// Switch

uint8_t current_state = 0;
uint8_t current_manual_state = 0;

void traffic_setup() {
	rcc_periph_clock_enable(LED_RCC);
	gpio_mode_setup(LED_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_ALL);
	gpio_set(LED_GPIO, LED_ALL);


	rcc_periph_clock_enable(RCC_TIM2);
	nvic_enable_irq(NVIC_TIM2_IRQ);
	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
		TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 5000));

	timer_disable_preload(TIM2);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 86000000 / 30000);
	timer_enable_counter(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

static void tick() {
	static uint8_t state, mstate = -1;
	static uint8_t count = 0;
	static uint8_t inverted = 0;

	// State was changed
	if ((current_state != state) || ((state==2) && (current_manual_state != mstate))) {
		gpio_set(LED_GPIO, LED_ALL); // Turn off (reset) all lights

		count = 0;

		state = current_state;
		mstate = current_manual_state;

		if (state == 2) {
			uint8_t d1 = mstate & 0x0F;
			uint8_t d2 = mstate >> 4;
			if (d1>=1 && d1 <= 3) {
				gpio_clear(LED_GPIO, LED_DIR1_BASE << (d1-1));
			}
			if (d2>=1 && d2 <= 3) {
				gpio_clear(LED_GPIO, LED_DIR2_BASE << (d2-1));
			}
		}
	}

	switch (state) {
		case 0:
			if (count < STEP1_TIME) {
				if (count == 0) gpio_set(LED_GPIO, LED_ALL);
				gpio_clear(LED_GPIO, (inverted)?LED_DIR1_RED:LED_DIR1_GREEN);
				gpio_clear(LED_GPIO, (!inverted)?LED_DIR2_RED:LED_DIR2_GREEN);
			} else if (count < (STEP1_TIME + STEP2_TIME)) {
				gpio_toggle(LED_GPIO, (inverted)?LED_DIR1_RED:LED_DIR1_GREEN);
				gpio_toggle(LED_GPIO, (!inverted)?LED_DIR2_RED:LED_DIR2_GREEN);
			} else if (count < (STEP1_TIME + STEP2_TIME + STEP3_TIME)) {
				gpio_set(LED_GPIO, LED_ALL);
				gpio_clear(LED_GPIO, LED_DIR1_YELLOW);
				gpio_clear(LED_GPIO, LED_DIR2_YELLOW);
				// gpio_clear(LED_GPIO, (inverted)?LED_DIR1_RED:LED_DIR2_RED);

			} else if (count < (STEP1_TIME + STEP2_TIME + STEP3_TIME + STEP4_TIME)) {
				// Switch directions and restart
				count = 0xff;
				inverted = !inverted;
			}
			count++;
		break;
		case 1:
			gpio_toggle(LED_GPIO, LED_DIR1_YELLOW);
			gpio_toggle(LED_GPIO, LED_DIR2_YELLOW);
			break;
	}
}

void tim2_isr() {
	if (timer_get_flag(TIM2, TIM_SR_UIF)) {
		timer_clear_flag(TIM2, TIM_SR_UIF);

		/* Toggle LED to indicate event. */
		gpio_toggle(GPIOC, GPIO13);
		tick();
	}
}