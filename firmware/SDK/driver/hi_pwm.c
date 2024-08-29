#include "hi_pwm.h"

static HI_PWM_CTRL_TYPE*        _hpwm_ctrl = ((HI_PWM_CTRL_TYPE*) HPWM_CTRL_BASE);
static PIN_CONFIG_TYPE*     _pin_config      = ((PIN_CONFIG_TYPE*)  PIN_CONFIG_BASE);     //???????
static hi_pwm_irq_cb_type irq_cb;

void high_speed_pwm_init(hi_pwm_parameter* parameter)
{
    high_speed_pwm_stop();

    switch (parameter->channel)
    {
    case HI_PWM_CHANNEL_0:
        break;

    case HI_PWM_CHANNEL_1:
        break;

    case HI_PWM_CHANNEL_2:
        break;

    case HI_PWM_CHANNEL_3:
        break;

    default:
        break;
    }

    /*mcu clock*/
    high_speed_pwm_set_mode(parameter->mode);
    high_speed_pwm_set_polarity(parameter->channel, parameter->polarity);
    high_speed_pwm_set_prescaler(parameter->prescaler);

    high_speed_pwm_set_period(parameter->period);
    high_speed_pwm_set_duty(parameter->channel, parameter->duty);


    _hpwm_ctrl->EVENTS = HI_PWM_INT_ALL;     //clr INT

}

void high_speed_pwm_set_prescaler(hi_pwm_perscaler_type type)
{
    _hpwm_ctrl->PRESCALER = type;
}

void high_speed_pwm_set_polarity(hi_pwm_channel channel, hi_pwm_polarity_type polarity)
{

    switch (channel)
    {
    case HI_PWM_CHANNEL_0:
        if (polarity == HI_PWM_POL_LOW_FIRST)
            _hpwm_ctrl->POLARITY &= 0x0e;
        else
            _hpwm_ctrl->POLARITY |= 0x01;

        break;

    case HI_PWM_CHANNEL_1:
        if (polarity == HI_PWM_POL_LOW_FIRST)
            _hpwm_ctrl->POLARITY &= 0x0d;
        else
            _hpwm_ctrl->POLARITY |= 0x02;

        break;

    case HI_PWM_CHANNEL_2:
        if (polarity == HI_PWM_POL_LOW_FIRST)
            _hpwm_ctrl->POLARITY &= 0x0b;
        else
            _hpwm_ctrl->POLARITY |= 0x04;

        break;

    case HI_PWM_CHANNEL_3:
        if (polarity == HI_PWM_POL_LOW_FIRST)
            _hpwm_ctrl->POLARITY &= 0x07;
        else
            _hpwm_ctrl->POLARITY |= 0x08;

        break;

    default:
        break;
    }

}

void high_speed_pwm_set_duty(hi_pwm_channel  channel, uint16_t duty)
{
    if (channel == HI_PWM_CHANNEL_0)
        _hpwm_ctrl->CMP_CH0 = duty;

    else if (channel == HI_PWM_CHANNEL_1)
        _hpwm_ctrl->CMP_CH1 = duty;

    else if (channel == HI_PWM_CHANNEL_2)
        _hpwm_ctrl->CMP_CH2 = duty;

    else if (channel == HI_PWM_CHANNEL_3)
        _hpwm_ctrl->CMP_CH3 = duty;

}

void high_speed_pwm_set_period(uint16_t period)
{
    _hpwm_ctrl->COUNTER_TOP = period;
}

void high_speed_pwm_set_mode(hi_pwm_mode mode)
{
    _hpwm_ctrl->MODE = mode;
}

void high_speed_pwm_start(void)
{
    _hpwm_ctrl->START = 1;

}

void high_speed_pwm_stop(void)
{
    _hpwm_ctrl->STOP = 1;
}
void high_speed_pwm_enable_interrupt(hi_pwm_irq_cb_type cb)
{
    _hpwm_ctrl->INTEN  = HI_PWM_INT_ALL;
    irq_cb = cb;
    NVIC_EnableIRQ(HPWM_IRQn);

}
void high_speed_pwm_disable_interrupt(void)
{
    NVIC_DisableIRQ(HPWM_IRQn);
}

void HPWM_IRQHandler(void)
{
    uint8_t st = _hpwm_ctrl->EVENTS;
    _hpwm_ctrl->EVENTS = HI_PWM_INT_ALL;     //clr all INT

    if (irq_cb != 0)
        irq_cb(st);
}
