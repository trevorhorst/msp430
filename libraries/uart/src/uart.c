#include "uart/uart.h"

void uart_initialize( uart_config_t *config )
{
    // USCI should be in reset before configuring
    if( UCA0CTL1 & UCSWRST ) {
    }
}
