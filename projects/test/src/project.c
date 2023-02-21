#include "project/project.h"

#include "common/resources.h"

#define ERROR_LED   BIT2

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
    int32_t error = 0;

    // Set the error LED direction to output
    P2DIR |= ERROR_LED;

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

    error = i2c_init();
    if( error ) {
        // Enable the error LED
        P2OUT = BIT2;
        return -1;
        // while (1);
    }

    // This should turn off the display
    P2DIR |= BIT5;
    P2OUT |= BIT5;
    for(volatile unsigned int i = 32000; i > 0; i--);

    // This should turn on the display
    P2OUT &= ~BIT5;

    // The I2C display is located at address 0x3C
    struct i2c_device ssd1306 = { 0x3C };

    error = i2c_check_ack( &ssd1306 );
    if( error ) {
        // Failed to ack device
        P2DIR &= ~ERROR_LED;
        P2OUT &= ~ERROR_LED;
    }

    // Clear any remnants that might be on the screen
    ssd1306_clear_screen( &ssd1306 );

    for(volatile unsigned int i = 32000; i > 0; i--);

    // Initialize display
    ssd1306_init( &ssd1306 );

    // Reset cursor to the 0,0 position
    ssd1306_reset_cursor( &ssd1306 );

    // Fills the screen, to the let user know everything is ready
    ssd1306_fill_screen( &ssd1306 );

    for(volatile unsigned int i = 32000; i > 0; i--);
    for(volatile unsigned int i = 32000; i > 0; i--);

    // for(volatile unsigned int i = 32000; i > 0; i--);

    while( 1 ) {


        // Clear any remnants that might be on the screen
        ssd1306_clear_screen( &ssd1306 );
        ssd1306_reset_cursor( &ssd1306 );

        write_string_to_screen( &ssd1306, please_touch );
        write_string_to_screen( &ssd1306, my_butt );


        // Delay
        for(volatile unsigned int i = 32000; i > 0; i--);
    }

    return 0;
}
