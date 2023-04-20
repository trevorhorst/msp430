#ifndef HWLIB_GPIO_H
#define HWLIB_GPIO_H

void hw_gpio_set_out(unsigned int bank, unsigned int pin, unsigned char value);
void hw_gpio_set_direction(unsigned int bank, unsigned int pin, unsigned char value);

void hw_gpio_toggle_out(unsigned int bank, unsigned int pin);

#endif // HWLIB_GPIO_H
