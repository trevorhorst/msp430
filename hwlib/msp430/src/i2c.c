#include "hwlib/i2c.h"

int32_t _check_ack(void)
{
    int32_t error = 0;
    /* Check for ACK */
    if(UCB0STAT & UCNACKIFG) {
        // Stop the I2C transmission
        UCB0CTL1 |= UCTXSTP;

        // Clear the interrupt flag
        UCB0STAT &= ~UCNACKIFG;

        // Set the error code
        error = -1;
    }
    return error;
}

int32_t hw_i2c_initialize(uint8_t bus)
{
    int32_t error = 0;

    if(bus != 0) {
        // We only have bus 0 on the msp430
        error = -1;
    } else {
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
    }

    return error;
}

int32_t hw_i2c_write(uint8_t bus, uint8_t address, const uint8_t *data, uint32_t n)
{
    int32_t error = 0;

    (void)bus;

    // Set the slave device address
    UCB0I2CSA = address;

    if(data && (n > 0)) {

        // Send the start condition
        UCB0CTL1 |= UCTR | UCTXSTT;

        // Wait for the start condition to be sent and ready to transmit interrupt
        while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

        // Check for ACK
        error = _check_ack();

        // If no error and bytes left to send, transmit the data
        while((error == 0) && (n > 0)) {
            UCB0TXBUF = *data;
            while ((IFG2 & UCB0TXIFG) == 0) {
                error = _check_ack();
                if (error < 0) {
                    break;
                }
            }

            data++;
            n--;
        }
    }

    return error;
}

int32_t hw_i2c_read(uint8_t bus, uint8_t address, uint8_t *data, uint32_t n)
{
    int32_t error = 0;

    /* Send the start and wait */
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;

    /* Wait for the start condition to be sent */
    while (UCB0CTL1 & UCTXSTT);

    /*
     * If there is only one byte to receive, then set the stop
     * bit as soon as start condition has been sent
     */
    if(n == 1) {
        UCB0CTL1 |= UCTXSTP;
    }

    /* Check for ACK */
    error = _check_ack();

    /* If no error and bytes left to receive, receive the data */
    while((error == 0) && (n > 0)) {
        /* Wait for the data */
        while((IFG2 & UCB0RXIFG) == 0);

        *data = UCB0RXBUF;
        data++;
        n--;

        /*
         * If there is only one byte left to receive
         * send the stop condition
         */
        if(n == 1) {
            UCB0CTL1 |= UCTXSTP;
        }
    }

    return error;
}
