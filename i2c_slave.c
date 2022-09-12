
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/i2c.h>

#include "common.h"

#define I2C_IN_BUFFER_SIZE 2
#define I2C_IN_BUFFER_COUNT 2

static uint8_t in_buffer[I2C_IN_BUFFER_COUNT][I2C_IN_BUFFER_SIZE+1] = {0};
static volatile uint8_t in_buffer_state[I2C_IN_BUFFER_COUNT] = {0};

void i2c_slave_setup(uint8_t addr)
{
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_GPIOB);
	
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
	gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO6 | GPIO7);

	i2c_peripheral_disable(I2C1);
	i2c_reset(I2C1);
	i2c_set_speed(I2C1, i2c_speed_sm_100k, 8);
	i2c_set_own_7bit_slave_address(I2C1, addr);
	// i2c_set_own_7bit_slave_address_two(I2C1, addr+0x10);
	nvic_enable_irq(NVIC_I2C1_EV_IRQ);
	i2c_enable_interrupt(I2C1, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
	// i2c_enable_dual_addressing_mode(I2C1);
	i2c_peripheral_enable(I2C1);

	// Slave
	i2c_enable_ack(I2C1);
}

void i2c1_ev_isr(void)
{
	uint32_t sr1, sr2;
	static uint8_t idx = 0;
	static uint8_t* rb = 0;

	sr1 = I2C_SR1(I2C1);

	if (sr1 & I2C_SR1_ADDR) {
        sr2 = I2C_SR2(I2C1);
        if (!(sr2 & I2C_SR2_TRA)) {
			// Reset refs
			rb = 0;
			idx = 0;
		}
	}
	if (sr1 & I2C_SR1_RxNE) {
        uint8_t b = i2c_get_data(I2C1);
		
		// Skip overflow
		if (idx && rb && ((rb-in_buffer[idx-1]) >= I2C_IN_BUFFER_SIZE)) return;

		// first: buffer index received
		if (!idx && !rb && b > 0 && b<=I2C_IN_BUFFER_COUNT) {
			
			idx = b;
			if (!in_buffer_state[idx-1])
				rb = in_buffer[idx-1];

		// other: apped to buffer
		} else if (rb) {
	    	*rb++=b;
		}
	}
	if (sr1 & I2C_SR1_TxE) {
		// Always send only one first byte
		i2c_send_data(I2C1, (rb) ? *rb : 0xff);
		I2C_SR1(I2C1) &= ~(I2C_SR1_TxE);
	}
	if (sr1 & I2C_SR1_STOPF) {
		i2c_peripheral_enable(I2C1);
		*rb= 0; // Null terminator (for strings)
		in_buffer_state[idx-1] = 1;
	}
	if (sr1 & I2C_SR1_AF) { // Acknowledge failure // TODO: do something)
        I2C_SR1(I2C1) &= ~(I2C_SR1_AF);
	}
}

bool get_buffer_ready(uint8_t idx) {
	return in_buffer_state[idx];
}

void clear_buffer_ready(uint8_t idx) {
	in_buffer_state[idx] = 0;
}

uint8_t* get_buffer(uint8_t idx) {
	return in_buffer[idx];
}