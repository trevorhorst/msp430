/** ****************************************************************************
 * @author Trevor Horst
 * @file ssd1306.h
 * @brief Declarations for the SSD1306 driver. Some good information on this
 * topic can be found here:
 * https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/610463
 * ****************************************************************************/

#ifndef SSD1306_H
#define SSD1306_H

#include "i2c/i2c.h"

void ssd1306_init( struct i2c_device *dev );
void ssd1306_write_char( const struct i2c_device *dev, struct i2c_data *data );
void ssd1306_reset_cursor( const struct i2c_device *dev );
void ssd1306_fill_screen( const struct i2c_device *dev );
void ssd1306_clear_screen( const struct i2c_device *dev );

#endif // SSD1306_H
