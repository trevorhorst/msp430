#ifndef HWLIB_TIMER_H
#define HWLIB_TIMER_H

#include <msp430g2553.h>
#include <stdint.h>

void hw_timer_initialize(uint16_t timer);
void hw_timer_start(uint16_t timer, uint16_t ccr);

#endif // HWLIB_TIMER_H
