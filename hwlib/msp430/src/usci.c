#include "hwlib/usci.h"

void hw_usci_initialize(uint32_t mode)
{
    if(mode == USCI_MODE_SPI) {
        // Configure Pin Muxing for USCI_A0 SPI (P1.1=MISO, P1.2=MOSI, P1.4=CLK)
        P1SEL  |= BIT1 + BIT2 + BIT4;
        P1SEL2 |= BIT1 + BIT2 + BIT4;

        UCA0CTL1 |= UCSWRST;        // Put state machine in reset

        // 3-wire, 8-bit, Master mode, MSB first, Synchronous mode
        // UCCKPH=1 (Data captured on first edge), UCCKPL=0 (Clock inactive low)
        UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
        UCA0CTL1 |= UCSSEL_2;       // Source clock from SMCLK

        UCA0BR0 = 0x02;             // Divide SMCLK by 2 (SPI Clock = 500 kHz)
        UCA0BR1 = 0;

        UCA0CTL1 &= ~UCSWRST;       // Initialize USCI state machine
    }
}
