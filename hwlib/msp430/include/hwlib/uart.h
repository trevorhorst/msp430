#ifndef HWLIB_UART_H
#define HWLIB_UART_H

#include <msp430g2553.h>
#include <stdint.h>

int32_t hw_uart_initialize(uint32_t baud_id);
int32_t hw_uart_write(const uint8_t *data, uint32_t n);

#endif // HWLIB_UART_H
