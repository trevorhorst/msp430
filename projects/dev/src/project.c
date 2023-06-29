#include "core/uart.h"
#include "core/i2c.h"
// #include "core/gpio.h"
#include "core/ssd1306.h"
#include "core/sht3xdis.h"

#include "project/project.h"

#define LOOP_COUNTER    32000

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

#define EPSILON 2.718281828459

const char string[] = { "Hello World\r\n" };
unsigned int i; //Counter

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    P1OUT |= TXLED;
    UCA0TXBUF = string[i++]; // TX next character
    if (i == sizeof string - 1) // TX over?
       UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    P1OUT &= ~TXLED;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    char data[10];
    snprintf(data, 6, "0x%02X\r\n", UCA0RXBUF);
    puts(data);
    IFG2 &= ~UCA0RXIFG; // Clear RX flag

    // Do we need to disable the TX interrupt to avoid clashes?

    // if(UCA0RXBUF == 'a')
    // {
    //     i = 0;
    //     UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
    //     UCA0TXBUF = string[i++];
    // }

    // Toggle LED to indicate received character
    P1OUT |= RXLED;
    P1OUT &= ~RXLED;
}

/**
 * @brief Base 2 logarithm implementation
 * https://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c
 * @param val Input value
 * @return float log2(x) approximation
 */
float fast_log2(float val) {
    union { float val; int32_t x; } u = { val };
    register float log_2 = (float)(((u.x >> 23) & 255) - 128);
    u.x   &= ~(255l << 23);
    u.x   += 127l << 23;
    // log_2 += ((-0.3358287811f) * u.val + 2.0f) * u.val  -0.65871759316667f;
    log_2 += ((-0.34484843f) * u.val + 2.02466578f) * u.val  - 0.67487759f;
    return (log_2);
}

/**
 * @brief Base 10 logarithm implementation
 * @param val Input value
 * @return float log10(x) approximation
 */
float fast_log(float val)
{
   return (fast_log2 (val) * 0.69314718f);
}

/**
 * @brief Calculates dew point in degrees Celsius (Magnus-Tetons formula). Allows
 * calculations for values between -40C and 60C with an uncertainty of 0.35C.
 * @param temp Temperature in degrees celsius
 * @param rh Relative humidity (percentage)
 * @return float dew point approximation
 */
float calculate_dew_point(float temp, float rh)
{
    float a = 17.27;
    float b = 237.7;

    float alpha = ((a * temp) / (b + temp)) + fast_log((double)(rh / 100.0));
    return (b * alpha) / (a - alpha);
}

float calculate_frost_point(float temp, float dew_point)
{
    float dewpoint_k = 273.15 + dew_point;
    float temp_k = 273.15 + temp;
    float frostpoint_k = dewpoint_k - temp_k + 2671.02 / ((2954.61 / temp_k) + 2.193665 * fast_log(temp_k) - 13.3448);
    return frostpoint_k - 273.15;
}

/**
 * @brief Convers degrees celsius to degrees farenheit
 * @param temp Temperature in degrees celsius
 * @return float temperature in degrees farenheit
 */
float convert_celsius_to_farenheit(float temp)
{
    return ((9 * temp) / 5) + 32;
}

/**
 * @brief Converts degrees farenheit to degrees celsius
 * @param temp Temperature in degrees farenheit
 * @return float temperature in degrees celsius
 */
float convert_farenheit_to_celsius(float temp)
{
    return (5 * (temp - 32)) / 9;
}

typedef struct num_t {
    int16_t whole;
    int16_t frac;
} num;

num split_float(float value, int8_t precision)
{
    float multiplier = 1.0;
    while(precision > 0) {
        multiplier *= 10.0;
        precision--;
    }

    int16_t value_frac = (int16_t)((value - (int16_t)value) * multiplier);
    num n = {(int16_t)value, value_frac};
    return n;
}


int run(void)
{
    // Stop the watch dog timer
    WDTCTL = WDTPW + WDTHOLD;

    // // Enable pin 0 on port 1
    // // gpio_set_direction(0, 6, GPIO_DIR_OUT);
    // gpio_set_direction(0, 0, GPIO_DIR_OUT);

    // // Initial LED conditions
    // gpio_set_out(0, 0, GPIO_OUT_LOW);
    // // gpio_set_out(0, 6, GPIO_OUT_HIGH);

    // /* Configure P1.6 and P1.7 for I2C */
    // P1SEL  |= BIT6 + BIT7;
    // P1SEL2 |= BIT6 + BIT7;

    // Configure the pinmux
    P2DIR |= 0xFF; // All P2.x outputs
    P2OUT &= 0x00; // All P2.x reset
    P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
    P1DIR |= TXLED;
    P1OUT &= 0x00;
    /* Configure P1.6 and P1.7 for I2C */
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    DCOCTL = 0; // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ; // Set DCO
    DCOCTL = CALDCO_1MHZ;

    // Initialize the UART
    uart_initialize(UART_BAUD_9600);
    // UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
    // UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt

    // Initialize the I2C
    i2c_initialize(0);

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

    // Configure LPM0 and the Global Interrupt Enable
    _BIS_SR(/*LPM0_bits +*/ GIE);

    sht3xdis_i2c_device ht_sensor = {0x00, 0x44};

    char t[20];

    puts("Welcome to the MSP430!\r");

    // Loop infinitely
    while(1) {
        // // puts("Hello\r\n");
        sht3xdis_measurement measurement = {0x00, 0x00};
        measurement = sht3xdis_singleshot_measurement(&ht_sensor, SHT3XDIS_REPEATABILITY_HIGH, 0x1);

        float temp_c = sht3xdis_convert_raw_to_celsius(measurement.raw_temperature);
        // float temp_f = sht3xdis_convert_raw_to_farenheit(measurement.raw_temperature);
        float rh     = sht3xdis_convert_raw_to_relative_humidity(measurement.raw_relative_humidity);
        float dp     = calculate_dew_point(temp_c, rh);
        // float dp_f   = convert_celsius_to_farenheit(dp);

        // Print temperature data to console
        num n = split_float(temp_c, 2);
        snprintf(t, sizeof(t), " T: %d.%02dC\r", n.whole, n.frac);
        puts(t);

        // Print relative humnidity to console
        num r = split_float(rh, 2);
        snprintf(t, sizeof(t), "RH: %d.%02d%%\r", r.whole, r.frac);
        puts(t);

        // Print dew point to console
        num d = split_float(dp, 2);
        snprintf(t, sizeof(t), "DP: %d.%02dC\r\n", d.whole, d.frac);
        puts(t);

        puts("\r");

        // Sleep for ~1 second
        for(int i = 0; i < INT16_MAX; i++);
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
