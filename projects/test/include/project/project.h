/** ****************************************************************************
 * @author Trevor Horst
 * @file project.h
 * @brief Project header file. Contains function prototypes unique to this
 * project.
 * ****************************************************************************/

#include <msp430g2553.h>
#include <string.h>

#include "ssd1306/ssd1306.h"

extern const char empty_line[];
extern const char i_love_you[];
extern const char jennifer[];
extern const char killingsworth[];
extern const char please_touch[];
extern const char my_butt[];

/**
 * @brief Execute flow control for the project
 * @return int error code
 */
int run( void );

/**
 * @brief Writes a desired symbol to a specified device
 * @param dev Desired devie to write
 * @param val Desired symbol to write
 */
void write_symbol_to_screen( struct i2c_device *dev, const unsigned char val );

/**
 * @brief Writes a string to a specified device
 * @param dev Desired device to write
 * @param str Desired string to write
 */
void write_string_to_screen( struct i2c_device *dev, const char *str );
