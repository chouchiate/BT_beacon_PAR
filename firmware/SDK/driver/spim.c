#include "spim.h"
#include "gpio.h"

static SPI_CTRL_TYPE* _spi_master_ctrl = ((SPI_CTRL_TYPE*) SPI_MASTER_BASE);

void spim_set_mode(spi_master_mode_type mode)
{

    switch (mode)
    {

    case SPI_MASTER_MODE_0:
        _spi_master_ctrl->CPOL = 0;
        _spi_master_ctrl->CPHA = 0;
        break;

    case SPI_MASTER_MODE_1:
        _spi_master_ctrl->CPOL = 1;
        _spi_master_ctrl->CPHA = 0;
        break;

    case SPI_MASTER_MODE_2:
        _spi_master_ctrl->CPOL = 0;
        _spi_master_ctrl->CPHA = 1;
        break;

    case SPI_MASTER_MODE_3:
        _spi_master_ctrl->CPOL = 1;
        _spi_master_ctrl->CPHA = 1;
        break;

    }



}
void spim_set_clk_speed(spi_master_clk_div_type speed)
{
    _spi_master_ctrl->SPEED = speed;
}
void spim_set_msb_first_mode(uint8_t mode)
{
    _spi_master_ctrl->MSB = mode;
}


void spi_chip_select_enable(void)
{
    _spi_master_ctrl->PE = 1;    // CS Low
}

void spi_chip_select_disable(void)
{
    _spi_master_ctrl->PE = 0;    // CS High
}

void spi_master_write(uint8_t* p_data, uint16_t num)
{
    spi_chip_select_enable();

    for (; num > 0; num--)
    {
        _spi_master_ctrl->OUT = *p_data;
        _spi_master_ctrl->START = 1;
        p_data++;

        while (_spi_master_ctrl->START == 1);

        _spi_master_ctrl->DONE = 0;
    }

    spi_chip_select_disable();
}

void spi_master_read(uint8_t* p_data, uint16_t num)
{
    spi_chip_select_enable();

    for (; num > 0; num--)
    {
        _spi_master_ctrl->OUT = 0xFF;
        _spi_master_ctrl->START = 1;

        while (_spi_master_ctrl->START == 1);

        *p_data = _spi_master_ctrl->IN;
        p_data++;
        _spi_master_ctrl->DONE = 0;
    }

    _spi_master_ctrl->DONE = 0;
    spi_chip_select_disable();

}
