#include <stdint.h>

#include "core/gpio.h"

#include "project/project.h"

int run(void)
{
    // Enable pin 0 on port 1
    gpio_set_direction(0, 0, GPIO_DIR_OUT);

    while(1) {
        // Toggle pin 0 on port 1
        gpio_toggle_out(0, 0);

        // Use loop as a wait
        for(uint16_t i = 0; i < UINT16_MAX; i++);
    }

    return 0;
}
