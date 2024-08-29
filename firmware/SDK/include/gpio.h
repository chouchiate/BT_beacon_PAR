/// @file  gpio.h
/// @brief PAR2860 GPIO driver
#ifndef _GPIO__H__
#define _GPIO__H__

#include "ARMCM0.h"



/// @defgroup GPIORoutines GPIO
/// @{
///     @brief driver document
///
typedef enum
{
    GPIO_MAX_NUM    = 31,   ///< Number of available gpio
} gpio_bank_type;

/// @brief Pull type<BR>
/// @ref PAD_CTRL_TYPE.PAD_PUP <BR>
/// @ref PAD_CTRL_TYPE.PAD3_PUP
typedef enum
{
    GPI_PULL_UP   = 0,
    GPI_NO_PULL   = 1,
} gpi_pull_type;

/// @brief Trigger type <BR>
/// @ref GPIO_CTRL_TYPE.GPIOINT_TYPE
typedef enum
{
    LEVEL_TRIGGER   = 0,
    EDGE_TRIGGER    = 1,
} gpi_int_trigger_type;

/// @brief Gpio output type <BR>
/// @ref GPIO_CTRL_TYPE.GPIO_DIR_SET <BR>
/// @ref GPIO_CTRL_TYPE.GPIO_OUT
typedef enum
{
    GPO_LOW         = 0,
    GPO_HIGH        = 1,
} gpo_value_type;

/// @brief Gpi interrupt type <BR>
/// @ref GPI_INT_TRIGGER_TYPE <BR>
/// @ref GPIO_CTRL_TYPE.GPIOINT_POL
typedef enum
{
    POL_RISING_HIGH = 0,    ///< Rising edge or<BR>High level
    POL_FALLING_LOW = 1,    ///< Falling edge or<BR>Low level
    POL_KEEP_LAST_SETTING = 2       ///< Keep last setting
} gpi_int_polarity_type;

