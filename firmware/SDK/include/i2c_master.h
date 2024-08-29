/// @file i2c_master.h
/// @brief PAR2802 I2C Master driver
///

#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#include "ARMCM0.h"
#include "stdbool.h"
#include "debug.h"

/// @defgroup I2CMRoutines I2C Master
/// @{
///     @brief driver document
///
///     Register
///
///     @ref I2C_MASTER_CTRL_TYPE
#define    I2C_BUFFER_MAX_SIZE         31 ///<  I2c max buferr size
#define    I2C_MASTER_ID_NUM           2  ///<  Number of I2C master modules


/// @brief I2C master modules 0~1
typedef enum
{
    I2C_MODULE_0 = 0, ///< I2C master modules 0
    I2C_MODULE_1 = 1, ///< I2C master modules 1
} i2c_module;
/// @brief I2C read mode
typedef enum
{
    I2C_MASTER_ADDRESS_READ,            ///< Send write address before read
    I2C_MASTER_CURRENT_ADDRESS_READ,    ///< Read without write address
} i2c_master_read_mode_type;

/// @brief I2C address length configuration
typedef enum
{
    I2C_MASTER_2BYTE_ADDRESS,           ///< 2Byte slave address
    I2C_MASTER_1BYTE_ADDRESS,           ///< 1Byte slave address
} i2c_master_address_mode_type;

/// @brief Set I2C master output clock rate
/// @param module_id Module id selection @ref I2C_MODULE
/// @param speed clock
/// @exampke if mcu clock 64m  the i2c speed is 79,then the i2c clock is 400kHz=(64m/(2(1+79)))
void i2c_master_set_clk_speed(i2c_module module, uint8_t speed);
/// @brief Set I2C master current r/w or combind mode
/// @param module_id Module id selection @ref I2C_MODULE
/// @param mode @ref i2c_master_read_mode_type
void i2c_master_set_mode(i2c_module i2c_moudle_id, i2c_master_read_mode_type mode);
/// @brief Set I2C master address mode  1 byte or 2 byte address
/// @param module_id Module id selection @ref I2C_MODULE
/// @param mode @ref i2c_master_address_mode_type
void i2c_master_set_address_mode(i2c_module i2c_moudle_id, i2c_master_address_mode_type mode);
/// @brief I2C master module current read
/// @param module_id Module id selection @ref I2C_MODULE
/// @param slave_id Target slave identity 7bit
/// @param buf RX buffer
/// @param sz Length of RX buffer<BR>@sa I2C_BUFFER_MAX_SIZE
/// @return 0 - Error<BR>1 - Success
uint8_t i2c_master_read(i2c_module i2c_moudle_id, uint8_t id, uint8_t* buf, uint16_t sz);
/// @brief I2C master module currnet write
/// @param module_id Module id selection @ref I2C_MODULE
/// @param slave_id Target slave identity 7bit
/// @param buf TX buffer <BR>@sa I2C_BUFFER_MAX_SIZE
/// @param sz Length of TX<BR>
/// @return 0 - Error<BR>1 - Success
uint8_t i2c_master_write(i2c_module i2c_moudle_id, uint8_t id, uint8_t* buf, uint16_t sz);
/// @brief Trigger I2C conbined write read mode, it can use to read or write EEPROM
/// @param module_id Module id selection @ref I2C_MODULE
/// @param addr_len_mode two byte or one byte address
/// @param addr address for write
/// @param buf RX buffer address
/// @param sz Length of RX buffer
/// @exmaple if the EEPROM address is 0xfffe 2byte.i2c module using 0,slave ID is 0x0e. size is
///  i2c_master_combined_write_read(0,0x0e, I2C_MASTER_2BYTE_ADDRESS,0xfffe, buf, 10)
/// @return 0 - Error<BR>1 - Success
uint8_t i2c_master_combined_write_read(i2c_module i2c_moudle_id, uint8_t id, i2c_master_address_mode_type addr_len_mode, uint16_t addr, uint8_t* buf, uint16_t sz);
/// @brief I2C master module currnet write addr and data
/// @param module_id Module id selection @ref I2C_MODULE
/// @param slave_id Target slave identity 7bit
/// @param addr address
/// @param buf TX buffer <BR>@sa I2C_BUFFER_MAX_SIZE
/// @param sz Length of TX<BR>
/// @return 0 - Error<BR>1 - Success

uint8_t i2c_master_write_addr_data(i2c_module i2c_moudle_id, uint8_t id, i2c_master_address_mode_type addr_len_mode, uint16_t addr, uint8_t* buf, uint16_t sz);
/// @}
#endif
