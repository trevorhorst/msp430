#include <stdint.h>

#include "project/project.h"

#define LCD_CS_PIN      BIT4    // P1.4 - Chip Select (Active HIGH)
#define LCD_POWER_PIN	BIT5
#define LCD_SPI_PINS    (BIT1 | BIT2 | BIT3) // P1.1 (CLK), P1.2 (MOSI), P1.3 (MISO)

__attribute__((interrupt(RTC_VECTOR)))
void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF)) {
    case RTCIV_NONE : break;
    case RTCIV_RTCIF:
        __bic_SR_register_on_exit(LPM3_bits);
        break;
    default: break;
    }
}

void init_gpio(void)
{
    // Configure Port A (P1 and P2)
    PAOUT = 0x0000;             // Drive all outputs low
    PADIR = 0xFFFF;             // Set all Port A pins as outputs

    // Configure Port B (P3)
    PBOUT = 0x0000;             // Drive all outputs low
    PBDIR = 0xFFFF;             // Set all Port B pins as outputs
}

void init_rtc(void)
{
    // 10,000 Hz (VLO) / 10 (Prescaler) = 1,000 Hz clock input to counter
    // To count to 1 second, we need 1,000 counts.
    // Since the counter is 0-indexed, we count from 0 to 999.
    RTCMOD = 1000 - 1;

    // Configure the RTC Control Register:
    // RTCSS__VLOCLK: Select the VLO as the clock source
    // RTCPS__10:     Set the prescaler divider to 10
    // RTCSR:         Clear the counter register to start fresh
    // RTCIE:         Enable the match interrupt
    RTCCTL = RTCSS__VLOCLK | RTCPS__10 | RTCSR | RTCIE;
}

void init_spi(void)
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
}

void spi_write_byte(unsigned char data)
{
    // Wait until the transmit buffer is ready for a new byte
    while (!(UCB0IFG & UCTXIFG));

    // Write data to the transmit buffer (this automatically kicks off transmission)
    UCB0TXBUF = data;
}

uint8_t spi_read_byte()
{
    // Wait until the receive buffer has data (data received)
    while (!(UCB0IFG & UCRXIFG));
    return UCA0RXBUF;
}

uint8_t spi_transfer_byte(const uint8_t data)
{
    spi_write_byte(data);
    return spi_read_byte();
}

int32_t spi_transfer(const uint8_t *buffer, int32_t len)
{
    int32_t error = 0;
    while((error == 0) && (len > 0)) {
        spi_transfer_byte((const uint8_t)(*buffer));
        buffer++;
        len--;
    }
    return error;
}

// Call this function periodically (e.g., inside your 1-second RTC wakeup ISR)
void toggle_LCD_VCOM(void)
{
    //static unsigned char vcom_state = 0x40; // Starts with VCOM bit high (0x40)
    static unsigned char vcom_state = 0x02; // Starts with VCOM bit high (0x40)

    P1OUT |= LCD_CS_PIN;                    // CS High

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = vcom_state;                 // Send Toggle VCOM command bit

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = 0x00;                       // 8 trailing dummy clocks

    while (UCB0STATW & UCBUSY);
    P1OUT &= ~LCD_CS_PIN;                   // CS Low

    //vcom_state ^= 0x40;                     // Flip the VCOM bit for the next cycle
    vcom_state ^= 0x02;                     // Flip the VCOM bit for the next cycle
}

void draw_power_indicator(int8_t line)
{
    P1OUT |= LCD_CS_PIN;                    // Select Display (CS High)

    // 1. Send Mode Flag: Write/Update Line Command (0x80)
    spi_write_byte(0x01);

    // 2. Send Line Address: We will write to Line 1
    // The display expects addresses LSB-first. Line 1 binary is 00000001.
    // In LSB-first format, that reverses to 10000000 (0x80).
    spi_write_byte(line);

    // 3. Send 16 Bytes of Row Data (128 pixels total)
    // 0x00 is transparent/white, 0xFF is solid black.
    // Let's clear the first 7 bytes, place a dark indicator in the 8th and 9th bytes,
    // and clear the remaining bytes.

    int i;
    for (i = 0; i < 20; i++) {
        if(line & 0x1) {
            spi_write_byte(0xAA);               // Clear pixels 0 to 55
        } else {
            spi_write_byte(0x55);               // Clear pixels 0 to 55
        }
    }

    // // Centered small dash symbol (Pixels 56 to 71)
    // spi_write_byte(0xFF);
    // spi_write_byte(0xFF);

    // for (i = 0; i < 9; i++) {
    //     spi_write_byte(0xFF);               // Clear pixels 72 to 127
    // }

    // 4. Send 8 trailing dummy clocks to complete the line update
    spi_write_byte(0x00);
    spi_write_byte(0x00);

    // 5. Finalize transfer
    while (UCB0STATW & UCBUSY);             // Wait until the hardware finishes shifting bits
    P1OUT &= ~LCD_CS_PIN;                   // Deselect Display (CS Low)
}

void display_clear(void)
{
    // Select Display (CS High)
    P1OUT |= LCD_CS_PIN;

    // Command code for clear mode
    spi_write_byte(0x04);
    // Traler byte (8 trailing dummy clocks)
    spi_write_byte(0x00);

    // Wait until eUSCI_B0 is no longer busy
    while (UCB0STATW & UCBUSY);

    // Deselect Display (CS Low)
    P1OUT &= ~LCD_CS_PIN;
}

int run( void )
{
    int32_t error = 0;

    init_gpio();
    init_rtc();
    init_spi();

    P1DIR &= ~LCD_POWER_PIN;
    P1REN |= LCD_POWER_PIN;
    P1OUT &= ~LCD_POWER_PIN;

    // Disable the PM5 power-on high-impedance mode to activate configurations
    PM5CTL0 &= ~LOCKLPM5;

    __delay_cycles(50000);

    P1DIR |= LCD_POWER_PIN;
    P1OUT |= LCD_POWER_PIN;                 // Turn panel power ON

    __delay_cycles(50000);

    display_clear();

    int8_t line = 0;
    while( 1 ) {
        // Toggle LED
        P1OUT ^= BIT0;
        // Draw test pattern
        draw_power_indicator(line);
        // Toggle polarity once a second
        toggle_LCD_VCOM();

        line++;
        if(line > 68) {
            line = 0;
            display_clear();
        }

        // Place MCU into Low Power Mode 3
        __bis_SR_register(LPM3_bits | GIE);
    }

    return error;
}
