/// @file atr_decoder.h
/// @brief PAR2802 ATR decoer driver
///

#ifndef _ATR_DECODER_H_
#define _ATR_DECODER_H_

#include "armcm0.h"
#include "sc_reader.h"

/// @addtogroup ATRDecoderRoutines ATR decoder
/// \ingroup SCReaderRoutines
/// @{
///     @brief driver document
///
///     Register
///
///     @ref SC_READER_CTRL_TYPE

#define ATR_MAX_T_LENGTH    (31)    ///< max length of ATR bytes
#define DEFAULT_FD          (0x11)  ///< default value of Fi,Di

#pragma pack(push, 1)
typedef struct
{
    uint8_t TS;                     ///< initial byte
    uint8_t T0;                     ///< format byte
    uint8_t T[ATR_MAX_T_LENGTH];    ///< buffer of ATR bytes
    uint8_t Check;                  ///< check byte
    uint8_t ILength;                ///< number of interface bytes
    uint8_t HLength;                ///< number of historical bytes
    uint8_t TotalLength;            ///< total number of bytes
    uint8_t Th_Start;               ///< start index of historical byte
} SC_ATR;
#pragma pack(pop)

/// @brief Reset ATR decoder module
void atr_reset(void);

/// @brief Decode ATR message & check error
/// @param atr content of ATR @ref SC_ATR
bool atr_decode(SC_ATR* atr);

/// @brief Get extra guard time
uint8_t atr_decoder_get_extra_guard_time(void);

/// @brief Get waiting time integer, only valid for T=0
uint8_t atr_decoder_get_waiting_integer(void);

/// @brief Get clock stop indicator value
/// @param high  the logical state the clock line must assume when the clock is stopped
bool atr_decoder_get_clock_stop(bool* high);

/// @brief Check if ATR allow Protocol Parameter Seletion mode
bool atr_decoder_allow_pps(void);

/// @brief Check if ATR allow switching between negotiable mode and specific mode
bool atr_decoder_allow_switch_mode(void);

/// @brief  Get protocol to be used
uint8_t atr_decoder_get_protocol(void);

/// @brief Get default Fi and Di value
uint8_t atr_decoder_get_FD(void);

/// @}

#endif //_ATR_DECODER_H_
