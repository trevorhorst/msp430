#include "hwlib/uart.h"

typedef struct hw_uart_baud_entry_t {
    uint32_t baud;
    uint8_t br0;
    uint8_t br1;
    uint8_t mod;
} hw_uart_baud_entry;

// Calculated from here https://mspgcc.sourceforge.net/baudrate.html
static const hw_uart_baud_entry hw_uart_baud_table[] = {
    {  9600,   (1e6/9600), 0, 0x04},
    { 19200,  (1e6/19200), 0, 0x40},
    { 38400,  (1e6/38400), 0, 0x00},
    { 57600,  (1e6/57600), 0, 0x52},
    {115200, (1e6/115200), 0, 0x60} // This doesn't seem to work
};

int32_t hw_uart_initialize(uint32_t baud_id)
{
    int32_t error = 0;
    // Select the SMCLK as the USCI_A0 clock and place the USCI_A0 in reset
    UCA0CTL1 |=  UCSSEL_2 + UCSWRST;
    // Selects baud rate from pre determined table, including modulation
    UCA0BR0   =  hw_uart_baud_table[baud_id].br0;
    UCA0BR1   =  hw_uart_baud_table[baud_id].br1;
    UCA0MCTL  =  hw_uart_baud_table[baud_id].mod;
    // UCA0STAT |=  UCLISTEN;            // loop back mode enabled
    UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0
    return error;
}

int32_t hw_uart_write(const uint8_t *data, uint32_t n)
{
    int32_t r = 0;

    if(data) {
        while(n > 0) {
            // Wait for the transmit buffer to be ready
            while(!(IFG2 & UCA0TXIFG));
            // Transmit a character
            UCA0TXBUF = *data;
            // Advance data pointer
            data++;
            n--;
        }
    }

    return r;
}
