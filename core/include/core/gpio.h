#ifndef CORE_GPIO_H
#define CORE_GPIO_H

typedef enum {
    GPIO_DIR_IN     = 0,
    GPIO_DIR_OUT    = 1
} gpio_direction;

typedef enum {
    GPIO_OUT_LOW    = 0,
    GPIO_OUT_HIGH   = 1
} gpio_out;

typedef enum gpio_function_t {
    GPIO_FUNC_0     = 0,
    GPIO_FUNC_1     = 1,
    GPIO_FUNC_2     = 2,
    GPIO_FUNC_3     = 3,
    GPIO_FUNC_4     = 4,
    GPIO_FUNC_5     = 5,
    GPIO_FUNC_6     = 6,
    GPIO_FUNC_7     = 7
} gpio_function;

void gpio_set_out(unsigned int bank, unsigned int pin, gpio_out value);
void gpio_set_direction(unsigned int bank, unsigned int pin, gpio_direction dir);
void gpio_set_function(unsigned int bank, unsigned int pin, unsigned char function);

void gpio_toggle_out(unsigned int bank, unsigned int pin);

#endif // CORE_GPIO_H
