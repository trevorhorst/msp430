#ifndef HWLIB_SPI_H
#define HWLIB_SPI_H

#include <msp430g2553.h>
#include <stdint.h>

int32_t hw_spi_initialize(void);
int32_t hw_spi_transfer(const unsigned char *buffer, uint32_t len);
int32_t hw_spi_write(const unsigned char *buffer, uint32_t len);

#endif // HWLIB_SPI_H
