#include <msp430.h>

#include "hwlib/spi.h"
#include "project/sharp_memory_display.h"

void sharp_memory_display_init(sharp_memory_display *display)
{
    sharp_memory_display_clear(display);
    display->vcom = 0x02;
}

void sharp_memory_display_clear(sharp_memory_display *display)
{
    // Select Display (CS High)
    P1OUT |= display->cs;

    // Command code for clear mode
    hw_spi_write_byte(0x04);
    // Traler byte (8 trailing dummy clocks)
    hw_spi_write_byte(0x00);

    // Wait until eUSCI_B0 is no longer busy
    while (UCB0STATW & UCBUSY);

    // Deselect Display (CS Low)
    P1OUT &= ~display->cs;
}

void sharp_memory_display_frame_buffer_part(sharp_memory_display *display, uint8_t *buffer,
                                            uint8_t line_start, uint8_t num_lines)
{
    if((line_start + num_lines) > display->height) {
        return;
    }

    // Width  of the display is in number of bytes
    uint32_t width = display->width / 8;

    // Select Display (CS High)
    P1OUT |= display->cs;
    __delay_cycles(6000);
    // Send Mode Flag: Write/Update Line Command (0x01)
    hw_spi_write_byte(0x01);
    for(uint32_t y = line_start; y < (line_start + num_lines); y++) {
        uint32_t offset = y * width;

        // Send Line Address: We will write to Line 1
        hw_spi_write_byte(y + 1);
        hw_spi_write(&buffer[offset], display->width - 97);

        // Send 8 trailing dummy clocks to complete the line update
        hw_spi_write_byte(0x00);
    }
    hw_spi_write_byte(0x00);
    // Finalize transfer
    while (UCB0STATW & UCBUSY);             // Wait until the hardware finishes shifting bits
    __delay_cycles(2000);
    P1OUT &= ~display->cs;                   // Deselect Display (CS Low)
}

void sharp_memory_display_frame_buffer(sharp_memory_display *display, uint8_t *buffer)
{
    sharp_memory_display_frame_buffer_part(display, buffer, 0, display->height);
}

void sharp_memory_display_line(sharp_memory_display *display)
{
    // Height of the display is in number of lines
    uint32_t height = display->height;
    // Width  of the display is in number of bytes
    uint32_t width = display->width / 8;

    // Select Display (CS High)
    P1OUT |= display->cs;
    // Send Mode Flag: Write/Update Line Command (0x01)
    hw_spi_write_byte(0x01);
    // Send Line Address: We will write to Line 1
    hw_spi_write_byte(0x0A);

    // Send 20 Bytes of Row Data (160 pixels total)
    // 0x00 is transparent/white, 0xFF is solid black.
    for(uint32_t x = 0; x < 25; x++) {
        hw_spi_write_byte(0xAA);
    }

    // Send 8 trailing dummy clocks to complete the line update
    hw_spi_write_byte(0x00);
    hw_spi_write_byte(0x00);
    // Finalize transfer
    while (UCB0STATW & UCBUSY);             // Wait until the hardware finishes shifting bits
    P1OUT &= ~display->cs;                   // Deselect Display (CS Low)
}

void sharp_memory_display_toggle_vcom(sharp_memory_display *display)
{
    P1OUT |= display->cs;                    // CS High

    hw_spi_write_byte(display->vcom);
    hw_spi_write_byte(0x00);

    while (UCB0STATW & UCBUSY);
    P1OUT &= ~display->cs;                   // CS Low

    display->vcom ^= 0x02;                     // Flip the VCOM bit for the next cycle
}
