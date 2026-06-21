#include <stdint.h>

#include "project/project.h"

__attribute__((interrupt(RTC_VECTOR)))
void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF)) {
    case RTCIV_NONE : break;
    case RTCIV_RTCIF:
        /* Toggle P1.1 */
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

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;
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

int run( void )
{
    int32_t error = 0;

    init_gpio();
    init_rtc();

    /* Toggle P1.0 LED to indicated device start up. */
    P1OUT ^= BIT0;
    __delay_cycles(100000);
    P1OUT ^= BIT0;

    while( 1 ) {
        P1OUT ^= BIT1;
        __bis_SR_register(LPM3_bits | GIE);
    }

    return error;
}
