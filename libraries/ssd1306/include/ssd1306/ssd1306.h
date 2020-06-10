#ifndef SSD1306_H
#define SSD1306_H

#include "i2c/i2c.h"

void ssd1306_init( struct i2c_device *dev );
void ssd1306_write_char( const struct i2c_device *dev, struct i2c_data *data );
void ssd1306_reset_cursor( const struct i2c_device *dev );
void ssd1306_fill_screen( const struct i2c_device *dev );
void ssd1306_clear_screen( const struct i2c_device *dev );

#endif // SSD1306_H
