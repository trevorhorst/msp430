/** ****************************************************************************
 * @author Trevor Horst
 * @file i2c.h
 * @brief Declarations for the MSP430 I2C driver.Some good information on this
 * topic can be found here:
 * http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
 * ****************************************************************************/

#ifndef I2C_H
#define I2C_H

struct i2c_device
{
    unsigned char address;
};

struct i2c_data
{
    unsigned int tx_len;
    const void *tx_buf;
    // void *rx_buf;
    // unsigned int rx_len;
};

int i2c_check_ack( const struct i2c_device *dev );
int i2c_init( void );
int i2c_transfer( const struct i2c_device *dev, struct i2c_data *data );

#endif // I2C_H
