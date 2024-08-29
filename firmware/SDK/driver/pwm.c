#include "pwm.h"
#include "gpio.h"

static PWM_CTRL_TYPE*   _pwm_0_ctrl  = ((PWM_CTRL_TYPE*)    PWM_0_CTRL_BASE);
static PWM_CTRL_TYPE*   _pwm_1_ctrl  = ((PWM_CTRL_TYPE*)    PWM_1_CTRL_BASE);
static PWM_CTRL_TYPE*   _pwm_2_ctrl  = ((PWM_CTRL_TYPE*)    PWM_2_CTRL_BASE);


static PWM_CTRL_TYPE* _pwm_led_ctrl[PWM_LED_ID_NUM] =
{
    (PWM_CTRL_TYPE*) PWM_0_CTRL_BASE,
    (PWM_CTRL_TYPE*) PWM_1_CTRL_BASE,
    (PWM_CTRL_TYPE*) PWM_2_CTRL_BASE,
};

void pwm_enable(pwm_module module, pwm_parameter parameter)
{
    PWM_CTRL_TYPE*  pwm_ctrl = 0;

    switch (module)
    {
    case PWM_0:
        pwm_ctrl = ((PWM_CTRL_TYPE*)   PWM_0_CTRL_BASE);
        break;

    case PWM_1:
        pwm_ctrl = ((PWM_CTRL_TYPE*)   PWM_1_CTRL_BASE);
        break;

    default:
        pwm_ctrl = ((PWM_CTRL_TYPE*)   PWM_2_CTRL_BASE);
        break;
    }

    pwm_ctrl->MODE = parameter.MODE;

    switch (parameter.MODE)
    {
    case PWM_LED_PWM_MODE:
        pwm_ctrl->PWM_M = parameter.PWM_M;
        pwm_ctrl->PWM_N = parameter.PWM_N;
        break;

    case PWM_LED_FLASH_MODE:
        pwm_ctrl->T1 = parameter.T1;
        pwm_ctrl->T2 = parameter.T2;
        pwm_ctrl->T3 = parameter.T3;
        pwm_ctrl->N1 = parameter.N1;
        pwm_ctrl->N2 = parameter.N2;
        break;

    case PWM_LED_BREATH_MODE:
        pwm_ctrl->BR_TH_MAX = parameter.BR_TH_MAX;
        pwm_ctrl->BR_TH_MIN = parameter.BR_TH_MIN;
        pwm_ctrl->BR_SP = parameter.BR_SP;
        pwm_ctrl->T4 = parameter.T4;
        break;
    }

    pwm_ctrl->PAUS = 1;
    pwm_ctrl->LED_PE = 1;

}

void pwm_disable(pwm_module module)
{
    switch (module)
    {
    case PWM_0 :
        _pwm_0_ctrl->LED_PE = 0;
        break;

    case PWM_1 :
        _pwm_1_ctrl->LED_PE = 0;
        break;

    case PWM_2 :
        _pwm_2_ctrl->LED_PE = 0;
        break;
    }
}


void pwm_led_set_flash(pwm_module module, uint32_t duty_kept_length, uint32_t duty_cycle_length,
                       uint32_t duty_cycle_count, uint32_t duty_cycle_end_delay, uint32_t duty_cycles_count)
{
    _pwm_led_ctrl[module]->T1 = duty_kept_length;
    _pwm_led_ctrl[module]->T2 = duty_cycle_length;
    _pwm_led_ctrl[module]->N1 = duty_cycle_count;
    _pwm_led_ctrl[module]->T3 = duty_cycle_end_delay;
    _pwm_led_ctrl[module]->N2 = duty_cycles_count;
    _pwm_led_ctrl[module]->MODE = PWM_LED_FLASH_MODE;

}

void pwm_led_set_breath(pwm_module module, uint32_t min, uint32_t max, uint32_t period, uint32_t step)
{
    _pwm_led_ctrl[module]->BR_TH_MAX = max;
    _pwm_led_ctrl[module]->BR_TH_MIN = min;
    _pwm_led_ctrl[module]->T4 = period;
    _pwm_led_ctrl[module]->BR_SP = step;
    _pwm_led_ctrl[module]->MODE = PWM_LED_BREATH_MODE;
}

void pwm_led_set_always_on(pwm_module module)
{
    _pwm_led_ctrl[module]->MODE = PWM_LED_ALWAYS_ON_MODE;
}

void pwm_led_set_pwm(pwm_module module, uint32_t start_high_period, uint32_t total_period)
{
    _pwm_led_ctrl[module]->MODE  = PWM_LED_PWM_MODE;
    _pwm_led_ctrl[module]->PWM_N = start_high_period;
    _pwm_led_ctrl[module]->PWM_M = total_period;
}

void pwm_led_start(pwm_module module)
{
    _pwm_led_ctrl[module]->LED_PE = 1;
}

void pwm_led_stop(pwm_module module)
{
    _pwm_led_ctrl[module]->LED_PE = 0;
}

void pwm_led_set_polarity(pwm_module module, int high)
{
    uint8_t tmp = 0;

    tmp = _pwm_led_ctrl[module]->BR_TH_MIN;

    if(high == 1)
    {
        _pwm_led_ctrl[module]->BR_TH_MIN = (tmp | 0x80);
    }
    if(high == 0)
    {
        _pwm_led_ctrl[module]->BR_TH_MIN = (tmp & 0x7f);
    }

}
