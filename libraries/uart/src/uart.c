#include "uart/uart.h"

void uart_initialize( uart_config_t *config )
{
    // USCI should be in reset before configuring
    if( UCA0CTL1 & UCSWRST ) {
        // Set clock source to SMCLK
        UCA0CTL1 |= UCSSEL_2;

        // Configure the baud rate
        UCA0BR0  = 104;
        UCA0BR1  = 0;
        UCA0MCTL = 0x2;

        // Take the USCI out of reset
        UCA0CTL1 &= ~UCSWRST;

        // Enable RX interrupts
        IE2 |= UCA0RXIE;
    }
}


__attribute__( ( interrupt( USCIAB0RX_VECTOR ) ) ) void rx_isr(void)
{
    if (IFG2 & UCA0RXIFG) {

        // Read character from the uart buffer
        const char c = UCA0RXBUF;

        // Clear the interrupt flag
        IFG2 &= ~UCA0RXIFG;

        // ring_buffer_put(_rbd, &c);
    }
}
