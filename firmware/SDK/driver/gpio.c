
#include "gpio.h"
#include "pxi_par2860_ble_lib.h"


static PIN_CONFIG_TYPE*     _pin_config  = ((PIN_CONFIG_TYPE*)  PIN_CONFIG_BASE);     //¹Ü½Å¿ØÖÆ¼Ä´æÆ÷
static GPIO_CTRL_TYPE*      _gpio_ctrl   = ((GPIO_CTRL_TYPE*)   GPIO_CTRL_BASE);       //GPIO¿ØÖÆ¼Ä´æÆ÷
static GPO_CTRL_TYPE*       _gpo_ctrl    = ((GPO_CTRL_TYPE*)    GPO_CTRL_BASE);       //GPIOÊä³ö¿ØÖÆ¼Ä´æÆ÷
static GPI_CTRL_TYPE*       _gpi_ctrl    = ((GPI_CTRL_TYPE*)    GPI_CTRL_BASE);       //GPIOÊäÈë¿ØÖÆ¼Ä´æÆ÷
static GPIO_IRQ_CTRL_TYPE* _gpio_irq_ctrl = ((GPIO_IRQ_CTRL_TYPE*) GPIO_IRQ_CTRL_BASE);


static gpi_irq_cb_type _irq_cb_arr[GPIO_MAX_NUM + 1];
static uint8_t _PAD_MUX_MAP_GPIO_TABLE_QFN_48[] =
{
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    17,// Gpio 11 map PAD_DIG_17
    18,// Gpio 12 map PAD_DIG_18
    19,// Gpio 13 map PAD_DIG_19
    20,// Gpio 14 map PAD_DIG_20
    21,// Gpio 15 map PAD_DIG_21
    22,// Gpio 16 map PAD_DIG_22
    23,// Gpio 17 map PAD_DIG_23
    24,// Gpio 18 map PAD_DIG_24
    25,// Gpio 19 map PAD_DIG_25
    26,// Gpio 20 map PAD_DIG_26
    27,// Gpio 21 map PAD_DIG_27

    30,// Gpio 22 map PAD_DIG_30
    31,// Gpio 23 map PAD_DIG_31
    32,// Gpio 24 map PAD_DIG_32
    33,// Gpio 25 map PAD_DIG_33
    34,// Gpio 26 map PAD_DIG_34
    35,// Gpio 27 map PAD_DIG_35
    36,// Gpio 28 map PAD_DIG_36
    37,// Gpio 29 map PAD_DIG_37
    38,// Gpio 30 map PAD_DIG_38
    39,// Gpio 31 map PAD_DIG_39
};
static uint8_t _PAD_MUX_MAP_GPIO_TABLE_QFN_32[] =
{
    0,		// Gpio 0 	map PAD_DIG_0
    1,		// Gpio 1 	map PAD_DIG_1
    2,		// Gpio 2 	map PAD_DIG_2
    3,		// Gpio 3 	map PAD_DIG_3
    4,		// Gpio 4 	map PAD_DIG_4
    5,		// Gpio 5 	map PAD_DIG_5
    6,		// Gpio 6 	map PAD_DIG_6
    9,		// Gpio 7 	map PAD_DIG_9
    10,		// Gpio 8 	map PAD_DIG_10
    7,	 	// Gpio 9 	map PAD_DIG_7
    8,	 	// Gpio 10 	map PAD_DIG_8
    15,		// Gpio 11 	map PAD_DIG_15
    21,   // Gpio 12 	map PAD_DIG_21
    17,		// Gpio 13	map PAD_DIG_17
    22,		// Gpio 14 	map PAD_DIG_22
    23,		// Gpio 15 	map PAD_DIG_23
    24,		// Gpio 16 	map PAD_DIG_22
    18,		// Gpio 17 	map PAD_DIG_18
    25,		// Gpio 18 	map PAD_DIG_24
    20,		// Gpio 19 	map PAD_DIG_20
    26,		// Gpio 20 	map PAD_DIG_26
    27,		// Gpio 21 	map PAD_DIG_27

    30,		// Gpio 22 	map PAD_DIG_30
    31,		// Gpio 23 	map PAD_DIG_31
    32,		// Gpio 24 	map PAD_DIG_32
    33,		// Gpio 25 	map PAD_DIG_33
    34,		// Gpio 26 	map PAD_DIG_34
    35,		// Gpio 27 	map PAD_DIG_35
    36,		// Gpio 28 	map PAD_DIG_36
    37,		// Gpio 29 	map PAD_DIG_37
    38,		// Gpio 30 	map	PAD_DIG_38
    39,		// Gpio 31 	map PAD_DIG_39
};
static uint8_t _PAD_MUX_MAP_GPIO_IE_TABLE_QFN_32[] =
{
    0,		// Gpio 0 	map GPIO_IE_0
    1,		// Gpio 1 	map GPIO_IE_1
    2,		// Gpio 2 	map GPIO_IE_2
    3,		// Gpio 3 	map GPIO_IE_3
    4,		// Gpio 4 	map GPIO_IE_4
    5,		// Gpio 5 	map GPIO_IE_5
    6,		// Gpio 6 	map GPIO_IE_6
    7,		// Gpio 7 	map GPIO_IE_7
    8,		// Gpio 8 	map GPIO_IE_8
    9,	 	// Gpio 9 	map GPIO_IE_9
    10,	 	// Gpio 10 	map GPIO_IE_10
    0xff,		// Gpio 11 	map NA
    0xff,   	// Gpio 12 	map 1
    0xff,		// Gpio 13	map NA
    0xff,		// Gpio 14 	map 1
    13,		// Gpio 15 	map GPIO_IE_13
    17,		// Gpio 16 	map GPIO_IE_17
    0xff,		// Gpio 17 	map NA
    0xff,		// Gpio 18 	map 1
    0xff,		// Gpio 19 	map 1
    0xff,		// Gpio 20 	map 1
    0xff,		// Gpio 21 	map 1

    0xff,		// Gpio 22 	map 1
    0xff,		// Gpio 23 	map 1
    19,		// Gpio 24 	map GPIO_IE_19
    12,		// Gpio 25 	map GPIO_IE_12
    0xff,		// Gpio 26 	map NA
    0xff,		// Gpio 27 	map NA
    16,		// Gpio 28 	map GPIO_IE_16
    0xff,		// Gpio 29 	map NA
    20,		// Gpio 30 	map	GPIO_IE_20
    0xff,		// Gpio 31 	map NA
};

