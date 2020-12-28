#include "project/project.h"

#define LOOP_COUNTER    20000

int run( void )
{
    // Enable pin 0 on port 1
    P1DIR |= BIT0;

    while( 1 ) {
        // Toggle pin 0 on port 1
        P1OUT ^= BIT0;

        // Use loop as a wait
        for( uint16_t i = 0; i < LOOP_COUNTER; i++ );
    }

    return 0;
}
