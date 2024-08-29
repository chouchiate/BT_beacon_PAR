/// @file  uart.h
/// @brief PAR2860 UART driver
#ifndef _UART_H_
#define _UART_H_

#include "ARMCM0.h"


/// @defgroup UARTRoutines UART 0/1
/// @{
///     @brief driver document

enum uart_baud_sel
{
    UART_BAUD_1200      = 0x00,     ///< 1200 Baud
    UART_BAUD_2400      = 0x01,     ///< 2400 Baud
    UART_BAUD_4800      = 0x02,     ///< 4800 Baud
    UART_BAUD_9600      = 0x03,     ///< 9600 Baud
    UART_BAUD_14400     = 0x04,     ///< 14400 Baud
    UART_BAUD_19200     = 0x05,     ///< 19200 Baud
    UART_BAUD_38400     = 0x06,     ///< 38400 Baud
    UART_BAUD_57600     = 0x07,     ///< 57600 Baud
    UART_BAUD_115200        = 0x08, ///< 115200 Baud
    UART_BAUD_230400        = 0x09, ///< 230400 Baud
    UART_BAUD_460800        = 0x0A, ///< 460800 Baud
    UART_BAUD_921600        = 0x0B, ///< 921600 Baud
};

enum uart_flowctrl
{
    UART_RTS_CTS_ENABLE     = 0x80,     ///< Flow Control Enable
    UART_RTS_CTS_DISABLE        = 0x00, ///< Flow Control Disable
};

enum uart_clock_source
{
    UART_CLK_SOURCE_16M     = 0x00, ///< UART CLK 16M
    UART_CLK_SOURCE_32M     = 0x01, ///< UART CLK 32M
};

typedef enum uart_int_status
{
    UART_RX_INT  = 0x10,
    UART_TX_INT  = 0x20,
    UART_ALL_INT = 0x30,

} uart_int_type;

///@brief uart interrupt callback type
typedef void (*uart_irq_cb_type)(uart_int_type type,uint8_t data);
/// @brief Initialize UART 0
/// @param flowctrl Flow Control
/// @param baud Baud Configuration
/// @note  UART 0 will be enabled after invoking this
extern void uart_0_init(uint8_t flowctrl, uint8_t baud);
/// @brief Send characters with UART0
/// @param buf Output buffer pointer
/// @param sz Size of output buffer in byte
extern void uart_0_write(uint8_t* buf, uint8_t sz);
/// @brief Read UART0
/// <BR>One byte only
/// @param buf Input char address
/// @return 1 - Success<BR>0 - Fail
extern uint8_t uart_0_read(uint8_t* buf);
/// @brief Clear UART0 rx fifo and set RX_FIFO_EMPTY
extern void uart_0_rx_flush(void);
/// @brief Initialize UART 1
/// @param flowctrl Flow Control
/// @param baud Baud Configuration
extern void uart_1_init(uint8_t flowctrl, uint8_t baud);
/// @brief Send characters with UART1
/// @param buf Output buffer pointer
/// @param sz Size of output buffer in byte
/// @note  UART 1 will be enabled after invoking this
extern void uart_1_write(uint8_t* buf, uint8_t sz);
/// @brief Read UART1
/// <BR>One byte only
/// @param buf Input char address
/// @return 1 - Success<BR>0 - Fail
extern uint8_t uart_1_read(uint8_t* buf);
/// @brief Clear UART1 rx fifo and set RX_FIFO_EMPTY
extern void uart_1_rx_flush(void);
/// @brief Read UART0/UART1/
/// <BR>One byte only
/// @param id UART Module Select<BR>Valid range 0 ~ 1
/// @param buf Input char address
extern void uart_read(uint8_t id, uint8_t* buf);
/// @brief UART0 enable interrupt
/// <BR>One byte only
/// @param uart_int_type interrupt type @ref uart_int_status
/// @param uart interrupt callback
extern void uart_0_en_int(uart_int_type int_type, uart_irq_cb_type callback);
/// @brief UART1 enable interrupt
/// <BR>One byte only
/// @param uart_int_type interrupt type @ref uart_int_status
/// @param uart interrupt callback
extern void uart_1_en_int(uart_int_type int_type, uart_irq_cb_type callback);
///@}
#endif
