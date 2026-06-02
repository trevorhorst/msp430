#include "core/drivers/ssd1681.h"
#include "core/gpio.h"
#include "core/spi.h"

static enum DisplayMode mode = FULL;

static const uint8_t lut_default_full[] =
    {
        0x32,  // command
        0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t lut_default_part[] =
    {
        0x32,  // command
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void ssd1681_initialize(ssd1681_spi_device *device, uint8_t cs, uint8_t dc, uint8_t busy, uint8_t reset)
{
    if(device) {
        device->cs = cs;
        device->dc = dc;
        device->busy = busy;
        device->reset = reset;

        gpio_set_direction(GPIO_BANK(device->busy), GPIO_PIN(device->busy), GPIO_DIR_IN);
        gpio_set_resistor(GPIO_BANK(device->busy), GPIO_PIN(device->busy), true, GPIO_RESISTOR_PULLDOWN);
    }
}

void ssd1681_initialize_display(ssd1681_spi_device *device)
{
    ssd1681_reset(device);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_SW_RESET);

    while(ssd1681_is_busy(device)) {
        __delay_cycles(160000 * 2);
    }

    uint16_t height = 200;
    uint16_t width  = 200;

    // Init code
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DRIVER_OUTPUT);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, (height - 1) % 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, (height - 1) / 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_BORDER_WAVEFORM);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x05);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_BUILTIN_TEMP);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x80);

    // ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_BOOSTER_SOFT_START);
    // ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xD7);
    // ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xD6);
    // ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x9D);

    // ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_VCOM);
    // ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x9B);

    ssd1681_set_partial_ram_area(device, 0, 0, width, height);

    while(ssd1681_is_busy(device)) {
        __delay_cycles(160000);
    }

    __delay_cycles(160000 * 2);
}

void ssd1681_initialize_display_partial(ssd1681_spi_device *device)
{
    ssd1681_initialize_display(device);

    const uint8_t *lut = lut_default_part;

    // Configure Data/Command pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_DIR_OUT);
    // The Data/Command pin should be low when writing the command
    gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_LOW);
    // Configure Chip Select pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_DIR_OUT);
    // Take Chip Select low when sending data
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_LOW);

    // Write the command
    spi_write(&lut[0], 1);

    // The Data/Command pin should be high when writing the data
    gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_HIGH);

    // Write the data
    for(uint8_t i = 0; i < sizeof(lut_default_part) - 1; i++) {
        spi_write(&lut[i + 1], 1);
    }

    // Take Chip Select high when finished
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_HIGH);
}

void ssd1681_initialize_display_full(ssd1681_spi_device *device)
{
    ssd1681_initialize_display(device);

    const uint8_t *lut = lut_default_full;

    // Configure Data/Command pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_DIR_OUT);
    // The Data/Command pin should be low when writing the command
    gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_LOW);
    // Configure Chip Select pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_DIR_OUT);
    // Take Chip Select low when sending data
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_LOW);

    // Write the command
    spi_write(&lut[0], 1);

    // The Data/Command pin should be high when writing the data
    gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_HIGH);

    // Write the data
    for(uint8_t i = 0; i < sizeof(lut_default_full) - 1; i++) {
        spi_write(&lut[i + 1], 1);
    }

    // Take Chip Select high when finished
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_HIGH);
}

void ssd1681_reset(ssd1681_spi_device *device)
{
    gpio_set_direction(GPIO_BANK(device->reset), GPIO_PIN(device->reset), GPIO_DIR_OUT);
    gpio_set_out(GPIO_BANK(device->reset), GPIO_PIN(device->reset), GPIO_OUT_HIGH);
    __delay_cycles(160000);
    gpio_set_out(GPIO_BANK(device->reset), GPIO_PIN(device->reset), GPIO_OUT_LOW);
    __delay_cycles(160000);
    gpio_set_out(GPIO_BANK(device->reset), GPIO_PIN(device->reset), GPIO_OUT_HIGH);
    __delay_cycles(160000);

    if(ssd1681_is_busy(device)) {
        __delay_cycles(160000 * 2);
    }

    /// @note Can we extract the clock speed from this an adjust our delay accordingly?
    // // Set MCLK = SMCLK = 1MHz
    // BCSCTL1 = CALBC1_1MHZ;
    // DCOCTL = CALDCO_1MHZ;

}

