#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "core/gpio.h"
#include "core/uart.h"

#include "project/project.h"

#define BIT(X)  (1 << X)

#define LAUNCHPAD_PORT1 0

#define LAUNCHPAD_LED1      0
#define LAUNCHPAD_UART_RX   BIT(1)
#define LAUNCHPAD_UART_TX   BIT(2)
#define LAUNCHPAD_LED2      6

#define CONSOLE_BUFFER_SIZE 128

#define false   0
#define true    1
typedef uint8_t bool;

static char console_buffer[CONSOLE_BUFFER_SIZE] = {0};
static uint8_t console_buffer_index = 0;
static bool command_ready = false;

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    // P1OUT |= TXLED;
    // UCA0TXBUF = string[i++]; // TX next character
    // if (i == sizeof string - 1) { // TX over?
    //    UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    // }
    // P1OUT &= ~TXLED;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    // Toggle LED to indicate received character
    gpio_set_direction(LAUNCHPAD_PORT1, LAUNCHPAD_LED2, GPIO_DIR_OUT);
    gpio_set_out(LAUNCHPAD_PORT1, LAUNCHPAD_LED2, GPIO_OUT_HIGH);


    // Print hex value of received character
    // char data[10];
    // snprintf(data, 6, "0x%02X\r\n", UCA0RXBUF);
    // puts(data);

    // Buffer received character and retransmit
    volatile unsigned char to_buffer = UCA0RXBUF;
    if(command_ready) {
        // Do nothing until command has been handled
    } else if(to_buffer == '\r') {
        putchar('\r');
        putchar('\n');
        console_buffer[console_buffer_index] = '\0';
        command_ready = true;
    } else {
        putchar(to_buffer);
        console_buffer[console_buffer_index] = to_buffer;
        console_buffer_index++;
    }

    // Clear RX flag
    IFG2 &= ~UCA0RXIFG;

    gpio_set_out(LAUNCHPAD_PORT1, LAUNCHPAD_LED2, GPIO_OUT_LOW);
}

void handle_command()
{
    printf("Handled command: %s\r\n", console_buffer);

    memset(console_buffer, 0, sizeof(console_buffer));
    console_buffer_index = 0;
    command_ready = false;
}


int run(void)
{
    // Stop the watch dog timer
    WDTCTL = WDTPW + WDTHOLD;

    // Configure LaunchPad UART pins. I don't have a core library method for this.
    P1SEL |= LAUNCHPAD_UART_RX + LAUNCHPAD_UART_TX;
    P1SEL2 |= LAUNCHPAD_UART_RX + LAUNCHPAD_UART_TX;

    // Initialize UART hardware
    uart_initialize(UART_BAUD_9600);
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
    // UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
    // Configure LPM0 and the Global Interrupt Enable
    _BIS_SR(/*LPM0_bits +*/ GIE);

    // Enable pin 0 on port 1
    gpio_set_direction(LAUNCHPAD_PORT1, LAUNCHPAD_LED1, GPIO_DIR_OUT);

    puts("Welcome to the MSP430G2553 LaunchPad!\r");

    while(1) {
        if(command_ready) {
            handle_command();
        }

        // Toggle pin 0 on port 1
        gpio_toggle_out(LAUNCHPAD_PORT1, LAUNCHPAD_LED1);

        // Use loop as a wait
        for(uint16_t i = 0; i < UINT16_MAX; i++);
    }

    return 0;
}

int putchar(int c)
{
    int error = 0;
    unsigned char data = (unsigned char)c;
    if((c >= 0x00) && (c <= 0xFF)) {
        error = uart_write(&data, sizeof(unsigned char));
    }
    return error;
}
