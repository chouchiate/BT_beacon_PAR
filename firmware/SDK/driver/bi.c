#include "bi.h"
#include "delay.h"
#include <string.h>

static  BI_CTRL_TYPE*   _bi_ctrl = ((BI_CTRL_TYPE*) BI_CTRL_BASE);

void battery_detect_set_ch(uint8_t ch)
{
    _bi_ctrl->GPADC_CHSEL = ch;
}

void battery_detect_get_data(uint16_t* bi)
{
    _bi_ctrl->GPADC_EN = 1;

    delay_us(200);

    while (_bi_ctrl->GPADC_EN == 1);

    _bi_ctrl->GPADC_DONE = 0;

    *bi = _bi_ctrl->GPADC_DATA;
}
