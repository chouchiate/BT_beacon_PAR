/// @file spim.h
/// @brief PAR2860 SPI Master driver
///
#ifndef _SPIM_H_
#define _SPIM_H_

#include "ARMCM0.h"

/// @defgroup SPIRoutines SPI master
/// @{
///     @brief driver document
///
///     Register
///
///
///     @ref SPI_MASTER_CTRL_TYPE
///
///
/// @brief SPI master clock divisor setting
///
/// Base clock is RC clock
typedef enum
{
    SPI_MASTER_CLK_DIV_128  = 6,
    SPI_MASTER_CLK_DIV_64   = 5,
    SPI_MASTER_CLK_DIV_32   = 4,
    SPI_MASTER_CLK_DIV_16   = 3,
    SPI_MASTER_CLK_DIV_8    = 2,
    SPI_MASTER_CLK_DIV_4    = 1,
    SPI_MASTER_CLK_DIV_2    = 0,
} spi_master_clk_div_type;

/// @brief SPI master mode
///
/// CPOL(bit0) - clock idle state
/// CPHA(bit1) - first or section clock edge to latch data
typedef enum
{
    SPI_MASTER_MODE_0   = 0, ///< Idle low,  first  edge latch  CPOL=0 CPHA=0
    SPI_MASTER_MODE_1   = 1, ///< Idle high, first  edge latch  CPOL=1 CPHA=0
    SPI_MASTER_MODE_2   = 2, ///< Idle low,  second edge latch  CPOL=0 CPHA=1
    SPI_MASTER_MODE_3   = 3, ///< Idle high, second edge latch  CPOL=1 CPHA=1

} spi_master_mode_type;


/// @brief Set SPI master's setting of CPOL and CPHA
/// @param mode @ref SPI_MASTER_MODE_TYPE
void spim_set_mode(spi_master_mode_type mode);
/// @brief Set SPI clock speed
/// @param speed Set output clock divisor<BR>@sa PCLK_get<BR>@sa SPI_MASTER_CLK_DIV_TYPE
void spim_set_clk_speed(spi_master_clk_div_type speed);
/// @brief Set SPI msb first
/// @param mode 1.msb first
/// @warning  Please decided the MSB first or LSB first when you start SPI read or write
/// @ref spi_master_write
/// @ref spi_master_read
void spim_set_msb_first_mode(uint8_t mode);
/// @brief Enable chip select pin
void spi_chip_select_enable(void);
/// @brief Disable chip select pin
void spi_chip_select_disable(void);
/// @brief SPI write
/// @param p_data write data buffer pointer
/// @param size  size of write data buffer
/// @warning  Please note the MSB first or LSB first ,when you use this API .It dependent on your peripheral  module
/// @ref spim_set_msb_first_mode
void spi_master_write(uint8_t* p_data, uint16_t size);
/// @brief SPI write
/// @param p_data read data buffer pointer
/// @param size  size of read data buffer
/// @warning  Please note the MSB first or LSB first ,when you use this API .It dependent on your peripheral  module
/// @ref spim_set_msb_first_mode
void spi_master_read(uint8_t* p_data, uint16_t size);
///@}
#endif