bool ssd1681_is_busy(ssd1681_spi_device *device)
{
    gpio_set_direction(GPIO_BANK(device->busy), GPIO_PIN(device->busy), GPIO_DIR_IN);
    return gpio_get_input(GPIO_BANK(device->busy), GPIO_PIN(device->busy));
}

bool ssd1681_write(ssd1681_spi_device *device, uint8_t type, uint8_t byte)
{
    // Configure Data/Command pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_DIR_OUT);

    if(type == SPI_WRITE_TYPE_COMMAND) {
        // The Data/Command pin should be low when writing the command
        gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_LOW);
    } else {
        // The Data/Command pin should be high when writing the data
        gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_HIGH);
    }

    // Configure Chip Select pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_DIR_OUT);
    // Take Chip Select low when sending data
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_LOW);
    // Write the byte
    spi_write(&byte, 1);
    // Take Chip Select high when finished
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_HIGH);
    return true;
}

bool ssd1681_write_array(ssd1681_spi_device *device, const uint8_t *buffer, uint16_t len)
{
    // Configure Data/Command pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_DIR_OUT);
    // The Data/Command pin should be high when writing the data
    gpio_set_out(GPIO_BANK(device->dc), GPIO_PIN(device->dc), GPIO_OUT_HIGH);
    // Configure Chip Select pin for output (just in case)
    gpio_set_direction(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_DIR_OUT);
    // Take Chip Select low when sending data
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_LOW);

    // Write the byte
    spi_write(buffer, len);

    // Take Chip Select high when finished
    gpio_set_out(GPIO_BANK(device->cs), GPIO_PIN(device->cs), GPIO_OUT_HIGH);
    return true;
}

void ssd1681_fill_screen(ssd1681_spi_device *device, uint8_t byte)
{
    unsigned int i;
    ssd1681_set_partial_ram_area(device, 0, 0, 200, 200);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_RAM_BW);   //write RAM for black(0)/white (1)
    for(i = 0; i < 5000; i++)
    {
        ssd1681_write(device, SPI_WRITE_TYPE_DATA, byte);
    }

    ssd1681_update_display(device);
}

void ssd1681_fill_screen_red(ssd1681_spi_device *device, uint8_t byte)
{
    unsigned int i;
    ssd1681_set_partial_ram_area(device, 0, 0, 200, 200);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_RAM_RED);   //write RAM for black(0)/white (1)
    for(i = 0; i < 5000; i++)
    {
        ssd1681_write(device, SPI_WRITE_TYPE_DATA, byte);
    }

    //ssd1681_update_display(device);
}

void ssd1681_update_display(ssd1681_spi_device *device)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE); //Display Update Control
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xF7);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);  //Activate Display Update Sequence

    while(ssd1681_is_busy(device)) {
        __delay_cycles(16000);
    }

    mode = FULL;
}

void ssd1681_partial_update_display(ssd1681_spi_device *device)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE); //Display Update Control
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xFC);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);  //Activate Display Update Sequence

    while(ssd1681_is_busy(device)) {
        __delay_cycles(16000);
    }
}

void ssd1681_write_buffer(ssd1681_spi_device *device, const uint8_t *buffer, uint16_t len)
{
    unsigned int i;
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_RAM_BW);   //write RAM for black(0)/white (1)
    for(i = 0; i < len; i++) {
        ssd1681_write(device, SPI_WRITE_TYPE_DATA, buffer[i]);
    }
}

void ssd1681_set_window(ssd1681_spi_device *device, uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end)
{
    ssd1681_set_window_x(device, x_start, x_end);
    ssd1681_set_window_y(device, y_start, y_end);
}

void ssd1681_set_window_x(ssd1681_spi_device *device, uint8_t start, uint8_t end)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, start);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, end);
}

