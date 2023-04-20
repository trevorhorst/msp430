#ifndef CORE_GPIO_H
#define CORE_GPIO_H

typedef enum{
    GPIO_DIR_IN     = 0,
    GPIO_DIR_OUT    = 1
} gpio_direction;

typedef enum{
    GPIO_OUT_LOW    = 0,
    GPIO_OUT_HIGH   =1
} gpio_out;

void gpio_set_out(unsigned int bank, unsigned int pin, gpio_out value);
void gpio_set_direction(unsigned int bank, unsigned int pin, gpio_direction dir);

void gpio_toggle_out(unsigned int bank, unsigned int pin);

#endif // CORE_GPIO_H
