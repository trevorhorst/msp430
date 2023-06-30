#ifndef PROJECT_BLINK_H
#define PROJECT_BLINK_H

#include <msp430g2553.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SYMBOL_LENGTH   5

extern char font_txt[];
extern const unsigned int font_txt_size;

int run(void);

#endif // PROJECT_BLINK_H
