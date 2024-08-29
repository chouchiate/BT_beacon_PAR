#include "rtc.h"
#include "pxi_par2860_ble_lib.h"


static uint32_t _int_status = 0;

static RTC_CTRL_TYPE* _rtc_ctrl = ((RTC_CTRL_TYPE*)    RTC_CTRL_BASE);
static rtc_irq_callback_type _irq_cb;

void RTC_IRQHandler(void)
{
    rtc_int_type status = (rtc_int_type)_rtc_ctrl->EVENTS;

    if (_irq_cb)
        _irq_cb(status);

    rtc_int_clear(status);
}

void rtc_set_interrupt_callback(rtc_irq_callback_type cb)
{
    _irq_cb = cb;
}

void rtc_int_clear(rtc_int_type type)
{
    _rtc_ctrl->EVENTS_CLR = type;
}

void rtc_int_enable(rtc_int_type type)
{
    _rtc_ctrl->EVENTS_EN |= type;
    _int_status |= type;
}

void rtc_int_disable(rtc_int_type type)
{
    _rtc_ctrl->EVENTS_EN &= ~type;
}

void rtc_start(void)
{
    if (_rtc_ctrl->START)
        return;

    _rtc_ctrl->START = 1;

    while (_rtc_ctrl->START);
}

void rtc_stop(void)
{
    /* This reset RTC counter for prescaler*/
    if (!_rtc_ctrl->STOP)
        return;

    _rtc_ctrl->STOP = 1;

    while (_rtc_ctrl->STOP);
}

void rtc_clear(void)
{
    /* This clear COUNTER & CALENDAR */
    _rtc_ctrl->CLEAR = 1;

    while (_rtc_ctrl->CLEAR);
}

uint32_t rtc_set_prescaler(uint32_t tick, uint8_t adjust_calendar)
{
    uint32_t prescaler;

    if (tick > 0x8000)
        return 0;
    else if (tick == 0)
        prescaler = 0;
    else
        prescaler = (tick - 1);

    _rtc_ctrl->PRESCALER = prescaler;

    if (adjust_calendar)
    {

        uint32_t i;

        for (i = 0; i < 32; i++)
        {
            if (prescaler == 0)
                break;

            prescaler >>= 1;
        }

        _rtc_ctrl->SECONDS_BIT = 15 - i;
        prescaler = (1 << i) - 1;

    }
    else
        _rtc_ctrl->SECONDS_BIT = 0;


    return prescaler + 1;
}

void rtc_set_seconds_bit(uint32_t order)
{
    _rtc_ctrl->SECONDS_BIT = order;
}

void rtc_get_compare(int id, rtc_time_type* time)
{
    __IO uint32_t* compare = id ? &_rtc_ctrl->COMPARE1 : &_rtc_ctrl->COMPARE0;
    int i;

    time->u32 = *compare;

    // time format to decimal
    for (i = 0 ; i < 3; i++)
    {
        uint8_t tmp = time->u8[i];
        time->u8[i] = (tmp >> 4) * 10;
        time->u8[i] += (tmp & 0xF);
    }
}

void rtc_get_calendar(rtc_time_type* time)
{
    int i;

    time->u32 = _rtc_ctrl->CALENDAR;

    // time format to decimal
    for (i = 0 ; i < 3; i++)
    {
        uint8_t tmp = time->u8[i];
        time->u8[i] = (tmp >> 4) * 10;
        time->u8[i] += (tmp & 0xF);
    }
}

static void rtc_decimal_to_time(rtc_time_type* time)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        uint8_t tmp = time->u8[i];
        time->u8[i] = (tmp / 10) << 4;
        time->u8[i] |= tmp % 10;
    }

}

uint8_t rtc_running(void)
{
    return _rtc_ctrl->START;
}

rtc_int_type rtc_int_enabled_status(void)
{
    return (rtc_int_type)_int_status;
}

void rtc_set_compare(int id, rtc_time_type* time)
{
    __IO uint32_t* compare = id ? &_rtc_ctrl->COMPARE1 : &_rtc_ctrl->COMPARE0;
    int i;

    // Decimal to time format
    for (i = 0; i < 3; i++)
    {
        uint8_t tmp = time->u8[i];
        time->u8[i] = (tmp / 10) << 4;
        time->u8[i] |= tmp % 10;
    }

    *compare = time->u32;

    pxi_delay_us(100);      //64M - delay 38us

    _rtc_ctrl->LOAD = 1;     //load alarm
}

void rtc_set_calendar(rtc_time_type* time)
{
    //rtc_set(time, NULL, NULL);
    int i;
    rtc_stop();

    // Decimal to time format
    for (i = 0; i < 3; i++)
    {
        uint8_t tmp = time->u8[i];
        time->u8[i] = (tmp / 10) << 4;
        time->u8[i] |= tmp % 10;
    }

    _rtc_ctrl->CALENDAR_INI = time->u32;

    rtc_start();

}
