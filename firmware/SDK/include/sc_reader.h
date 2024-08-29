/// @file sc_reader.h
/// @brief PAR2802 Smart Card reader driver
///

#ifndef _SC_READER_H_
#define _SC_READER_H_

#include "armcm0.h"

/// @defgroup SCReaderRoutines SmartCard Reader
/// @{
///     @brief driver document
///
///     Register
///
///     @ref SC_READER_CTRL_TYPE

typedef enum SC_CALL_BACK_TYPE
{
    ACT_CB,         ///< callback of activated state
    DACT_CB,        ///< callback of deactivated state
    CALL_BACK_NUM,  ///< number of callback
} SC_CB;

typedef enum SC_PPS_CALL_BACK_TYPE
{
    PPS_REQUEST_CB,     ///< callback of PPS done
    PPS_FAIL_CB,        ///< callback of PPS fail
    PPS_CALL_BACK_NUM,  ///< number of PPS callback
} SC_PPS_CB;

typedef struct
{
    uint8_t PPSS;       ///< initial character, must be 0xFF
    uint8_t PPS[4];     ///< PPS0, PPS1, PPS2, PPS3
    uint8_t PCK;        ///< check character
    uint8_t R_PPSS;     ///< response PPSS
    uint8_t R_PPS[4];   ///< response PPS0, PPS1, PPS2, PPS3
    uint8_t R_PCK;      ///< response PCK
} SC_PPS;

typedef void (*CALLBACK)(void); ///< Callback for activated/deactivated
typedef bool (*PPS_CALLBACK)(SC_PPS* pps);  ///< Callback for PPS releated events

/// @brief Enable and configure default setting of SC reader module
void sc_reader_enable(void);

/// @brief Disable Smart Card reader module
void sc_reader_disable(void);

/// @brief Perform deferred tasks
/// Do TPDU request & PPS verification
void sc_reader_task(void);

/// @brief Configure vcc output of SC reader module when activated
/// @param en 0 - deactivated<BR>1 - activated
void sc_reader_config_vcc_level(bool en);

/// @brief Configure retry enable for each transaction
/// @param en 0 - Disable<BR>1 - Enable
void sc_reader_config_retry(bool en);

/// @brief Callback registration for activated/deactivated
/// @param c pointer of callabck function
/// @param type @ref SC_CB
void sc_reader_add_callback(CALLBACK c, SC_CB type);

/// @brief Callback registration for PPS releated events
/// @param c pointer of callback function
/// @param type @ref SC_PPS_CB
void sc_reader_add_PPS_callback(PPS_CALLBACK c, SC_PPS_CB type);

/// @}

#endif //_SC_READER_H_
