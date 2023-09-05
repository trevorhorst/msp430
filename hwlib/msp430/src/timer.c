#include "hwlib/timer.h"

void hw_timer_initialize(uint16_t timer)
{
    if(timer == 0) {
        // Initialize timer 0
        TA0CCR0  =  0;                      //
        TA0CCR1  =  0;                      //
        TA0CCR2  =  0;                      //
        TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
        TA0CTL   = TASSEL_2 + MC_1 + ID_0;           // SMCLK/8, upmode
    } else if(timer == 1) {
        // Initialize timer 1
        TA1CCR0  =  0;                      //
        TA1CCR1  =  0;                      //
        TA1CCR2  =  0;                      //
        TA1CCTL0 = CCIE;                    // CCR0 interrupt enabled
        TA1CTL   = TASSEL_2 + MC_1 + ID_0;  // SMCLK/1, count up to CCR0
    }
}

void hw_timer_start(uint16_t timer, uint16_t ccr)
{
    if(timer == 0) {
        TA0CCR0 = ccr;
    } else if(timer == 1) {
        TA1CCR0 = ccr;
    }
}
