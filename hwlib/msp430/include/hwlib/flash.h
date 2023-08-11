#ifndef HWLIB_FLASH_H
#define HWLIB_FLASH_H

#include <msp430g2553.h>
#include <stdint.h>

extern volatile uint8_t *flash_memory;

/**
 * @brief hw_flash_read Read n bytes into data from the flash memory starting
 * at provided address
 * @param address Flash offset to read from
 * @param data Container to store read data
 * @param n Number of bytes to read
 * @return Number of bytes read
 */
int16_t hw_flash_read(uint16_t address, uint8_t *data, uint16_t n);

/**
 * @brief hw_flash_erase Erase flash segment containing provided address
 * @param address Flash segment address
 * @return 1 on succes
 */
int16_t hw_flash_erase(uint16_t address);

/**
 * @brief hw_flash_write Write n byyes of data into the flash memory starting
 * at provided address
 * @param address Flash memory offset to write
 * @param data Container to write data from
 * @param n Number of bytes to write
 * @return Number of bytes written
 */
int16_t hw_flash_write(uint16_t address, const uint8_t *data, uint16_t n);

#endif // HWLIB_FLASH_H
