#ifndef CORE_UART_H
#define CORE_UART_H

#include <stdint.h>

typedef enum {
    UART_BAUD_9600   = 0,
    UART_BAUD_19200  = 1,
    UART_BAUD_38400  = 2,
    UART_BAUD_57600  = 3,
    UART_BAUD_115200 = 4
} uart_baud;

int32_t uart_initialize(uart_baud baud);
int32_t uart_write(const uint8_t *data, uint32_t n);

#endif // CORE_UART_H
