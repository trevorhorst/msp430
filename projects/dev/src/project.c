#include "core/gpio.h"
#include "core/i2c.h"
#include "core/ssd1306.h"

#include "project/project.h"

#define LOOP_COUNTER    32000

int run(void)
{
    // Enable pin 0 on port 1
    // gpio_set_direction(0, 6, GPIO_DIR_OUT);
    gpio_set_direction(0, 0, GPIO_DIR_OUT);

    // Initial LED conditions
    gpio_set_out(0, 0, GPIO_OUT_LOW);
    // gpio_set_out(0, 6, GPIO_OUT_HIGH);

    /* Configure P1.6 and P1.7 for I2C */
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    i2c_init();
    // The I2C display is located at address 0x3C
    struct i2c_device ssd1306 = { 0x3C };

    // Clear any remnants that might be on the screen
    ssd1306_clear_screen( &ssd1306 );

    for(volatile unsigned int i = 32000; i > 0; i--);

    // Initialize display
    ssd1306_init( &ssd1306 );

    // Reset cursor to the 0,0 position
    ssd1306_reset_cursor( &ssd1306 );

    // Fills the screen, to the let user know everything is ready
    ssd1306_fill_screen( &ssd1306 );

    while(1) {
        // Toggle pin 0 on port 1
        // gpio_toggle_out(0, 6);
        gpio_toggle_out(0, 0);

        // Use loop as a wait
        for(uint16_t i = 0; i < LOOP_COUNTER; i++);
    }

    return 0;
}
