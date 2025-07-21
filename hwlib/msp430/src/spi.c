#include "hwlib/spi.h"

static void _write(const uint8_t data)
{
    // Wait until the transmit buffer is ready
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;
}

static uint8_t _read()
{
    // Wait until the receive buffer has data (data received)
    while (!(IFG2 & UCA0RXIFG));
    return UCA0RXBUF;
}

static uint8_t _transfer(const uint8_t data)
{
    // UCA0TXBUF = data; // Place data in the transmit buffer
    // // Wait until the transmit buffer is empty (data sent)
    // while (!(IFG2 & UCA0TXIFG));
    // // Wait until the receive buffer has data (data received)
    // while (!(IFG2 & UCA0RXIFG));
    // return UCA0RXBUF; // Read the received data
    _write(data);
    return _read();
}

int32_t hw_spi_initialize(void)
{
    // Put USCI_A0 in reset while configuring
    UCA0CTL1 |= UCSWRST;

    // Configure UCA0CTL0 register
    // UCMSB: Most Significant Bit first
    // UCMST: Master mode
    // UCSYNC: Synchronous mode (SPI)
    // UCCKPL: Clock polarity low (idle low)
    // UCCKPH: Clock phase - data changed on first edge, captured on second (SPI Mode 0)
    UCA0CTL0 |= UCMSB | UCMST | UCSYNC | UCCKPL; // SPI Mode 0

    // Select SMCLK as the clock source
    UCA0CTL1 |= UCSSEL_2;

    // Set clock divider for desired SPI clock frequency
    // SMCLK (1MHz) / 2 = 500kHz (UCA0BR0 = 2)
    UCA0BR0 = 2; // Low byte of clock divider
    UCA0BR1 = 0; // High byte of clock divider

    // Release USCI_A0 from reset
    UCA0CTL1 &= ~UCSWRST;

    return 0;
}

int32_t hw_spi_transfer(const unsigned char *buffer, uint32_t len)
{
    int32_t error = 0;
    while((error == 0) && (len > 0)) {
        _transfer((const uint8_t)(*buffer));
        buffer++;
        len--;
    }
    return error;
}

int32_t hw_spi_write(const unsigned char *buffer, uint32_t len)
{
    int32_t error = 0;
    while((error == 0) && (len > 0)) {
        _transfer((const uint8_t)(*buffer));
        buffer++;
        len--;
    }
    return error;
}
