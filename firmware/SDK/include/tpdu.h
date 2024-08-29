/// @file tpdu.h
/// @brief PAR2802 TPDU module
///

#ifndef _TPDU_H_
#define _TPDU_H_

#include "armcm0.h"

/// @addtogroup TPDURoutines Transmission Protocol Data Unit
/// \ingroup SCReaderRoutines
/// @{
///     @brief module document
///
///     TPDU state machine
///     TPDU request helper
///

/// @brief Hander Index definition
enum HEADER_INDEX
{
    CLA,            ///< Instruction class
    INS,            ///< Instruction code
    P1,             ///< Instruction parameters for the command
    P2,             ///< Instruction parameters for the command
    P3,             ///< Instruction parameters for the command
    HEADER_SIZE     ///< Header Size
};

/// @brief Status Index definition
enum STATUS_INDEX
{
    SW1,        ///< Command processing status
    SW2,        ///< Command processing status
    SW_SIZE     ///< Status Size
};

/// @brief TPDU Command format definition
typedef struct
{
    uint8_t header[HEADER_SIZE];    ///< Header
    bool    writeCommand;           ///< Is Tx
    uint8_t data[256];              ///< Command data
    uint8_t sw[SW_SIZE];            ///< Command processing status
} TPDU_COMMAND;

/// @brief TPDU task state definition
typedef enum
{
    TT_IDLE,                    ///< Task status is IDLE
    TT_SEND_HEADER,             ///< Task status is Send Header
    TT_DETERMINE_PROCEDURE,     ///< Task status is Determine Procedure
    TT_WAIT_FOR_TX_FINISH,      ///< Task status is Wait for Tx Finish
    TT_DATA,                    ///< Task status is Process Data
    TT_ERROR,                   ///< Task status is Error
} TPDU_TASK_STATE;

/// @brief TPDU callback definition
typedef void (*TPDU_CALLBACK)(TPDU_COMMAND* command);

/// @brief TPDU Request
/// @param command TPDU command @ref TPDU_COMMAND
/// @param callback Callback function @ref TPDU_CALLBACK
/// @return 0 if error<BR>1 if success
bool tpdu_request(TPDU_COMMAND* command, TPDU_CALLBACK callback);

/// @brief TPDU Reset
void tpdu_reset(void);

/// @brief TPDU Task
/// @param tx_finished Is tx finished
/// @return TPDU status @ref TPDU_TASK_STATE
TPDU_TASK_STATE tpdu_task(bool tx_finished);

/// @}

#endif //_TPDU_H_

