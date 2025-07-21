#include "core/spi.h"
#include "hwlib/spi.h"


int32_t spi_initialize()
{
    return hw_spi_initialize();
}

int32_t spi_write(const unsigned char *buffer, uint32_t len)
{
    return hw_spi_write(buffer, len);
}
