#include "hwlib/flash.h"

volatile uint8_t *flash_memory = (volatile uint8_t*)0x0000;

int16_t hw_flash_read(uint16_t address, uint8_t *data, uint16_t n)
{
    int16_t r = 0;
    for(uint16_t i = 0; i < n; i++) {
        data[i] = flash_memory[address + i];
        r++;
    }
    return r;
}

int16_t hw_flash_erase(uint16_t address)
{
    // Disable interrupt
    _DINT();
    FCTL2 = FWKEY + FSSEL_1 + FN0; // Select MCLK/3 (1.04MHz / 3)

    // Unlock flash memory
    FCTL3 = FWKEY;
    // Set erase segment mode
    FCTL1 = FWKEY + ERASE; // Set erase segment mode
    flash_memory[address] = 0; // Dummy write to segment we are erasing
    // Wait for write to complete
    while((FCTL3 & WAIT) != WAIT);

    // Lock flash memory
    FCTL3 = FWKEY + LOCK;
    // Enable interrupt
    _EINT();

    return 1;
}

int16_t hw_flash_write(uint16_t address, const uint8_t *data, uint16_t n)
{
    int16_t r = 0;
    // Disable interrupt
    _DINT();
    // SMCLK/2 (1.04MHz / 2)
    FCTL2 = FWKEY + FSSEL_1 + FN0;
    // Unlock Flash memory
    FCTL3 = FWKEY;
    // Set write byte mode
    FCTL1 = FWKEY + WRT;

    for(uint16_t i = 0; i < n; i++) {
        // Write byte
        flash_memory[address + i] = data[i];
        // Wait for write to complete
        while((FCTL3 & WAIT) != WAIT);
        r++;
    }

    // Set write byte mode
    FCTL1 = FWKEY;
    // Lock flash memory
    FCTL3 = FWKEY + LOCK;
    // Enable interrupt
    _EINT();

    return r;
}