void pad_mux_write(uint32_t io, pad_mux_type value)
{
    _pin_config->PINSEL[io] = value;
}

void gpo_config(uint32_t io, gpo_value_type val)
{
    uint32_t config = 1 << io;

    _gpio_ctrl->GPIODIR &= ~config;


    if (val != GPO_LOW)
        _gpo_ctrl->GPO |= config;
    else
        _gpo_ctrl->GPO &= ~config;
}
void gpo_out_high(uint32_t io)
{
    uint32_t config = 1 << io;

    _gpo_ctrl->GPO |= config;
}

void gpo_out_low(uint32_t io)
{
    uint32_t config = 1 << io;

    _gpo_ctrl->GPO &= ~config;
}

void gpo_toggle(uint32_t io)
{
    uint32_t config = 1 << io;

    _gpo_ctrl->GPO ^= config;

}

uint8_t gpi_get_val(uint32_t io)
{
    uint32_t config = 1 << io;

    return ((_gpi_ctrl->GPI & config) != 0);
}

void gpi_enable_int(uint32_t io, gpi_int_trigger_type trigger, gpi_int_polarity_type pol, gpi_irq_cb_type cb)
{

    uint32_t  config = 1 << io ;

    gpi_clear_all_int();

    if (pol == POL_RISING_HIGH)
    {
        gpi_disable_input_invert(io); //pol è¨­ç½®??0
    }

    if (pol == POL_FALLING_LOW)
    {
        gpi_enable_input_invert(io); //pol è¨­ç½®??1
    }

    _gpio_irq_ctrl->GPIO_INT_MSK = _gpio_irq_ctrl->GPIO_INT_MSK & (~config);  // unmask gpio interrupt

    _gpio_irq_ctrl->GPIO_TOP_INT_MSK = 0;

    NVIC_EnableIRQ(GPIO_IRQn);

    _irq_cb_arr[io] = cb;

}
void gpi_disable_int(uint32_t io)
{
    uint32_t    config = 1 << io ;
    uint32_t    setting = 0;

    setting = (_gpio_irq_ctrl->GPIO_INT_MSK  | config) ;
    _gpio_irq_ctrl->GPIO_INT_MSK = setting;
    _irq_cb_arr[io] = 0;

}
void gpi_config_input_invert(uint32_t io, gpi_inverted_type invert_type)
{

    if (invert_type == GPI_NOT_INVERTED)
        gpi_disable_input_invert(io);

    if (invert_type == GPI_INVERTED)
        gpi_enable_input_invert(io);

}
void gpi_enable_input_invert(uint32_t io)
{

    uint32_t gpi_pol = 0x00;

    gpi_pol = (_gpi_ctrl->GPI_POL | (1 << io));

    _gpi_ctrl->GPI_POL = gpi_pol;

}
void gpi_disable_input_invert(uint32_t io)
{

    uint32_t gpi_pol = 0x00;

    gpi_pol = _gpi_ctrl->GPI_POL &= ~(1 << io);

    _gpi_ctrl->GPI_POL = gpi_pol;

}
void pad_input_configure(uint32_t io, int en)
{
    uint32_t config = 1 << io;


    if(pxi_get_qfn_type() == QFN_32)
    {
        if(_PAD_MUX_MAP_GPIO_IE_TABLE_QFN_32[io] != 0xff)
        {
            config = 1 << _PAD_MUX_MAP_GPIO_IE_TABLE_QFN_32[io];
        }

    }

    if (en)
        _gpi_ctrl->GPI_InE |= config;
    else
        _gpi_ctrl->GPI_InE &= ~config;

}

