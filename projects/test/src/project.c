#include "project/project.h"

#include "common/resources.h"

const char empty_line[]    = "                ";
const char i_love_you[]    = "   I LOVE YOU   ";
const char jennifer[]      = "    JENNIFER    ";
const char killingsworth[] = "  KILLINGSWORTH ";
const char please_touch[]  = "  PLEASE TOUCH  ";
const char my_butt[]       = "    MY  BUTT    ";

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

int run( void )
{
    /* Configure P1.1 and P1.2 for UART (USCI_A0) */
    // P1SEL |= 0x6;
    // P1SEL2 |= 0x6;
    P2REN |= BIT0;
    P2DIR |= BIT0;
    P2OUT &= ~BIT0;

    /* Configure P1.6 and P1.7 for I2C */
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    /* Global interrupt enable */
    // __enable_interrupt();

    if (i2c_init() != 0) {
        while (1);
    }

    // This should turn off the display
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    for(volatile unsigned int i = 25000; i > 0; i--);

    struct i2c_device ssd1306 = { 0x3C };

    if( i2c_check_ack( &ssd1306 ) != 0 ) {
        // Failed to ack device
        P2DIR &= ~BIT2;
        P2OUT &= ~BIT2;
    } else {
        // Successfully acked the device
        P2DIR |= BIT2;
        P2OUT |= BIT2;
    }

    P1OUT |= BIT0;
    ssd1306_clear_screen( &ssd1306 );
    ssd1306_init( &ssd1306 );
    ssd1306_reset_cursor( &ssd1306 );
    // ssd1306_fill_screen( &ssd1306 );

    unsigned char val = 0;
    while(1) {

        // P1OUT ^= BIT0;

        // if( val == 20 ) {
        //     // ssd1306_clear_screen( &ssd1306 );
        //     // ssd1306_reset_cursor( &ssd1306 );
        // }

        if( (val % 5) == 0 ) {
            P1OUT ^= BIT0;
            if( P1OUT & BIT0 ){
                ssd1306_init( &ssd1306 );
                ssd1306_reset_cursor( &ssd1306 );
                ssd1306_clear_screen( &ssd1306 );
                write_string_to_screen( &ssd1306, empty_line );
                write_string_to_screen( &ssd1306, empty_line );
                write_string_to_screen( &ssd1306, i_love_you );
                write_string_to_screen( &ssd1306, jennifer );
                write_string_to_screen( &ssd1306, killingsworth );
                write_string_to_screen( &ssd1306, empty_line );
                write_string_to_screen( &ssd1306, please_touch );
                write_string_to_screen( &ssd1306, my_butt );
            }
        }


        // Delay
        for(volatile unsigned int i = 25000; i > 0; i--);
        val++;
    }

    return 0;
}
