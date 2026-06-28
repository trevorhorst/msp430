#ifndef SHARP_MEMORY_DISPLAY_H
#define SHARP_MEMORY_DISPLAY_H

#include <stdint.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    int8_t cs;
    int8_t extcomin;
    int8_t disp;
    uint8_t vcom;
} sharp_memory_display;

void sharp_memory_display_init(sharp_memory_display *display);
void sharp_memory_display_clear(sharp_memory_display *display);
void sharp_memory_display_frame_buffer(sharp_memory_display *display, uint8_t *buffer);
void sharp_memory_display_frame_buffer_part(sharp_memory_display *display, uint8_t *buffer,
                                            uint8_t line_start, uint8_t num_lines);
void sharp_memory_display_line(sharp_memory_display *display);
void sharp_memory_display_toggle_vcom(sharp_memory_display *display);

#endif // SHARP_MEMORY_DISPLAY_H