void gpi_config(uint32_t io, gpi_pull_type pull, gpi_inverted_type invert_type)
{

    uint32_t    config = 1 << io;
    uint8_t     pad_dig = 0;

    _gpio_ctrl->GPIODIR |= config;

    gpi_config_input_invert(io, invert_type);

    if(pxi_get_qfn_type() == QFN_48)
    {
        pad_dig = _PAD_MUX_MAP_GPIO_TABLE_QFN_48[io];
    }
    else
    {
        pad_dig = _PAD_MUX_MAP_GPIO_TABLE_QFN_32[io];
    }

    if(pad_dig != 0xff)
        pad_pull_config(pad_dig,pull);

}

void pad_pull_config(uint8_t pad, gpi_pull_type pull)
{
    uint32_t    pull_up_config = 0;
    uint8_t     bank = 0;
    uint8_t     position = 0;

    bank = (uint8_t)(pad / 8);
    position = (uint8_t)(pad % 8);
    pull_up_config = 1 << position;

    if (pull == GPI_NO_PULL)
        _pin_config->PAD_INPUT_PULL_UP[bank] &= ~pull_up_config;

    if (pull == GPI_PULL_UP)
        _pin_config->PAD_INPUT_PULL_UP[bank] |= pull_up_config;
}
uint32_t gpi_get_val_32bit(void)
{
    return _gpi_ctrl->GPI;
}

void gpi_disable_all_int(void)
{

    NVIC_DisableIRQ(GPIO_IRQn);

    _gpio_irq_ctrl->GPIO_INT_CLR = _gpio_irq_ctrl->GPIO_INT;

    _gpio_irq_ctrl->GPIO_INT_MSK = 0xffffffff;

    _gpio_irq_ctrl->GPIO_TOP_INT_MSK = 1;

}

void gpi_clear_all_int(void)
{

    _gpio_irq_ctrl->GPIO_INT_CLR = _gpio_irq_ctrl->GPIO_INT;

}
gpi_int_polarity_type gpi_get_int_pol_type(uint8_t io)
{
    uint32_t ret = 0;
    gpi_int_polarity_type polarity_type = POL_FALLING_LOW;

    ret = (_gpi_ctrl->GPI_POL & (1 << io));

    if (ret > 0)
    {
        polarity_type = POL_FALLING_LOW; //pol è¨­ç½®??1
    }
    else
    {
        polarity_type = POL_RISING_HIGH;//pol è¨­ç½®??0

    }

    return polarity_type;
}
gpi_inverted_type gpi_get_input_invert_type(uint8_t io)
{
    uint32_t ret = 0;

    gpi_inverted_type type_ret = GPI_NOT_INVERTED;

    ret = (_gpi_ctrl->GPI_POL & (1 << io));

    if (ret > 0)
        type_ret = GPI_INVERTED;

    return type_ret;

}
uint8_t gpi_get_physical_val(uint8_t io)
{
    uint8_t value = 0x00;

    value = gpi_get_val(io);

    if (gpi_get_int_pol_type(io) == GPI_INVERTED)
    {
        if (value == 1)
            value = 0;
        else
            value = 1;
    }

    return value;
}
void GPIO_IRQHandler(void)
{
    uint32_t    idx = 0;
    uint32_t    io = 0;
    uint32_t    io_config = 0;

    io_config = (_gpio_irq_ctrl->GPIO_INT & (~_gpio_irq_ctrl->GPIO_INT_MSK));

    if (io_config >= 1)
    {

        _gpio_irq_ctrl->GPIO_INT_CLR = _gpio_irq_ctrl->GPIO_INT;

        for (idx = 0 ; idx <= GPIO_MAX_NUM ; idx++)
        {
            if ( (U32BIT(idx) & io_config) == U32BIT(idx))
            {
                io = idx;
                if (_irq_cb_arr[io] != 0)
                    _irq_cb_arr[io]();

            }
        }

    }
}
