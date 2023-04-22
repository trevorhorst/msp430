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

void hw_gpio_set_value(unsigned int bank, unsigned int pin, unsigned char value)
{
    volatile unsigned char *b1 = 0;
    volatile unsigned char *b2 = 0;
    if(bank == 0) {
        b1 = &P1SEL;
        b2 = &P1SEL2;
    } else if(bank == 1) {
        b1 = &P2SEL;
        b2 = &P2SEL2;
    }

    if(b1 && b2) {
        /// @todo Need to finish filling this out... this will take some effort.
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
