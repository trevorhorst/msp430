#ifndef CORE_SPI_H

#include <stdint.h>

#define SPI_WRITE_TYPE_DATA      0
#define SPI_WRITE_TYPE_COMMAND   1

int32_t spi_initialize();
int32_t spi_write(const unsigned char *buffer, uint32_t len);

#endif // CORE_SPI_H
