#include <msp430g2553.h>
#include <string.h>

#include "ssd1306/ssd1306.h"

extern const char empty_line[];
extern const char i_love_you[];
extern const char jennifer[];
extern const char killingsworth[];
extern const char please_touch[];
extern const char my_butt[];

int run( void );

void write_symbol_to_screen( struct i2c_device *dev, const unsigned char val );
void write_string_to_screen( struct i2c_device *dev, const char *str );
