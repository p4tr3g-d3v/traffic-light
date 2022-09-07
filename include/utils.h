#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f4/nvic.h>

#define I2C_IN_BUFFER_SIZE 2
#define I2C_IN_BUFFER_COUNT 2

#define I2C_ADDRESS 0x15 //! change

static uint8_t in_buffer[I2C_IN_BUFFER_COUNT][I2C_IN_BUFFER_SIZE+1] = {0};
static volatile uint8_t in_buffer_state[I2C_IN_BUFFER_COUNT] = {0};

void traffic_blink(uint32_t gpio_channel, uint16_t gpios);
void traffic_delay(int d);

void clock_setup(void);
void gpio_setup(void);
void i2c_setup(void);
void usart_setup(void);

void i2c1_ev_isr(void);
bool get_buffer_ready(uint8_t idx);
void clear_buffer_ready(uint8_t idx);
uint8_t* get_buffer(uint8_t idx);

int _write(int fd, char *ptr, int len);

#endif