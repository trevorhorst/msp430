#ifndef HWLIB_SPI_H
#define HWLIB_SPI_H

#include <stdint.h>

int32_t hw_spi_initialize(void);
int32_t hw_spi_transfer(const uint8_t *buffer, uint32_t len);
int32_t hw_spi_write(const uint8_t *buffer, uint32_t len);
int32_t hw_spi_write_byte(const uint8_t byte);

#endif // HWLIB_SPI_H
