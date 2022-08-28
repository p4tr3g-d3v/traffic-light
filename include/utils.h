#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f4/nvic.h>

void traffic_blink(uint32_t gpio_channel, uint16_t gpios);
void traffic_delay(int d);

void clock_setup(void);
void gpio_setup(void);
void i2c_setup(void);
void usart_setup(void);

int _write(int fd, char *ptr, int len);

#endif