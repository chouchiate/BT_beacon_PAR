/// @file  wdt.h
/// @brief PAR2860 WDT driver
#ifndef _WDT_H_
#define _WDT_H_

#include "ARMCM0.h"

/// @defgroup WdtRoutines WDT
/// @{
///     @brief driver document
///
///     Register
///
///     @ref WDT_CTRL_TYPE

/// @brief Initialize WDT module
/// Must be invoked before configureations
/// @param count  range 33~65535. watch dog value init ,if count is 256,then wdt time is 256*7.8 = 1996 ms
void wdt_init(uint16_t count);
/// @brief Start WDT module
void wdt_start(void);
/// @brief Clear WDT value
void wdt_clear(void);
/// @brief Stop WDT module
void wdt_stop(void);
//@}





#endif

