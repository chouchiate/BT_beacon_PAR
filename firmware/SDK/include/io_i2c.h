#ifndef _IO_I2C_H_
#define _IO_I2C_H_

#include "ARMCM0.h"
#include "config.h"

typedef enum {
	IO_I2C_2_BYTE_ADDRESS = 0x00,
	IO_I2C_1_BYTE_ADDRESS = 0x01,
    IO_I2C_0_BYTE_ADDRESS = 0x02,
}io_i2c_byte_address_t;

typedef enum {
	I2C_WRITE,
	I2C_READ,
}i2c_direction_t;

extern void io_i2c_config(uint32_t _iic_sclk, uint32_t _iic_sda);
extern uint8_t io_i2c_reg_read(uint8_t id, io_i2c_byte_address_t addr_len, uint16_t addr, uint8_t * buf, uint16_t sz, uint8_t stop_en);
extern uint8_t io_i2c_reg_write(uint8_t id, io_i2c_byte_address_t addr_len, uint16_t addr, uint8_t * buf, uint16_t sz);

#endif
