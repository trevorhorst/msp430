#ifndef DRIVERS_SSD1681_H
#define DRIVERS_SSD1681_H

#include <stdbool.h>

#include "hwlib/spi.h"
#include "hwlib/gpio.h"

#define SSD1681_COMMAND_DRIVER_OUTPUT           0x01
#define SSD1681_COMMAND_GATE_DRIVING_VOLTAGE    0x03
#define SSD1681_COMMAND_SOURCE_DRIVING_VOLTAGE  0x04
#define SSD1681_COMMAND_DEEP_SLEEP_MODE         0x10
#define SSD1681_COMMAND_DATA_ENTRY_MODE         0x11
#define SSD1681_COMMAND_SW_RESET                0x12
#define SSD1681_COMMAND_BUILTIN_TEMP            0x18
#define SSD1681_COMMAND_TEMP_LUT                0x1A
#define SSD1681_COMMAND_MASTER_ACTIVATION       0x20
#define SSD1681_COMMAND_DISPLAY_UPDATE          0x22
#define SSD1681_COMMAND_WRITE_RAM_BW            0x24
#define SSD1681_COMMAND_WRITE_RAM_RED           0x26
#define SSD1681_COMMAND_OTP_READ                0x2D
#define SSD1681_COMMAND_BORDER_WAVEFORM         0x3C
#define SSD1681_COMMAND_RAM_X_ADDR              0x44
#define SSD1681_COMMAND_RAM_Y_ADDR              0x45
#define SSD1681_COMMAND_RAM_X_COUNTER           0x4E
#define SSD1681_COMMAND_RAM_Y_COUNTER           0x4F

typedef struct ssd1681_spi_device_t {
    uint8_t cs;
    uint8_t dc;
    uint8_t busy;
    uint8_t reset;
} ssd1681_spi_device;

void ssd1681_initialize(ssd1681_spi_device *device, uint8_t cs, uint8_t dc, uint8_t busy, uint8_t reset);
void ssd1681_initialize_display(ssd1681_spi_device *device);
void ssd1681_reset(ssd1681_spi_device *device);
bool ssd1681_is_busy(ssd1681_spi_device *device);
bool ssd1681_write(ssd1681_spi_device *device, uint8_t type, uint8_t byte);
void ssd1681_fill_screen(ssd1681_spi_device *device, uint8_t byte);

#endif // DRIVERS_SSD1681_H
