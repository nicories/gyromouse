#ifndef LEVEL_I2C_H
#define LEVEL_I2C_H
#include <stdint.h>
#include "STM32F042x.h"
void i2c_init();
int32_t i2c_write8(uint8_t slave_addr, uint8_t data);
int32_t i2c_read8(uint8_t slave_addr);
int32_t i2c_read_reg8(uint8_t slave_addr, uint8_t reg_addr);
int32_t i2c_write_reg8(uint8_t slave_addr, uint8_t reg_addr, uint8_t data);
static volatile I2C1_Type* const i2c = I2C1;
#endif
