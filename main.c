#include <msp430.h>
#include "msp430/ssd1306.h"
#include "msp430/i2c.h"
#include "msp430/resources.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer. This line of code is needed at the beginning of most MSP430 projects.
                                            // This line of code turns off the watchdog timer, which can reset the device after a certain period of time.
                                            // If you've a lot of initializations, this line prevents the system from falling into infinite loops.

    // P1DIR = 0;
    // P1OUT = 0;

    // P1DIR = BIT0;

    // if( getChar( 1 ) == '5' ) {
    //     P1DIR |= BIT6;
    //     P1OUT |= BIT6;
    // }

    // while(1) {
    //     P1OUT ^= BIT0;
    //     for(volatile unsigned int i = 25000; i > 0; i--);
    // }

    /* Configure P1.1 and P1.2 for UART (USCI_A0) */
    P1SEL |= 0x6;
    P1SEL2 |= 0x6;

    /* Configure P1.6 and P1.7 for I2C */
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    /* Global interrupt enable */
    // __enable_interrupt();

    if (i2c_init() != 0) {
        while (1);
    }

    struct i2c_device ssd1306 = { 0x3C };

    if( i2c_check_ack( &ssd1306 ) != 0 ) {
        // Fail
        P2DIR |= BIT2;
        P2OUT |= BIT2;
    } else {
        // Success
        P1DIR |= BIT0;
        P1OUT |= BIT0;
    }

    ssd1306_init( &ssd1306 );
    ssd1306_reset_cursor( &ssd1306 );
    ssd1306_clear_screen( &ssd1306 );
    ssd1306_fill_screen( &ssd1306 );

    int alternate = 0;
    while(1) {
        P1OUT ^= BIT0;
        ssd1306_reset_cursor( &ssd1306 );

        for(volatile unsigned int i = 25000; i > 0; i--);
        alternate ^= BIT0;
        // if(alternate) {
        //     ssd1306_clear_screen( &ssd1306 );
        // } else {
        //     ssd1306_fill_screen( &ssd1306 );
        // }

    }

    return 0;
}
