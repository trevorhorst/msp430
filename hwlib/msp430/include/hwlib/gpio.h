#ifndef HWLIB_GPIO_H
#define HWLIB_GPIO_H

#define GPIO_FUNCTION_CAPD(X)      ((X & 0x1) << 0)
#define GPIO_FUNCTION_JTAG(X)      ((X & 0x1) << 1)
#define GPIO_FUNCTION_ADC10AE(X)   ((X & 0x1) << 2)
#define GPIO_FUNCTION_PSEL2(X)     ((X & 0x1) << 3)
#define GPIO_FUNCTION_PSEL(X)      ((X & 0x1) << 4)
#define GPIO_FUNCTION_PDIR(X)      ((X & 0x1) << 5)

// typedef struct hw_gpio_function_t {
//     unsigned char p1sel : 1;
//     unsigned char p1sel2 : 1;
// } hw_gpio_function;
//
// extern const hw_gpio_function hw_gpio_p1[7][10];

void hw_gpio_set_out(unsigned int bank, unsigned int pin, unsigned char value);
void hw_gpio_set_direction(unsigned int bank, unsigned int pin, unsigned char value);
void hw_gpio_set_function(unsigned int bank, unsigned int pin, unsigned char value);
void hw_gpio_set_value(unsigned int bank, unsigned int pin, unsigned char value);

void hw_gpio_toggle_out(unsigned int bank, unsigned int pin);

#endif // HWLIB_GPIO_H
