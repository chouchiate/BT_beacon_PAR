#include "i2c_master.h"
#include <string.h>

static I2C_CTRL_TYPE* _i2c_ctrl[I2C_MASTER_ID_NUM] =
{
    (I2C_CTRL_TYPE*) I2C_0_CTRL_BASE,
    (I2C_CTRL_TYPE*) I2C_1_CTRL_BASE,
};

static I2C_TXD_TYPE* _i2c_txd_ctrl[I2C_MASTER_ID_NUM] =
{
    (I2C_TXD_TYPE*) I2C_0_TXD_BASE,
    (I2C_TXD_TYPE*) I2C_1_TXD_BASE,
};

static I2C_RXD_TYPE* _i2c_rxd_ctrl[I2C_MASTER_ID_NUM] =
{
    (I2C_RXD_TYPE*) I2C_0_RXD_BASE,
    (I2C_RXD_TYPE*) I2C_1_RXD_BASE,
};

void i2c_master_set_clk_speed(i2c_module module, uint8_t speed)
{

    _i2c_ctrl[module]->SPEED = speed;
}
void i2c_master_set_address_mode(i2c_module i2c_moudle_id, i2c_master_address_mode_type mode)
{

    _i2c_ctrl[i2c_moudle_id]->ADDR_1 = mode;
}

void i2c_master_set_mode(i2c_module i2c_moudle_id, i2c_master_read_mode_type mode)
{

    if (mode == I2C_MASTER_CURRENT_ADDRESS_READ)

        _i2c_ctrl[i2c_moudle_id]->CURR_ADDR = 1;

    else

        _i2c_ctrl[i2c_moudle_id]->CURR_ADDR = 0;


}
uint8_t i2c_master_read(i2c_module i2c_moudle_id, uint8_t id, uint8_t* buf, uint16_t sz)
{

    uint8_t bret = 0;


    _i2c_ctrl[i2c_moudle_id]->ID = id;
    _i2c_ctrl[i2c_moudle_id]->DATA_CNT = (sz - 1);
    i2c_master_set_mode(i2c_moudle_id, I2C_MASTER_CURRENT_ADDRESS_READ);
    _i2c_ctrl[i2c_moudle_id]->RSTB = 1;
    _i2c_ctrl[i2c_moudle_id]->READ = 1;

    while (_i2c_ctrl[i2c_moudle_id]->DONE_FLG == 0);

    _i2c_ctrl[i2c_moudle_id]->DONE_FLG = 0;

    if (_i2c_ctrl[i2c_moudle_id]->ERR_FLG == 1)
    {
        _i2c_ctrl[i2c_moudle_id]->ERR_FLG = 0;
        bret =  0;
    }
    else
        memcpy(buf, (uint8_t*)_i2c_rxd_ctrl[i2c_moudle_id]->RX, sz);

    _i2c_ctrl[i2c_moudle_id]->RSTB = 0;      //reset
    _i2c_ctrl[i2c_moudle_id]->DONE_FLG = 0;
    _i2c_ctrl[i2c_moudle_id]->ERR_FLG = 0;
    _i2c_ctrl[i2c_moudle_id]->READ = 0;
    _i2c_ctrl[i2c_moudle_id]->RSTB = 0;


    return bret;


}
uint8_t i2c_master_write(i2c_module i2c_moudle_id, uint8_t id, uint8_t* buf, uint16_t sz)
{

    uint8_t bret = 1;

    _i2c_ctrl[i2c_moudle_id]->RSTB = 1;
    _i2c_ctrl[i2c_moudle_id]->ID = id;
    _i2c_ctrl[i2c_moudle_id]->ADDRESS = buf[0];
    _i2c_ctrl[i2c_moudle_id]->DATA_CNT = (sz - 1);
    i2c_master_set_mode(I2C_MODULE_0, I2C_MASTER_CURRENT_ADDRESS_READ);

    memcpy((uint8_t*)_i2c_txd_ctrl[i2c_moudle_id]->TX, &buf[1], sz - 1);

    _i2c_ctrl[i2c_moudle_id]->WRITE = 1;

    while (_i2c_ctrl[i2c_moudle_id]->DONE_FLG == 0);

    _i2c_ctrl[i2c_moudle_id]->DONE_FLG = 0;

    if (_i2c_ctrl[i2c_moudle_id]->ERR_FLG == 1)
    {
        _i2c_ctrl[i2c_moudle_id]->ERR_FLG = 0;
        bret = 0;
    }

    _i2c_ctrl[i2c_moudle_id]->RSTB = 0;          //reset

    return bret;

}
uint8_t i2c_master_combined_write_read(i2c_module i2c_moudle_id, uint8_t id, i2c_master_address_mode_type addr_len_mode, uint16_t addr, uint8_t* buf, uint16_t sz)
{

    uint8_t bret = 1;

    _i2c_ctrl[i2c_moudle_id]->RSTB = 1;
    _i2c_ctrl[i2c_moudle_id]->ID = id;
    _i2c_ctrl[i2c_moudle_id]->ADDR_1 = addr_len_mode;
    _i2c_ctrl[i2c_moudle_id]->ADDRESS = addr;
    _i2c_ctrl[i2c_moudle_id]->DATA_CNT = (sz - 1);
    i2c_master_set_address_mode(i2c_moudle_id, addr_len_mode);
    _i2c_ctrl[i2c_moudle_id]->READ = 1;

    while (_i2c_ctrl[i2c_moudle_id]->DONE_FLG == 0);

    _i2c_ctrl[i2c_moudle_id]->DONE_FLG = 0;

    if (_i2c_ctrl[i2c_moudle_id]->ERR_FLG == 1)
    {
        _i2c_ctrl[i2c_moudle_id]->ERR_FLG = 0;
        bret =  0;
    }
    else
        memcpy(buf, (uint8_t*)_i2c_rxd_ctrl[i2c_moudle_id]->RX, sz);

    _i2c_ctrl[i2c_moudle_id]->RSTB = 0;      //reset

    return bret;


}
uint8_t i2c_master_write_addr_data(i2c_module i2c_moudle_id, uint8_t id, i2c_master_address_mode_type addr_len_mode, uint16_t addr, uint8_t* buf, uint16_t sz)
{

    uint8_t bret = 1;

    _i2c_ctrl[i2c_moudle_id]->RSTB = 1;
    _i2c_ctrl[i2c_moudle_id]->ID = id;
    _i2c_ctrl[i2c_moudle_id]->ADDR_1 = addr_len_mode;
    _i2c_ctrl[i2c_moudle_id]->ADDRESS = addr;
    _i2c_ctrl[i2c_moudle_id]->DATA_CNT = (sz - 1);

    memcpy((uint8_t*)_i2c_txd_ctrl[i2c_moudle_id]->TX, &buf[0], sz);

    _i2c_ctrl[i2c_moudle_id]->WRITE = 1;

    while (_i2c_ctrl[i2c_moudle_id]->DONE_FLG == 0);

    _i2c_ctrl[i2c_moudle_id]->DONE_FLG = 0;

    if (_i2c_ctrl[i2c_moudle_id]->ERR_FLG == 1)
    {
        _i2c_ctrl[i2c_moudle_id]->ERR_FLG = 0;
        bret = 0;
    }

    _i2c_ctrl[i2c_moudle_id]->RSTB = 0;          //reset

    return bret;

}
