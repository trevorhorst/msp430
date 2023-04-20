#include <msp430g2553.h>

#include "hwlib/gpio.h"

void hw_gpio_set_out(unsigned int bank, unsigned int pin, unsigned char value)
{
    volatile unsigned char *b = 0;
    if(bank == 0) {
        b = &P1OUT;
    } else if(bank == 1) {
        b = &P2OUT;
    }

    if(b) {
        value ? (*b |= (1 << pin)) : (*b &= ~(1 << pin));
    }
}

void hw_gpio_set_direction(unsigned int bank, unsigned int pin, unsigned char value)
{
    volatile unsigned char *b = 0;
    if(bank == 0) {
        b = &P1DIR;
    } else if(bank == 1) {
        b = &P2DIR;
    }

    if(b) {
        value ? (*b |= (1 << pin)) : (*b &= ~(1 << pin));
    }
}

void hw_gpio_toggle_out(unsigned int bank, unsigned int pin)
{
    volatile unsigned char *b = 0;
    if(bank == 0) {
        b = &P1OUT;
    } else if(bank == 1) {
        b = &P2OUT;
    }

    if(b) {
        *b ^= (1 << pin);
    }
}
