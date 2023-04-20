#include "project/project.h"

#define LOOP_COUNTER    32000

int run( void )
{
    // Enable pin 0 on port 1
    // P1DIR |= BIT0;
    gpio_set_direction(0, 6, GPIO_DIR_OUT);
    gpio_set_direction(0, 0, GPIO_DIR_OUT);

    gpio_toggle_out(0, 0);

    while( 1 ) {
        // Toggle pin 0 on port 1
        // P1OUT ^= BIT0;
        gpio_toggle_out(0, 6);
        gpio_toggle_out(0, 0);

        // Use loop as a wait
        for( uint16_t i = 0; i < LOOP_COUNTER; i++ );
    }

    return 0;
}
