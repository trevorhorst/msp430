#include "core/uart.h"
#include "hwlib/uart.h"

int32_t uart_initialize(uart_baud baud)
{
    return hw_uart_initialize(baud);
}

int32_t uart_write(const uint8_t *data, uint32_t n)
{
    return hw_uart_write(data, n);
}
