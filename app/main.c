#include <msp430.h>

#include "project/project.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer. This line of code is needed at the beginning of most MSP430 projects.
                                            // This line of code turns off the watchdog timer, which can reset the device after a certain period of time.
                                            // If you've a lot of initializations, this line prevents the system from falling into infinite loops.

    return run();
}
