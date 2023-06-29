#ifndef HWLIB_I2C_H
#define HWLIB_I2C_H

#include <msp430g2553.h>
#include <stdint.h>

int32_t hw_i2c_initialize(uint8_t bus);
int32_t hw_i2c_write(uint8_t bus, uint8_t address, const uint8_t *data, uint32_t n);
int32_t hw_i2c_read(uint8_t bus, uint8_t address, uint8_t *data, uint32_t n);

#endif // MSP430_I2C_H
