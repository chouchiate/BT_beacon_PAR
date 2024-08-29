#include "gpadc.h"
#include "gpio.h"
#include "pxi_par2860_ble_lib.h"

static GPADC_CTRL_TYPE* _gpadc_ctrl     = ((GPADC_CTRL_TYPE*)  GPADC_CTRL_BASE);
static PIN_CONFIG_TYPE* _pin_config     = ((PIN_CONFIG_TYPE*)  PIN_CONFIG_BASE);     //¹Ü½Å¿ØÖÆ¼Ä´æÆ÷
static GPIO_CTRL_TYPE*  _gpio_ctrl      = ((GPIO_CTRL_TYPE*)   GPIO_CTRL_BASE);      //GPIO¿ØÖÆ¼Ä´æÆ÷
static GPI_CTRL_TYPE*   _gpi_ctrl       = ((GPI_CTRL_TYPE*)    GPI_CTRL_BASE);       //GPIOÊäÈë¿ØÖÆ¼Ä´æÆ÷
static gpadc_irq_cb_type _irq_cb;

void gpadc_init(gpadc_mode mode, gpadc_channel_type channel, uint8_t gpadc_int_en, gpadc_irq_cb_type cb)
{
    //-----------------pin config-------------------

    switch (channel)
    {

    case GPIO2_CHANNEL:

        _pin_config->PIN_2_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_2_DIR = 1;
        _gpi_ctrl->GPI_2_InE = 0;
        _pin_config->PAD_2_INPUT_PULL_UP = 0;      //
        _pin_config->PIN_2_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO3_CHANNEL:

        _pin_config->PIN_3_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_3_DIR = 1;
        _gpi_ctrl->GPI_3_InE = 0;
        _pin_config->PAD_3_INPUT_PULL_UP = 0;      //
        _pin_config->PIN_3_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO4_CHANNEL:
        //ADCGP_CH[2] Pin Init

        _pin_config->PIN_4_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_4_DIR = 1;
        _gpi_ctrl->GPI_4_InE = 0;
        _pin_config->PAD_4_INPUT_PULL_UP = 0;      //
        _pin_config->PIN_4_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO5_CHANNEL:
        //ADCGP_CH[3] Pin Init
        _pin_config->PIN_5_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_5_DIR = 1;
        _gpi_ctrl->GPI_5_InE = 0;
        _pin_config->PAD_5_INPUT_PULL_UP = 0;      //
        _pin_config->PIN_5_SEL = PIN_SEL_ANALOG;

    case GPIO28_CHANNEL:
        //ADCGP_CH[4] Pin Init
        _pin_config->PIN_28_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_28_DIR = 1;

        if(pxi_get_qfn_type() == QFN_48)
        {
            _gpi_ctrl->GPI_28_InE = 0;
        }
        else
        {
            _gpi_ctrl->GPI_16_InE = 0;
        }

        _pin_config->PAD_28_INPUT_PULL_UP = 0;     //
        _pin_config->PIN_28_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO29_CHANNEL:
        //ADCGP_CH[5] Pin Init
        _pin_config->PIN_29_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_29_DIR = 1;
        _gpi_ctrl->GPI_29_InE = 0;
        _pin_config->PAD_29_INPUT_PULL_UP = 0;     //
        _pin_config->PIN_29_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO30_CHANNEL:
        //ADCGP_CH[6] Pin Init
        _pin_config->PIN_30_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_30_DIR = 1;

        if(pxi_get_qfn_type() == QFN_48)
        {
            _gpi_ctrl->GPI_30_InE = 0;
        }
        else
        {
            _gpi_ctrl->GPI_20_InE = 0;
        }

        _pin_config->PAD_38_INPUT_PULL_UP = 0;     //
        _pin_config->PIN_30_SEL = PIN_SEL_ANALOG;
        break;

    case GPIO31_CHANNEL:
        //ADCGP_CH[7] Pin Init
        _pin_config->PIN_31_SEL = PIN_SEL_GPIO;
        _gpio_ctrl->GPIO_31_DIR = 1;
        _gpi_ctrl->GPI_31_InE = 0;
        _pin_config->PAD_39_INPUT_PULL_UP = 0;     //
        _pin_config->PIN_31_SEL = PIN_SEL_ANALOG;
        break;
    }

    _gpadc_ctrl->CLKRATE = 31;        // 64M/((31+1)*2)= 1M
    _gpadc_ctrl->DA_GPADC_EN = 1;

    _gpadc_ctrl->CHANNEL_SET_NUM = 1;
    _gpadc_ctrl->SCAN_COUNT = 1;      //
    gpadc_channel_sel(channel);
    _gpadc_ctrl->Continue_Scan = 0;

    //set time delay
    _gpadc_ctrl->START_SETTLE = 24;

    _gpadc_ctrl->CHANNEL_SETTLE = 4;


    if (mode == AVERAGE_MODE)
    {

        _gpadc_ctrl->AVERAGE = 7;
        _gpadc_ctrl->DATA_LENGTH = 7;
    }
    else
    {
        _gpadc_ctrl->AVERAGE = 1;
        _gpadc_ctrl->DATA_LENGTH = 0;
    }

    _gpadc_ctrl->EVENTS = 0;
    _gpadc_ctrl->INTENSET = 1;

    if (gpadc_int_en == 1)
    {
        NVIC_EnableIRQ(GPADC_IRQn);
        _irq_cb = cb;
    }
    else
    {
        NVIC_DisableIRQ(GPADC_IRQn);
        _irq_cb = 0;
    }
}

