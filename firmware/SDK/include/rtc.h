/// @file rtc.h
/// @brief PAR2802 RTC driver
///

#ifndef _RTC_H_
#define _RTC_H_

#include "ARMCM0.h"
#include "stdbool.h"
#include "string.h"

/// @defgroup RTCRoutines RTC
/// @{
///     @brief driver document
///
///     Register
///
///     @ref RTC_CTRL_TYPE

/// @brief RTC interrupt bitmask definition
typedef enum
{
    RTC_INT_CMP0 = 1, ///< Alarm 0 interrupt bitmask
    RTC_INT_CMP1 = 2, ///< Alarm 1 interrupt bitmask
    RTC_INT_TICK = 4, ///< Tick interrupt bitmask
    RTC_INT_ALL  = 7, ///< All interrupt mask

} rtc_int_type;

/// @brief RTC interrupt callback type
/// @param type RTC interrupt status<BR>Bit definition @ref RTC_INT_TYPE
typedef void (* rtc_irq_callback_type) (rtc_int_type type);

#pragma pack(push, 1)
/// @brief RTC time type definition
///
/// Used for configure calendar and alarms
typedef union
{
    struct
    {
        uint8_t  second;
        uint8_t  minute;
        uint8_t  hour;
        uint8_t  day;
    } decimal_format;
    uint8_t  u8[4];
    uint32_t u32;

} rtc_time_type;
#pragma pack(pop)


/// @brief Set RTC interrupt callback
/// @param cb @ref RTC_IRQ_CALLBACK_TYPE
void rtc_set_interrupt_callback(rtc_irq_callback_type cb);

/// @brief Clear RTC interrupt
/// @param type Write 1 to clear @ref RTC_INT_TYPE
void rtc_int_clear(rtc_int_type type);

/// @brief Enable RTC interrupt
/// @param type Write 1 to enable @ref RTC_INT_TYPE
void rtc_int_enable(rtc_int_type type);

/// @brief Disable RTC interrupt
/// @param type Write 1 to disable @ref RTC_INT_TYPE
void rtc_int_disable(rtc_int_type type);

/// @brief Query RTC interrrupt enabled status
/// @return Bitfield for enabled interrupt @ref RTC_INT_TYPE
rtc_int_type rtc_int_enabled_status(void);

/// @brief Start RTC
void rtc_start(void);

/// @brief Stop RTC
///
/// This API pause RTC only
/// For reset RTC, please ref @ref rtc_clear
void rtc_stop(void);

/// @brief Clear RTC content
///
/// Reset Calendar and tick counter
void rtc_clear(void);

/// @brief Set tick interrupt interval
/// @param tick Interval settings<BR>Tick interrupt will be generated per (32768/tick) second(s)
/// @param adjust_calendar Set 1 to let calendar being updated per second
/// @return 0 if error<BR>tick interval if success
uint32_t rtc_set_prescaler(uint32_t tick, uint8_t adjust_calendar);

/// @brief Set calendar update interval
/// @param order Calendar will be update per (2 ^ order) ticks
void rtc_set_seconds_bit(uint32_t order);

/// @brief Get configured alarm setting
/// @param id Alarm ID<BR>Valid range 0~1
/// @param time @ref RTC_TIME_TYPE
void rtc_get_compare(int id, rtc_time_type* time);

/// @brief Set alarm setting
/// @param id Alarm ID<BR>Valid range 0~1
/// @param time @ref RTC_TIME_TYPE
void rtc_set_compare(int id, rtc_time_type* time);

/// @brief Get calendar
/// @param time @ref RTC_TIME_TYPE
void rtc_get_calendar(rtc_time_type* time);

/// @brief Set calendar
/// @param time @ref RTC_TIME_TYPE
void rtc_set_calendar(rtc_time_type* time);

/// @brief Query if RTC started
/// @return 1 for running
uint8_t rtc_running(void);

/// @}

#endif //_RTC_H_
