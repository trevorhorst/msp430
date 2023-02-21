#include "common/i2c.h"

static int _transmit(
        const struct i2c_device *dev
        , const unsigned char *buf
        , unsigned int nbytes )
{
    int err = 0;

    /* Send the start condition */
    UCB0CTL1 |= UCTR | UCTXSTT;

    /* Wait for the start condition to be sent and ready to transmit interrupt */
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

    /* Check for ACK */
    err = i2c_check_ack(dev);

    /* If no error and bytes left to send, transmit the data */
    while ((err == 0) && (nbytes > 0)) {
        UCB0TXBUF = *buf;
        while ((IFG2 & UCB0TXIFG) == 0) {
            err = i2c_check_ack(dev);
            if (err < 0) {
                break;
            }
        }

        buf++;
        nbytes--;
    }

    return err;
}

int i2c_check_ack( const struct i2c_device *dev )
{
    int err = 0;
    (void)dev;

    /* Check for ACK */
    if( UCB0STAT & UCNACKIFG ) {
        // Stop the I2C transmission
        UCB0CTL1 |= UCTXSTP;

        // Clear the interrupt flag
        UCB0STAT &= ~UCNACKIFG;

        // Set the error code
        err = -1;
    }

    return err;
}

int i2c_init(void)
{
    /* Ensure USCI_B0 is in reset before configuring */
    UCB0CTL1 = UCSWRST;

    /* Set USCI_B0 to master mode I2C mode */
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    /**
     * Configure the baud rate registers for 100kHz when sourcing from SMCLK
     * where SMCLK = 1MHz
     */
    UCB0BR0 = 10;
    UCB0BR1 = 0;

    /* Take USCI_B0 out of reset and source clock from SMCLK */
    UCB0CTL1 = UCSSEL_2;

    return 0;
}

int i2c_transfer(const struct i2c_device *dev, struct i2c_data *data)
{
    int err = 0;

    /* Set the slave device address */
    UCB0I2CSA = dev->address;

    /* Transmit data is there is any */
    if (data->tx_len > 0) {
        err = _transmit(dev, (const unsigned char *) data->tx_buf, data->tx_len);
    }

    /* Receive data is there is any */
    // if ((err == 0) && (data->rx_len > 0)) {
    //     err = _receive(dev, (uint8_t *) data->rx_buf, data->rx_len);
    // } else {
    //     /* No bytes to receive send the stop condition */
    //     UCB0CTL1 |= UCTXSTP;
    // }

    return err;
}