void gpadc_start(void)
{

    _gpadc_ctrl->DA_GPADC_EN = 1;
    _gpadc_ctrl->TASK_START = 1;
}

void gpadc_stop(void)
{
    if (_gpadc_ctrl->gpadc_fsm_cs)
    {
        _gpadc_ctrl->TASK_STOP = 1;
        //
        _gpadc_ctrl->INTENCLR = 1;        //disable INT
        _gpadc_ctrl->EVENTS = 0;          //clear INT flag

        while (_gpadc_ctrl->gpadc_fsm_cs);

        while (_gpadc_ctrl->EVENTS == 0);

        _gpadc_ctrl->EVENTS = 0;          //clr INT flag

        _gpadc_ctrl->INTENSET = 1;        //Enable INT
    }
}

uint16_t gpadc_get_value(void)
{
    uint16_t adc = 0;

    if (!NVIC_GetEnableIRQ(GPADC_IRQn))
    {
        while (1)
        {
            if (_gpadc_ctrl->EVENTS)
            {

                adc = _gpadc_ctrl->adc_data_hclk;
                _gpadc_ctrl->EVENTS = 0; //Clear interrupt event
                _gpadc_ctrl->DA_GPADC_EN = 0;
                break;

            }
        }
    }

    return adc;

}

void gpadc_channel_sel(uint8_t ch)
{
    if (ch <= 10)
    {
        _gpadc_ctrl->CHANNEL_SET0 = ch;
        _gpadc_ctrl->CHANNEL_SEL = ch;

    }
}

uint8_t gpadc_get_evt(void)
{
    return _gpadc_ctrl->EVENTS;
}

void gpadc_clr_evt(void)
{
    _gpadc_ctrl->EVENTS = 0;
}

void gpadc_set_callback(gpadc_irq_cb_type cb)
{
    _irq_cb = cb;
}

void GPADC_IRQHandler(void)
{

    if (_gpadc_ctrl->EVENTS)
    {

        uint16_t adc = _gpadc_ctrl->adc_data_hclk;
        _gpadc_ctrl->EVENTS = 0; //Clear interrupt event
        _gpadc_ctrl->DA_GPADC_EN = 0;

        if (_irq_cb != 0)
            _irq_cb(adc);

    }
}
