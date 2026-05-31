#ifndef DRIVERS_SSD1681_H
#define DRIVERS_SSD1681_H

#include <stdbool.h>

#include "hwlib/spi.h"
#include "hwlib/gpio.h"

#define SSD1681_COMMAND_DRIVER_OUTPUT           0x01
#define SSD1681_COMMAND_GATE_DRIVING_VOLTAGE    0x03
#define SSD1681_COMMAND_SOURCE_DRIVING_VOLTAGE  0x04
#define SSD1681_COMMAND_BOOSTER_SOFT_START		0x0C
#define SSD1681_COMMAND_DEEP_SLEEP_MODE         0x10
#define SSD1681_COMMAND_DATA_ENTRY_MODE         0x11
#define SSD1681_COMMAND_SW_RESET                0x12
#define SSD1681_COMMAND_BUILTIN_TEMP            0x18
#define SSD1681_COMMAND_TEMP_LUT                0x1A
#define SSD1681_COMMAND_MASTER_ACTIVATION       0x20
#define SSD1681_COMMAND_DISPLAY_UPDATE          0x22
#define SSD1681_COMMAND_WRITE_RAM_BW            0x24
#define SSD1681_COMMAND_WRITE_RAM_RED           0x26
#define SSD1681_COMMAND_WRITE_VCOM				0x2C
#define SSD1681_COMMAND_OTP_READ                0x2D
#define SSD1681_COMMAND_WRITE_LUT_REG			0x32
#define SSD1681_COMMAND_BORDER_WAVEFORM         0x3C
#define SSD1681_COMMAND_RAM_X_ADDR              0x44
#define SSD1681_COMMAND_RAM_Y_ADDR              0x45
#define SSD1681_COMMAND_RAM_X_COUNTER           0x4E
#define SSD1681_COMMAND_RAM_Y_COUNTER           0x4F

enum DisplayMode {
    FULL,
    FAST,
    GOOD,  // 6_1 // Might burn the display after days/weeks...
    QUICK, // 2_1
    // REPAIR, // Use overvoltage, and keep it long time one direction then the other
};

typedef struct ssd1681_spi_device_t {
    uint8_t cs;
    uint8_t dc;
    uint8_t busy;
    uint8_t reset;
} ssd1681_spi_device;

void ssd1681_initialize(ssd1681_spi_device *device, uint8_t cs, uint8_t dc, uint8_t busy, uint8_t reset);
void ssd1681_initialize_display(ssd1681_spi_device *device);
void ssd1681_initialize_display_partial(ssd1681_spi_device *device);
void ssd1681_initialize_display_full(ssd1681_spi_device *device);
void ssd1681_reset(ssd1681_spi_device *device);
bool ssd1681_is_busy(ssd1681_spi_device *device);
bool ssd1681_write(ssd1681_spi_device *device, uint8_t type, uint8_t byte);
bool ssd1681_write_array(ssd1681_spi_device *device, const uint8_t *buffer, uint16_t len);
void ssd1681_fill_screen(ssd1681_spi_device *device, uint8_t byte);
void ssd1681_fill_screen_red(ssd1681_spi_device *device, uint8_t byte);
void ssd1681_update_display(ssd1681_spi_device *device);
void ssd1681_partial_update_display(ssd1681_spi_device *device);
void ssd1681_partial_update_full(ssd1681_spi_device *device);
void ssd1681_write_buffer(ssd1681_spi_device *device, const uint8_t *buffer, uint16_t len);
void ssd1681_set_window(ssd1681_spi_device *device, uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end);
void ssd1681_set_window_x(ssd1681_spi_device *device, uint8_t start, uint8_t end);
void ssd1681_set_window_y(ssd1681_spi_device *device, uint8_t start, uint8_t end);
void ssd1681_set_cursor(ssd1681_spi_device *device, uint8_t x, uint8_t y);
void ssd1681_set_cursor_x(ssd1681_spi_device *device, uint8_t x);
void ssd1681_set_cursor_y(ssd1681_spi_device *device, uint8_t x);

void ssd1681_set_partial_ram_area(ssd1681_spi_device *device, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void ssd1681_set_custom_lut(ssd1681_spi_device *device, uint8_t m);
void ssd1681_power_off(ssd1681_spi_device *device);
void ssd1681_partial_refresh(ssd1681_spi_device *device);

#endif // DRIVERS_SSD1681_H
