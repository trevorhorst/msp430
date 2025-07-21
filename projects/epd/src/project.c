// Core headers
#include "core/gpio.h"
#include "core/spi.h"
#include "core/drivers/ssd1681.h"

// Project headers
#include "project/project.h"

#define ERROR_LED   BIT2

#define SSD1681_LED    GPIO(0, 0)
#define SSD1681_SWTCH  GPIO(0, 3)

#define SSD1681_CHIP_SELECT     GPIO(0, 5)
#define SSD1681_DATA_COMMAND    GPIO(1, 0)
#define SSD1681_RESET           GPIO(1, 1)
#define SSD1681_BUSY            GPIO(1, 2)

static ssd1681_spi_device epd;
static uint8_t screen_byte = 0x00;

// Port 1 Interrupt Service Routine
#pragma vector=PORT1_VECTOR // Specifies the interrupt vector for Port 1
__interrupt void Port_1_ISR(void)
{
    if (P1IFG & BIT3) { // Check if P1.3 caused the interrupt
        // --- Your code to handle the button press goes here ---
        // Example: Toggle an LED on P1.0
        P1OUT ^= BIT0;

        // Debounce (software debounce is often needed for switches)
        // You might introduce a short delay or use a timer to debounce.
        // For simple debouncing:
        __delay_cycles(20000); // Small delay to ignore bounce
        // while (!(P1IN & BIT3)); // Wait for button release (if it's a press event)
        // P1IFG &= ~BIT3; // Clear flag again after debouncing if waiting for release
        gpio_set_out(0, 6, GPIO_OUT_HIGH);
        ssd1681_fill_screen(&epd, screen_byte);
        gpio_set_out(0, 6, GPIO_OUT_LOW);
        // --- IMPORTANT: Clear the interrupt flag! ---
        // If you don't clear the flag, the interrupt will trigger repeatedly.
        P1IFG &= ~BIT3; // Clear P1.3 interrupt flag
        screen_byte = ~screen_byte;
    }

    // If you had other pins on Port 1 interrupting, you'd add more `if` statements
    // to check their respective P1IFG bits.
}

int run( void )
{
    int32_t error = 0;

    // Set MCLK = SMCLK = 16MHz
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    gpio_set_direction(0, 3, GPIO_DIR_IN);
    gpio_set_resistor(0, 3, true, GPIO_RESISTOR_PULLUP);

    // Enable pin 0 on port 1
    // gpio_set_direction(GPIO_BANK(SSD1681_LED), GPIO_PIN(SSD1681_LED), GPIO_DIR_OUT);
    gpio_set_direction(0, 6, GPIO_DIR_OUT);
    // gpio_set_out(GPIO_BANK(SSD1681_LED), GPIO_PIN(SSD1681_LED), GPIO_OUT_HIGH);
    gpio_set_out(0, 6, GPIO_OUT_LOW);

    // Configure P1.1 (SOMI), P1.2 (SIMO), P1.4 (CLK) for USCI_A0 function
    P1SEL |= BIT1 | BIT2 | BIT4;
    P1SEL2 |= BIT1 | BIT2 | BIT4;

    error = spi_initialize();

    ssd1681_initialize(&epd,
                       SSD1681_CHIP_SELECT,
                       SSD1681_DATA_COMMAND,
                       SSD1681_BUSY,
                       SSD1681_RESET);
    ssd1681_initialize_display(&epd);

    gpio_set_out(0, 6, GPIO_OUT_HIGH);
    ssd1681_fill_screen(&epd, 0x00);
    gpio_set_out(0, 6, GPIO_OUT_LOW);

    // Configure P1.3 for interrupt
    P1IES |= BIT3;   // P1.3 Hi/low transition (falling edge for button press)
    P1IFG &= ~BIT3;  // Clear P1.3 interrupt flag (important!)
    P1IE |= BIT3;    // Enable P1.3 interrupt

    _BIS_SR(GIE);

    while( 1 ) {
        // // Toggle pin 0 on port 1
        // gpio_toggle_out(GPIO_BANK(SSD1681_LED), GPIO_PIN(SSD1681_LED));

        // if(P1IN & BIT3) {
        //     gpio_set_out(0, 6, GPIO_OUT_HIGH);
        // } else {
        //     gpio_set_out(0, 6, GPIO_OUT_LOW);
        // }

        // // Use loop as a wait
        // __delay_cycles(16000000 * 2);
        // gpio_set_out(0, 6, GPIO_OUT_HIGH);
        // ssd1681_fill_screen(&epd, 0x0);
        // gpio_set_out(0, 6, GPIO_OUT_LOW);

        // __delay_cycles(16000000 * 2);
        // gpio_set_out(0, 6, GPIO_OUT_HIGH);
        // ssd1681_fill_screen(&epd, 0xFF);
        // gpio_set_out(0, 6, GPIO_OUT_LOW);
    }

    return error;
}
