#ifndef I2C_H
#define I2C_H

#include <msp430g2553.h>
#include <stdint.h>

// static int TXByteCtr;
// static unsigned char PRxData;
// static int Rx = 0;
// static char WHO_AM_I = 0x00;
//
// static char itgAddress = 0x3C;

void init_I2C(void);
void Transmit(void);
void Receive(void);

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

int32_t i2c_initialize(uint8_t bus);
int32_t i2c_write(uint8_t bus, uint8_t address, const uint8_t *data, uint32_t n);

//static int _receive(const struct i2c_device *dev, uint8_t *buf, size_t nbytes)
//{
//    int err = 0;
//    // IGNORE(dev);
//
//    /* Send the start and wait */
//    UCB0CTL1 &= ~UCTR;
//    UCB0CTL1 |= UCTXSTT;
//
//    /* Wait for the start condition to be sent */
//    while (UCB0CTL1 & UCTXSTT);
//
//    /*
//     * If there is only one byte to receive, then set the stop
//     * bit as soon as start condition has been sent
//     */
//    if (nbytes == 1) {
//        UCB0CTL1 |= UCTXSTP;
//    }
//
//    /* Check for ACK */
//    err = _check_ack(dev);
//
//    /* If no error and bytes left to receive, receive the data */
//    while ((err == 0) && (nbytes > 0)) {
//        /* Wait for the data */
//        while ((IFG2 & UCB0RXIFG) == 0);
//
//        *buf = UCB0RXBUF;
//        buf++;
//        nbytes--;
//
//        /*
//         * If there is only one byte left to receive
//         * send the stop condition
//         */
//        if (nbytes == 1) {
//            UCB0CTL1 |= UCTXSTP;
//        }
//    }
//
//    return err;
//}

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
//-------------------------------------------------------------------------------
// __attribute__((interrupt(USCIAB0TX_VECTOR))) void USCIAB0TX_ISR(void)
// {
//   if(Rx == 1){                              // Master Recieve?
//       PRxData = UCB0RXBUF;                       // Get RX data
//       __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
//   }
//
//   else{                                     // Master Transmit
//       if (TXByteCtr)                            // Check TX byte counter
//         {
//           UCB0TXBUF = WHO_AM_I;                     // Load TX buffer
//           TXByteCtr--;                            // Decrement TX byte counter
//         }
//         else
//         {
//           UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
//           IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
//           __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
//         }
//  }
//
// }
// void init_I2C(void) {
//       P1SEL  |= ( BIT6 + BIT7 );                     // Assign I2C pins to USCI_B0
//       P1SEL2 |= ( BIT6 + BIT7 );                     // Assign I2C pins to USCI_B0
//       UCB0CTL1 |= UCSWRST;                      // Enable SW reset
//       UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
//       UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
//       UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
//       UCB0BR1 = 0;
//       UCB0I2CSA = itgAddress;                         // Slave Address is 0x3Ch
//       UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//       IE2 |= UCB0RXIE + UCB0TXIE;               //Enable RX and TX interrupt
// }
//
// void Transmit(void){
//     while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
//     UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
//     __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
// }
// void Receive(void){
//         while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
//         UCB0CTL1 &= ~UCTR ;                     // Clear UCTR
//         UCB0CTL1 |= UCTXSTT;                    // I2C start condition
//         while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
//         UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
//         __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
// }

#endif // I2C_H
