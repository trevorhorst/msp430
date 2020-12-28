#ifndef UART_H
#define UART_H

#include <msp430g2553.h>
#include <stdint.h>

typedef struct
{
    uint32_t baud;
} uart_config_t;

/**
 * @brief Initialize the MSP430 UART interface
 * @param config Container holding initialization information
 */
void uart_initialize( uart_config_t *config );

#endif // UART_H
