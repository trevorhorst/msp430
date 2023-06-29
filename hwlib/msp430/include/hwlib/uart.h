#ifndef HWLIB_UART_H
#define HWLIB_UART_H

#include <msp430g2553.h>
#include <stdint.h>

/**
 * @brief Initializes hardware eo enable USCI_A0 as UART
 * @param baud_id ID corresponding to baud rate table
 * @return int32_t error code
 */
int32_t hw_uart_initialize(uint32_t baud_id);

/**
 * @brief Write data to the UART interface
 * @param data Data to be written
 * @param n Number of bytes to be written
 * @return
 */
int32_t hw_uart_write(const uint8_t *data, uint32_t n);

#endif // HWLIB_UART_H
