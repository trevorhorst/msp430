#include "core/gpio.h"
#include "core/i2c.h"
#include "core/ssd1306.h"

#include "project/project.h"

#define LOOP_COUNTER    32000

//-----------------------------------------------------------------------//
//                Transmit and Receive interrupts                        //
//-----------------------------------------------------------------------//

#pragma vector = USCIAB0TX_VECTOR
__interrupt void TransmitInterrupt(void)
{
  P1OUT  ^= BIT0;//light up P1.0 Led on Tx
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void ReceiveInterrupt(void)
{
  P1OUT  ^= BIT6;     // light up P1.6 LED on RX
  IFG2 &= ~UCA0RXIFG; // Clear RX flag
}

int run(void)
{
    // // Enable pin 0 on port 1
    // // gpio_set_direction(0, 6, GPIO_DIR_OUT);
    // gpio_set_direction(0, 0, GPIO_DIR_OUT);

    // // Initial LED conditions
    // gpio_set_out(0, 0, GPIO_OUT_LOW);
    // // gpio_set_out(0, 6, GPIO_OUT_HIGH);

    // /* Configure P1.6 and P1.7 for I2C */
    // P1SEL  |= BIT6 + BIT7;
    // P1SEL2 |= BIT6 + BIT7;

    // i2c_init();
    // // The I2C display is located at address 0x3C
    // struct i2c_device ssd1306 = { 0x3C };

    // // Clear any remnants that might be on the screen
    // ssd1306_clear_screen( &ssd1306 );

    // for(volatile unsigned int i = 32000; i > 0; i--);

    // // Initialize display
    // ssd1306_init( &ssd1306 );

    // // Reset cursor to the 0,0 position
    // ssd1306_reset_cursor( &ssd1306 );

    // // Fills the screen, to the let user know everything is ready
    // ssd1306_fill_screen( &ssd1306 );

    // while(1) {
    //     // Toggle pin 0 on port 1
    //     // gpio_toggle_out(0, 6);
    //     gpio_toggle_out(0, 0);

    //     // Use loop as a wait
    //     for(uint16_t i = 0; i < LOOP_COUNTER; i++);
    // }

    WDTCTL = WDTPW + WDTHOLD; // Stop the Watch dog

    //------------------- Configure the Clocks -------------------//

    if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
       {
          while(1);          // do not load, trap CPU!!
       }

     DCOCTL  = 0;             // Select lowest DCOx and MODx settings
     BCSCTL1 = CALBC1_1MHZ;   // Set range
     DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

    //---------------- Configuring the LED's ----------------------//

     P1DIR  |=  BIT0 + BIT6;  // P1.0 and P1.6 output
     P1OUT  &= ~BIT0 + BIT6;  // P1.0 and P1.6 = 0

    //--------- Setting the UART function for P1.1 & P1.2 --------//

     P1SEL  |=  BIT1 + BIT2;  // P1.1 UCA0RXD input
     P1SEL2 |=  BIT1 + BIT2;  // P1.2 UCA0TXD output

    //------------ Configuring the UART(USCI_A0) ----------------//

     UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
     UCA0BR0   =  104;                 // 104 From datasheet table-
     UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
     UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
     UCA0STAT |=  UCLISTEN;            // loop back mode enabled
     UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0

    //---------------- Enabling the interrupts ------------------//

     IE2 |= UCA0TXIE;                  // Enable the Transmit interrupt
     IE2 |= UCA0RXIE;                  // Enable the Receive  interrupt
     _BIS_SR(GIE);                     // Enable the global interrupt

     UCA0TXBUF = 'A';                  // Transmit a byte

     _BIS_SR(LPM0_bits + GIE);         // Going to LPM0

    return 0;
}
