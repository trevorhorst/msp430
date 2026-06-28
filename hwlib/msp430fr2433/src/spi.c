#include <msp430.h>

#include "hwlib/spi.h"

static void _write(const uint8_t data)
{
    // Wait until the transmit buffer is ready for a new byte
    while (!(UCB0IFG & UCTXIFG));

    // Write data to the transmit buffer (this automatically kicks off transmission)
    UCB0TXBUF = data;
}

static uint8_t _read()
{
    // Wait until the receive buffer has data (data received)
    while (!(UCB0IFG & UCRXIFG));
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
    // Put eUSCI_B0 into software reset state
    UCB0CTLW0 |= UCSWRST;

    // Configure SPI Control Register 0 & 1
    // UCCKPH     : Data is changed on the first UCLK edge and captured on the following edge
    // UCMSB      : MSB first
    // UCMST      : Master mode
    // UCSYNC     : Synchronous mode (SPI)
    // UCSSEL__SMCLK : Use SMCLK as the clock source
    UCB0CTLW0 = UCSWRST | UCMST | UCSYNC | UCSSEL__SMCLK;

    // Configure Bit Rate (Clock Divider)
    // If SMCLK is running at 1 MHz, dividing by 1 yields a 1 MHz SPI clock
    UCB0BRW = 1;

    // Configure GPIO Port Mapping for eUSCI_B0
    // For P1.1 (CLK), P1.2 (MOSI), P1.3 (MISO), the secondary module function is SPI.
    // Consult the device datasheet "Input/Output Diagrams" to verify PSEL bits.
    P1SEL0 |= (BIT1 | BIT2 | BIT3);
    P1SEL1 &= ~(BIT1 | BIT2 | BIT3);

    // 5. Release from software reset to enable the SPI peripheral
    UCB0CTLW0 &= ~UCSWRST;

    return 0;
}

int32_t hw_spi_transfer(const uint8_t *buffer, uint32_t len)
{
    int32_t error = 0;
    while((error == 0) && (len > 0)) {
        _transfer(*buffer);
        buffer++;
        len--;
    }
    return error;
}

int32_t hw_spi_write(const uint8_t *buffer, uint32_t len)
{
    int32_t error = 0;
    while((error == 0) && (len > 0)) {
        _transfer(*buffer);
        buffer++;
        len--;
    }
    return error;
}

int32_t hw_spi_write_byte(const uint8_t byte)
{
    hw_spi_write(&byte, 1);
}