/// @brief Gpi invert type <BR>
/// @ref GPI_INT_TRIGGER_TYPE <BR>
/// @ref GPIO_CTRL_TYPE.GPIOINT_POL
typedef enum
{
    GPI_INVERTED = 1,               ///< Invert gpi input value
    GPI_NOT_INVERTED = 0,           ///< Do not invert gpi input value
    GPI_KEEP_LAST_SETTING = 2       ///< Keep last setting
} gpi_inverted_type;
/// @brief Pad mux type vlaue <BR>
typedef enum pin_function_define
{

    PIN_SEL_GPIO,               ///< gpio
    PIN_SEL_UART_CTS0,  ///< uart 0 cts
    PIN_SEL_UART_RTS0,  ///< uart 0 rts
    PIN_SEL_UART_RXD0,  ///< uart 0 tx
    PIN_SEL_UART_TXD0,  ///< uart 0 rx
    PIN_SEL_UART_RXD1,  ///< uart 1 rx
    PIN_SEL_UART_TXD1,  ///< uart 1 tx
    PIN_SEL_SPIM_CSN,       ///< spi master chip select
    PIN_SEL_SPIM_CLK,       ///< spi master clocke
    PIN_SEL_SPIM_MOSI,  ///< spi master  MOSI
    PIN_SEL_SPIM_MISO,  ///< spi master  MISO
    PIN_SEL_I2C_SCL0,       ///< i2c 0 master serial clock
    PIN_SEL_I2C_SDA0,       ///< i2c 0 master serial data
    PIN_SEL_I2C_SCL1,       ///< i2c 1 master serial clock
    PIN_SEL_I2C_SDA1,       ///< i2c 1 master serial data
    PIN_SEL_PWM_0,          ///< pwm 0
    PIN_SEL_PWM_1,          ///< pwm 1
    PIN_SEL_PWM_2,          ///< pwm 2
    PIN_SEL_IR_OUT,         ///< ir tx
    PIN_SEL_SC_IO,          ///< sdcard io
    PIN_SEL_SC_DETECT,  ///< sdcard detect
    PIN_SEL_SC_RSTN,        ///< sdcard rstn
    PIN_SEL_SC_CLK,         ///< sdcard clock
    PIN_SEL_HI_PWM_0,       ///< high speed pwm 0
    PIN_SEL_HI_PWM_1,       ///< high speed pwm 1
    PIN_SEL_HI_PWM_2,       ///< high speed pwm 2
    PIN_SEL_HI_PWM_3,       ///< high speed pwm 3
    PIN_SEL_HID_SENSOR, ///< hid sensor
    PIN_SEL_ANALOG,         ///< analog pin
    PIN_SEL_SC_VCC


} pad_mux_type;
/// @brief GPI interrupt callback type
typedef void (*gpi_irq_cb_type)(void);
/// @brief Set Pad mux for GPIO
/// @param io Pin number select<BR>Valid range 0 ~ 31
void pad_mux_write(uint32_t io, pad_mux_type value);
/// @brief Set IO as GPO and configure output value
/// @param io Pin number select<BR>Valid range 0 ~ 31<BR>@ref GPIO_BANK_TYPE.GPIO_MAX_NUM
/// @param val Pin output value<BR>@ref GPO_VALUE_TYPE
void gpo_config(uint32_t io, gpo_value_type val);
/// @brief Set IO as GPO output high
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @param val Pin output value<BR>@ref GPO_VALUE_TYPE
void gpo_out_high(uint32_t io);
/// @brief Set IO as GPO output low
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @param val Pin output value<BR>@ref GPO_VALUE_TYPE
void gpo_out_low(uint32_t io);
/// @brief Toggle GPO value
/// @param io Pin number select<BR>Valid range 0 ~ 31
void gpo_toggle(uint32_t io);
/// @brief config GPI
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @param invert_type invert setting @ref GPI_INVERTED_TYPE
void gpi_config(uint32_t io, gpi_pull_type pull, gpi_inverted_type invert_type);
/// @brief Read GPI value of specific pin
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @return GPI value
uint8_t gpi_get_val(uint32_t io);
/// @brief Read GPI value of all pin IO  range 0 ~ 31 total 32bit
/// @return GPI value
/// @warning the gpi inpult value is decided by GPI_INVERTED_TYPE, if type is GPI_INVERTED, the gpi input value is opposite to physical.
uint32_t gpi_get_val_32bit(void);
/// @brief Enable GPI value invert
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @param invert_type invert setting @ref GPI_INVERTED_TYPE
void gpi_config_input_invert(uint32_t io, gpi_inverted_type invert_type);
/// @brief Enable GPI value invert
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @return GPI value
void gpi_enable_input_invert(uint32_t io);
/// @brief disable GPI value invert
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @return GPI value
void gpi_disable_input_invert(uint32_t io);
/// @brief Enable IO interrupt and configure interrupt settings
/// @param io Pin number select<BR>Valid range 0 ~ 31<BR>@ref GPIO_BANK_TYPE.GPIO_MAX_NUM
/// @param trigger Pin interrupt type<BR>@ref GPI_INT_TRIGGER_TYPE
/// @param pol Pin interrupt polarity<BR>@ref GPI_INT_POLARITY_TYPE
/// @param cb Interrupt callback function<BR>@ref GPI_IRQ_CB_TYPE
void gpi_enable_int(uint32_t io, gpi_int_trigger_type trigger, gpi_int_polarity_type pol, gpi_irq_cb_type cb);
/// @brief Disable IO interrupt and configure interrupt settings
/// @param io Pin number select<BR>Valid range 0 ~ 31<BR>@ref GPIO_BANK_TYPE.GPIO_MAX_NUM
void gpi_disable_int(uint32_t io);
/// @brief Enable or disable GPI value invert
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @param en enable or disable @ref gpi_enable_input_invert @ref gpi_disable_input_invert
void pad_input_configure(uint32_t io, int en);
/// @brief pad pull up or not pull
/// @param pad pad number select<BR>Valid range 0 ~ 39
/// @param pull pull up or not pull @ref GPI_PULL_TYPE
void pad_pull_config(uint8_t pad, gpi_pull_type pull);
/// @brief Gpi disable interrupt
void gpi_disable_all_int(void);
/// @brief Gpi clear interrupt
void gpi_clear_all_int(void);
/// @brief Get gpi interrupt trigger type
/// @param io Pin number select<BR>Valid range 0 ~ 31
gpi_int_polarity_type gpi_get_int_pol_type(uint8_t io);
/// @brief Get gpi input invert type
/// @param io Pin number select<BR>Valid range 0 ~ 31
/// @return GPI input invert type
gpi_inverted_type gpi_get_input_invert_type(uint8_t io);
/// @brief Read GPI value of all pin IO  range 0 ~ 31 total 32bit
/// @return GPI value
/// @warning Get gpi input value by this function is same with physical
uint8_t gpi_get_physical_val(uint8_t io);
/// @}



void io_irq_enable(uint32_t io, void*   p_callback);
void io_irq_disable(uint32_t io);
void io_irq_disable_all(void);


#endif
