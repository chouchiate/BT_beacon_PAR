
#include "delay.h"

void delay_ms(uint16_t n)
{
    uint16_t i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < 2700; j++);
}

void delay_us(uint16_t n)
{
    uint16_t i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < 2; j++);
}