void ssd1681_set_window_y(ssd1681_spi_device *device, uint8_t start, uint8_t end)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, start & 0xFF);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, end & 0xFF);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
}

void ssd1681_set_cursor(ssd1681_spi_device *device, uint8_t x, uint8_t y)
{
    ssd1681_set_cursor_x(device, x);
    ssd1681_set_cursor_y(device, y);
}

void ssd1681_set_cursor_x(ssd1681_spi_device *device, uint8_t x)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, x);
}

void ssd1681_set_cursor_y(ssd1681_spi_device *device, uint8_t y)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, y & 0xFF);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
}

void ssd1681_set_partial_ram_area(ssd1681_spi_device *device, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DATA_ENTRY_MODE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x03);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, x / 8);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, (x + w - 1) / 8);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, y % 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, y / 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, (y + h - 1) % 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, (y + h - 1) / 256);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, x / 8);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, y % 256);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, y / 256);

    // _writeCommand(0x11); // set ram entry mode
    // _writeData(0x03);    // x increase, y increase : normal mode
    // _writeCommand(0x44);
    // _writeData(x / 8);
    // _writeData((x + w - 1) / 8);
    // _writeCommand(0x45);
    // _writeData(y % 256);
    // _writeData(y / 256);
    // _writeData((y + h - 1) % 256);
    // _writeData((y + h - 1) / 256);
    // _writeCommand(0x4e);
    // _writeData(x / 8);
    // _writeCommand(0x4f);
    // _writeData(y % 256);
    // _writeData(y / 256);
}

void ssd1681_power_on(ssd1681_spi_device *device)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xC0);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);
    if(ssd1681_is_busy(device)) {
        __delay_cycles(160000 * 200);
    }
}

void ssd1681_power_off(ssd1681_spi_device *device)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xC3);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);
    if(ssd1681_is_busy(device)) {
        __delay_cycles(160000 * 200);
    }
}

static const uint8_t quick_lut[] = {0x0010,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00A0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0050,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0002,0x0001,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

void ssd1681_set_custom_lut(ssd1681_spi_device *device, uint8_t m)
{
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_LUT_REG);

    const uint8_t *lut = quick_lut;
    ssd1681_write_array(device, lut, 153);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, 0x3F);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x07);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_GATE_DRIVING_VOLTAGE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x17);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_SOURCE_DRIVING_VOLTAGE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x41);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x32);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_VCOM);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x20);

    // .eopt {0x07}, // "keep previous output before power off"
    // .vgh {0x17}, // Max 20V
    // .vsh1_vsh2_vsl {{0x41, 0x0, 0x32}}, //15/0/-15
    // .vcom {0x20}, // VCOM 0x20 best
}

void ssd1681_partial_refresh(ssd1681_spi_device *device)
{
    uint8_t kTurnOn = 0b11000000; // Enables oscillator & analog
    uint8_t kLoadTemp = 0b00100000;
    uint8_t kLoadLut = 0b00010000;
    uint8_t kPartialMode = 0b00001000;
    uint8_t kDisplay = 0b00000100;
    // constexpr auto kTurnOff = 0b00000011; // Disables oscillator & analog

    // Default modes FAST/FULL are loaded from the ROM
    bool romLut = false;

    // Build default updateCommand
    uint8_t updateCommand = kTurnOn;
    if (romLut) {
        updateCommand |= kLoadLut;
    } else {
        ssd1681_set_custom_lut(device, mode);
    }

    if(mode != FULL) {
        updateCommand |= kPartialMode;
    }
    // if (!kFastUpdateTemp) {
    //     updateCommand |= kLoadTemp;
    // }

    // If we are chaging from FULL to FAST/CUSTOM need to set
    // the display into 2 buffer mode again by triggering an update
    if(mode == FULL) {
        ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE);
        ssd1681_write(device, SPI_WRITE_TYPE_DATA, updateCommand);
        ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);
        if(ssd1681_is_busy(device)) {
            __delay_cycles(160000 * 200);
        }
    }

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, updateCommand | kDisplay);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);  //Activate Display Update Sequence
    // kState.fullMode = mode == DisplayMode::FULL;
    mode = QUICK;
}
