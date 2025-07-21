#include "core/gpio.h"
#include "hwlib/gpio.h"

void gpio_set_out(unsigned int bank, unsigned int pin, gpio_out value)
{
    hw_gpio_set_out(bank, pin, value);
}

void gpio_set_direction(unsigned int bank, unsigned int pin, gpio_direction dir)
{
    hw_gpio_set_direction(bank, pin, dir);
}

void gpio_set_function(unsigned int bank, unsigned pin, unsigned char function)
{
    hw_gpio_set_function(bank, pin, function);
}

void gpio_set_resistor(unsigned int bank, unsigned int pin, unsigned char enable, unsigned char type)
{
    hw_gpio_set_resistor(bank, pin, enable, type);
}

void gpio_toggle_out(unsigned int bank, unsigned int pin)
{
    hw_gpio_toggle_out(bank, pin);
}

unsigned char gpio_get_input(unsigned int bank, unsigned int pin)
{
    return hw_gpio_get_input(bank, pin);
}
