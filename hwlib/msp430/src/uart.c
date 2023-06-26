#include "hwlib/uart.h"

int32_t hw_uart_initialize()
{
    int32_t error = 0;
    UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
    UCA0BR0   =  104;                 // 104 From datasheet table-
    UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
    UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
    UCA0STAT |=  UCLISTEN;            // loop back mode enabled
    UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0
    return error;
}
