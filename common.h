#ifndef __COMMON_H
#define __COMMON_H

#include <stdint.h>

#define I2C_ADDRESS_BASE 0x20

extern uint8_t current_state;
extern uint8_t current_manual_state;

void traffic_setup();
void i2c_slave_setup(uint8_t addr);
void debug_setup(void);

bool get_buffer_ready(uint8_t idx);
void clear_buffer_ready(uint8_t idx);
uint8_t* get_buffer(uint8_t idx);

#endif