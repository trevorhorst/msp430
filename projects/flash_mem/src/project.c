#include <string.h>
#include <stdio.h>

#include "msp430g2553.h"
#include "core/uart.h"
#include "project/project.h"

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

const unsigned char * INFOMEM = (unsigned char *) 0xc000;

const char string[] = { "Hello World\r\n" };
// unsigned int i; //Counter

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    // P1OUT |= TXLED;
    // UCA0TXBUF = string[i++]; // TX next character
    // if (i == sizeof string - 1) { // TX over?
    //    UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    // }
    // P1OUT &= ~TXLED;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    char data[10];
    snprintf(data, 6, "0x%02X\r\n", UCA0RXBUF);
    puts(data);
    IFG2 &= ~UCA0RXIFG; // Clear RX flag

    // Do we need to disable the TX interrupt to avoid clashes?

    // if(UCA0RXBUF == 'a')
    // {
    //     i = 0;
    //     UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
    //     UCA0TXBUF = string[i++];
    // }

    // Toggle LED to indicate received character
    P1OUT |= RXLED;
    P1OUT &= ~RXLED;
}

// Erase entire information memory
int erase(void)
{
    unsigned char * segment;
    segment = (unsigned char *) INFOMEM;

    FCTL2 = FWKEY + FSSEL_1 + FN0 + FN1; // Select MCLK/3 (1.04MHz / 3)

    FCTL3 = FWKEY; // Unlock flash memory
    FCTL1 = FWKEY + ERASE; // Set erase segment mode
    *segment = 0; // Dummy write to segment we are erasing
    while((FCTL3 & WAIT) != WAIT);

    segment += 128; // Point to next segment
     FCTL3 = FWKEY; // Unlock flash memory
    FCTL1 = FWKEY + ERASE; // Set erase segment mode
    *segment = 0; // Dummy write to segment we are erasing
    while((FCTL3 & WAIT) != WAIT); // Wait for erase to complete

    FCTL3 = FWKEY + LOCK; // Lock flash memory
    return 1;
}

// Save a byte to the information memory
int save(char data, unsigned short address)
{
    _DINT();
    char * segment;
    segment = (char *) INFOMEM;

    FCTL2 = FWKEY + FSSEL_1 + FN0; // Select MCLK/3 (1.04MHz / 3)

    FCTL3 = FWKEY; // Unlock flash memory
    FCTL1 = FWKEY + WRT; // Set write byte mode
    segment[address] = data; // Write byte
    while((FCTL3 & WAIT) != WAIT); // Wait for write to complete
    FCTL1 = FWKEY; // Set write byte mode
    FCTL3 = FWKEY + LOCK; // Lock flash memory
    _EINT();
    return address;
}

double readdouble(int offset)
{// Read a double from infomem
    double data;
    int i;
    char * charptr;
    charptr = (char *) &data;

    for(i=0;i<4;i++)
    {
        charptr[i] = read(i + (offset * 4));
    }
    return data;
}

int savedouble(double data, int offset)
{// Save a double into infomem
    char * charptr;
    int i;
    charptr = (char *) &data;

    for(i=0;i<4;i++)
    {
        save(charptr[i], i + (offset * 4));
    }
    return 0;
}

char read(unsigned short address)
{
    char * segment;
    segment = (char *) INFOMEM;
    return segment[address];
}

char puts_data[64];

int run()
{
    // Stop the watch dog timer
    WDTCTL = WDTPW + WDTHOLD;

    P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    P1DIR |= TXLED;
    P1OUT &= 0x00;

    // Sleep for ~1 second
    for(int i = 0; i < INT16_MAX; i++);

    int error = 0;
    uint8_t counter = 0;
    uart_initialize(UART_BAUD_9600);

    puts("\r");

    char data_counter = read(0x3F00);
    snprintf(puts_data, sizeof(puts_data), "data_counter: %d\r", data_counter);
    puts(puts_data);
    data_counter = data_counter + 1;
    snprintf(puts_data, sizeof(puts_data), "data_counter: %d\r", data_counter);
    puts(puts_data);
    save(data_counter, 0x3F00);

    puts("\r");
    puts("\r");
    unsigned char data[16];
    while(1) {
        // Sleep for ~1 second
        for(int i = 0; i < INT16_MAX; i++);
        // puts("Welcome to the MSP430\r");
        for(int i = 0; i < 16; i++) {
            data[i] = read(0x3F00 + ((/*counter * 16*/ 0) + i));
        }
        snprintf(puts_data, sizeof(puts_data), "0x%04x: %04X %04X %04X %04X %04X %04X %04X %04X\r",
                 (counter),
                 ((uint16_t)data[ 1] << 8 | (uint16_t)data[ 0]),
                 ((uint16_t)data[ 3] << 8 | (uint16_t)data[ 2]),
                 ((uint16_t)data[ 5] << 8 | (uint16_t)data[ 4]),
                 ((uint16_t)data[ 7] << 8 | (uint16_t)data[ 6]),
                 ((uint16_t)data[ 9] << 8 | (uint16_t)data[ 8]),
                 ((uint16_t)data[11] << 8 | (uint16_t)data[10]),
                 ((uint16_t)data[13] << 8 | (uint16_t)data[12]),
                 ((uint16_t)data[15] << 8 | (uint16_t)data[14])
                 );
        puts(puts_data);
        counter++;
        save(counter, 0x3F00);
    }
    return error;
}

int putchar(int c)
{
    int error = 0;
    unsigned char data = (unsigned char)c;
    if((c >= 0x00) && (c <= 0xFF)) {
        error = uart_write(&data, sizeof(unsigned char));
    }
    return error;
}
