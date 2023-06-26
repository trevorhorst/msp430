#include "project/project.h"

#define LOOP_COUNTER    32000


#pragma vector=PORT1_VECTOR
__attribute__((__interrupt__)) void Port_1(void)
{
    __delay_cycles(500000);
    P1OUT ^= BIT6;
    P1IFG = ~BIT3;
}

int run( void )
{
    // Enable pin 0 and pin 6 on port 1
    P1DIR |= (BIT0 | BIT6);

    P1REN = BIT3;
    P1OUT = BIT3;
    P1IE = BIT3;
    __enable_interrupt();

    while( 1 ) {
        // Toggle pin 0 and pin 6 on port 1
        P1OUT ^= BIT0;
        P1OUT ^= BIT6;

        // Use loop as a wait
        for( uint16_t i = 0; i < LOOP_COUNTER; i++ );
    }

    return 0;
}

