#include <msp430.h>
#include "string.h"
#include "common/ssd1306.h"
#include "common/i2c.h"
#include "common/resources.h"

#include "project/project.h"

static const char empty_line[]    = "                ";
static const char i_love_you[]    = "   I LOVE YOU   ";
static const char jennifer[]      = "    JENNIFER    ";
static const char killingsworth[] = "  KILLINGSWORTH ";
static const char please_touch[]  = "  PLEASE TOUCH  ";
static const char my_butt[]       = "    MY  BUTT    ";

void write_symbol_to_screen( struct i2c_device *dev, const unsigned char val )
{
    unsigned char map[9];
    memset( &map, 0, 9 );
    const char *symbol = getSymbol( (char)(val) );
    map[0] = 0x40;
    memcpy( &map[1], symbol, SYMBOL_LENGTH );
    struct i2c_data data = { 9, &map };
    ssd1306_write_char( dev, &data );
}

void write_string_to_screen( struct i2c_device *dev, const char *str )
{
    for(unsigned int i = 0; i < strlen( str ); i++ ) {
        write_symbol_to_screen( dev, (unsigned char)str[ i ] );
    }
}

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
    // ssd1306_fill_screen( &ssd1306 );

    unsigned char val = 0;
    while(1) {

        // P1OUT ^= BIT0;


        if( val == 20 ) {
            // ssd1306_clear_screen( &ssd1306 );
            // ssd1306_reset_cursor( &ssd1306 );
            write_string_to_screen( &ssd1306, please_touch );
            write_string_to_screen( &ssd1306, my_butt );
        }

        if( val == 5 ) {
            write_string_to_screen( &ssd1306, empty_line );
            write_string_to_screen( &ssd1306, empty_line );
            write_string_to_screen( &ssd1306, i_love_you );
            write_string_to_screen( &ssd1306, jennifer );
            write_string_to_screen( &ssd1306, killingsworth );
            write_string_to_screen( &ssd1306, empty_line );
        }


        // Delay
        for(volatile unsigned int i = 25000; i > 0; i--);
        val++;
    }

    return 0;
}
