/**
 * @file project.c
 * @author Trevor Horst
 * @brief Example project entry point
 *
 * This is an example project that demonstrates how to use the various features
 * providded by the MSP430G2553. This includes how to use the GPIO, UART,
 * interrupts and reading, writing and erasing the flash memory.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/gpio.h"
#include "core/uart.h"

#include "hwlib/flash.h"

#include "project/project.h"


#define BIT(X)  (1 << X)

#define LAUNCHPAD_PORT1 0

#define LAUNCHPAD_LED1      0
#define LAUNCHPAD_UART_RX   BIT(1)
#define LAUNCHPAD_UART_TX   BIT(2)
#define LAUNCHPAD_LED2      6

#define CONSOLE_BUFFER_SIZE 128
#define CONSOLE_CMD_ARGS_MAX    6
#define CONSOLE_DELIMITER   ' '

#define false   0
#define true    1
typedef uint8_t bool;

static char console_buffer[CONSOLE_BUFFER_SIZE] = {0};
static uint8_t console_cursor = 0;
static bool command_ready = false;
static uint16_t timer_counter = 0;

typedef struct callback_t {
    const char *command;
    int16_t (*call)(int32_t, char **);
} callback;

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
        // Evaluate the input string
        printf("\r%s\r\n", console_buffer);
        console_buffer[console_cursor] = '\0';
        command_ready = true;
    } else if(to_buffer == '\b') {
        if(console_cursor > 0) {
            console_cursor--;
            console_buffer[console_cursor] = '\0';
            printf("\r%s ", console_buffer);
            printf("\r%s", console_buffer);
        }

    } else {
        // Add character to the buffer if there is room
        if(console_cursor < CONSOLE_BUFFER_SIZE) {
            console_buffer[console_cursor] = to_buffer;
            console_cursor++;
            // Redraw the buffer to screen
            printf("\r%s", console_buffer);
        }

    }

    // Clear RX flag
    IFG2 &= ~UCA0RXIFG;

    gpio_set_out(LAUNCHPAD_PORT1, LAUNCHPAD_LED2, GPIO_OUT_LOW);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMERA_ISR(void)
{
    timer_counter++;
    if(timer_counter == 1000) {
        // Triggers about once a second
        gpio_set_direction(LAUNCHPAD_PORT1, LAUNCHPAD_LED1, GPIO_DIR_OUT);
        gpio_toggle_out(LAUNCHPAD_PORT1, LAUNCHPAD_LED1);
        timer_counter = 0;
    }
}

uint16_t hex2int(char *hex) {
    uint16_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

int16_t cmd_erase_flash(int32_t args, char **argv)
{
    int16_t error = 0;
    if(args == 1) {
        int16_t offset = hex2int(argv[1]);
        hw_flash_erase(offset);
        printf("erase [0x%04X] <-> [0x%04X]\r\n", offset, (offset + 128));
    } else {
        printf("Invalid arg count\r\n");
        error = -1;
    }
    return error;
}

int16_t cmd_read_flash(int32_t args, char **argv)
{
    int16_t error = 0;
    if(args == 1) {
        int16_t offset = hex2int(argv[1]);
        uint8_t data = 0;
        hw_flash_read(offset, &data, sizeof(data));
        printf("[0x%04X] -> 0x%02X\r\n", offset, data);
    } else {
        printf("Invalid arg count\r\n");
        error = -1;
    }
    return error;
}

int16_t cmd_dump_flash(int32_t args, char **argv)
{
    int16_t error = 0;
    if(args == 2) {
        int16_t offset = hex2int(argv[1]);
        int16_t bytes = atoi(argv[2]);
        for(int16_t i = 0; i < bytes; i++) {
            uint8_t data = 0;
            hw_flash_read(offset, &data, sizeof(data));
            printf("[0x%04X] -> 0x%04X\r\n", (offset + i), data);
        }
    } else {
        printf("Invalid arg count\r\n");
        error = -1;
    }
    return error;
}

int16_t cmd_write_flash(int32_t args, char **argv)
{
    int16_t error = 0;
    if(args == 2) {
        int16_t offset = hex2int(argv[1]);
        uint8_t data = hex2int(argv[2]);
        hw_flash_write(offset, &data, sizeof(data));
        // write_flash(offset, data);
        printf("[0x%04X] <- 0x%02X\r\n", offset, data);
    } else {
        printf("Invalid arg count\r\n");
        error = -1;
    }
    return error;
}

int16_t cmd_md(int32_t args, char **argv)
{
    int16_t error = 0;
    if(args == 1) {
        int16_t offset = hex2int(argv[1]);
        uint8_t data[16];
        for(int16_t i = 0; i < 8; i++) {
            printf("    %04X:", offset);
            hw_flash_read(offset, data, sizeof(data));
            for(size_t b = 0; b < sizeof(data); b++) {
                printf(" %02X", data[b]);
            }
            printf("\r\n");
            offset += sizeof(data);
        }
    } else {
        printf("Invalid arg count\r\n");
        error = -1;
    }
    return error;
}

#define CONSOLE_NUM_COMMANDS    5
static callback command_map[CONSOLE_NUM_COMMANDS] = {
    {"r32", &cmd_read_flash},
    {"d32", &cmd_dump_flash},
    {"w32", &cmd_write_flash},
    {"e32", &cmd_erase_flash},
    {"md", &cmd_md}
};

void handle_command()
{
    puts("Handle start\r");

    char *parsed[CONSOLE_CMD_ARGS_MAX];
    for(size_t i = 0; i < CONSOLE_CMD_ARGS_MAX; i++) {
        parsed[i] = NULL;
    }

    uint8_t arguments = 0;
    parsed[0] = console_buffer;
    bool done = false;
    for(size_t i = 0; (i < console_cursor) && !done; i++) {
        if(console_buffer[i] == CONSOLE_DELIMITER) {
            console_buffer[i] = '\0';
            arguments++;
            parsed[arguments] = &console_buffer[i + 1];
        } else if(console_buffer[i] == '\0') {
            // We are done parsing
            arguments++;
            done = true;
        }
    }

    for(uint8_t i = 0; i < CONSOLE_NUM_COMMANDS; i++) {
        if(strncmp(parsed[0], command_map[i].command, CONSOLE_BUFFER_SIZE) == 0) {
            command_map[i].call(arguments, parsed);
        }
    }
    // for(uint8_t i = 0; i < CONSOLE_CMD_ARGS_MAX; i++) {
    //     if(parsed[i] != NULL) {
    //         printf("%d: %s\r\n", i, parsed[i]);
    //     }
    // }

    puts("Handle end\r");

    memset(console_buffer, 0, sizeof(console_buffer));
    console_cursor = 0;
    command_ready = false;
}

int run(void)
{
    // Stop the watch dog timer
    WDTCTL = WDTPW + WDTHOLD;

    //Set MCLK = SMCLK = 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // Configure LaunchPad UART pins. I don't have a core library method for this.
    P1SEL |= LAUNCHPAD_UART_RX + LAUNCHPAD_UART_TX;
    P1SEL2 |= LAUNCHPAD_UART_RX + LAUNCHPAD_UART_TX;

    // Initialize UART hardware
    uart_initialize(UART_BAUD_9600);
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
    // UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt

    // Initialize a timer
    TACCR0 =  0;
    TACCTL0 = CCIE;                             // CCR0 interrupt enabled
    TACTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK/8, upmode

    // Configure LPM0 and the Global Interrupt Enable
    _BIS_SR(/*LPM0_bits +*/ GIE);

    puts("Welcome to the MSP430G2553 LaunchPad!\r");

    TACCR0 = 999;
    uint8_t counter = 0;
    uint16_t heartbeat = 4096;
    while(1) {
        if(command_ready) {
            // Always check for a command
            handle_command();
        }

        // Use loop as a wait
        for(uint16_t i = 0; i < heartbeat; i++);

        // Increment heartbeat counter
        counter++;
        counter &= 0x0F;
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
