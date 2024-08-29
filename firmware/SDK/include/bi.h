/// @file bi.h
/// @brief PAR2860 VBAT driver
#ifndef _BI_H_
#define _BI_H_

#include "ARMCM0.h"


/// @defgroup VBAT
/// @{
///     @brief driver document
///
///     Register
///
///     @ref BI_CTRL_TYPE
/// @brief VBAT channel setting definition


/// @brief Set VBAT channle
/// @param ch VBAT channle
void battery_detect_set_ch(uint8_t ch);
/// @brief Get VBAT channle
/// @param *bi VBAT ADC data value
void battery_detect_get_data(uint16_t* bi);
/// @}
#endif
