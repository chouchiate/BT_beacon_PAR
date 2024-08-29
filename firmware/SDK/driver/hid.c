#include "hid.h"
#include "spim.h"
#include <string.h>

static  MISC_CTRL_TYPE* _misc_ctrl = ((MISC_CTRL_TYPE*) MISC_BASE);
static void (*hid_callback)(void);

void hid_init(void)
{
    _misc_ctrl->BTN_INIT = 1;
}

void hid_stop(void)
{
    _misc_ctrl->BTN_INIT = 0;
}

void hid_get_button_raw_data(uint16_t* raw_btn_data)
{

    hid_button_report_type* report;

    _misc_ctrl->BTN_TRIG = 1;

    while (_misc_ctrl->BTN_TRIG == 1);

    if (raw_btn_data != NULL)
        *raw_btn_data = _misc_ctrl->BTNS_RPT;


    memcpy(report, (uint16_t*)&_misc_ctrl->BTNS_RPT, sizeof(*report));
}

void hid_spi_set_mode(hid_spi_mode_type mode)
{
    switch (mode)
    {

    case HID_SPI_MASTER_MODE_0:
        _misc_ctrl->R_CPOL = 0;
        _misc_ctrl->R_CPHA = 0;
        break;

    case HID_SPI_MASTER_MODE_1:
        _misc_ctrl->R_CPOL = 1;
        _misc_ctrl->R_CPHA = 0;
        break;

    case HID_SPI_MASTER_MODE_2:

        _misc_ctrl->R_CPOL = 0;
        _misc_ctrl->R_CPHA = 1;
        break;

    case HID_SPI_MASTER_MODE_3:
        _misc_ctrl->R_CPOL = 1;
        _misc_ctrl->R_CPHA = 1;
        break;

    }
}

void hid_spi_single_write(uint8_t addr, uint8_t data)
{
    uint8_t i = 0;

    _misc_ctrl->SS_RDY_CLR = 1;

    _misc_ctrl->SS_BUF[addr] = data;

    while (1)
    {
        for (i = 0; i < 10; i++)
            __asm volatile (" NOP");

        if (_misc_ctrl->SS_RDY == 1)
            break;
    }
}

void hid_spi_single_read(uint8_t addr, uint8_t* p_data)
{
    uint8_t num_tmp = _misc_ctrl->SS_BURST_NUM;
    uint8_t addr_tmp = _misc_ctrl->SS_ADDR[0];

    _misc_ctrl->SS_BURST_NUM = 0x01;

    _misc_ctrl->SS_ADDR[0] = addr;

    _misc_ctrl->SS_BURST_READ = 1;

    while (_misc_ctrl->SS_BURST_READ == 1);

    *p_data = _misc_ctrl->SS_DATA[0];

    _misc_ctrl->SS_BURST_NUM = num_tmp;

    _misc_ctrl->SS_ADDR[0] = addr_tmp;
}

void hid_spi_read_pixart( uint8_t* addr, uint8_t* data, uint8_t  size)
{
    uint8_t i = 0;


    if (size > 5)
        size = 6;

    _misc_ctrl->SS_BURST_NUM = size;

    for (i = 0; i < size; i++)
        _misc_ctrl->SS_ADDR[i] = addr[i];


    _misc_ctrl->SS_BURST_READ = 1;

    while (_misc_ctrl->SS_BURST_READ == 1);

    for (i = 0; i < size; i++)
        data[i] = _misc_ctrl->SS_DATA[i];

}

void hid_spi_set_write_msb_polarity(hid_spi_write_msb_pol_type msb)
{
    _misc_ctrl->R_WRITE_POL = msb;
}

void hid_set_debounce_threshold(uint8_t val)
{

    _misc_ctrl->R_STABLE_THL = val;

}

void hid_set_z_btn_div(uint8_t val)
{
    _misc_ctrl->R_ZBTN_DIV = val;
}

void hid_int_enable(void*   p_callback)
{
    hid_callback = ((void (*)(void))p_callback);

    _misc_ctrl->HID_TOP_INT_MSK = 0;

    NVIC_EnableIRQ(HID_IRQn);
}

void hid_int_disable(void)
{
    _misc_ctrl->HID_TOP_INT_MSK = 1;

    NVIC_DisableIRQ(HID_IRQn);
}

void HID_IRQHandler(void)
{
    hid_int_disable();

    if (hid_callback != 0)
        (*hid_callback)();
}
