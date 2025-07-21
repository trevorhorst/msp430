#include "core/drivers/ssd1681.h"
#include "core/gpio.h"
#include "core/spi.h"

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

    //while(ssd1681_is_busy(device)) {
        __delay_cycles(160000 * 2);
    //}

    // Init code
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DRIVER_OUTPUT);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xC7);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x01);

    // Data entry sequence setting
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DATA_ENTRY_MODE);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x11);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x18);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_ADDR);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xC7);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_BORDER_WAVEFORM);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x05);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_BUILTIN_TEMP);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x80);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_X_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_RAM_Y_COUNTER);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xC7);
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0x00);

    //while(ssd1681_is_busy(device)) {
        __delay_cycles(160000);
    //}

    gpio_set_out(0, 6, GPIO_OUT_HIGH);

    __delay_cycles(160000 * 2);
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

void ssd1681_fill_screen(ssd1681_spi_device *device, uint8_t byte)
{
    unsigned int i;
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_WRITE_RAM_BW);   //write RAM for black(0)/white (1)
    for(i = 0; i < 5000; i++)
    {
        ssd1681_write(device, SPI_WRITE_TYPE_DATA, byte);
    }

    // ssd1781_write(device, COMMAND, WRITE_RAM_RED);   //write RAM for black(0)/white (1)
    // for(i = 0; i < 5000; i++)
    // {
    //     ssd1781_write(device, DATA, 0x55);
    // }

    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_DISPLAY_UPDATE); //Display Update Control
    ssd1681_write(device, SPI_WRITE_TYPE_DATA, 0xF7);
    ssd1681_write(device, SPI_WRITE_TYPE_COMMAND, SSD1681_COMMAND_MASTER_ACTIVATION);  //Activate Display Update Sequence

    // if(ssd1681_is_busy(device)) {
    //     __delay_cycles(160000 * 1);
    // }
    __delay_cycles(160000 * 2);
}
