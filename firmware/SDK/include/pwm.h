/// @file  pwm.h
/// @brief PAR2860 PWM driver
#ifndef _PWM_H_
#define _PWM_H_

#include "ARMCM0.h"



/// @defgroup PwmLedRoutines PWM LED
/// @{
///     @brief Driver document
///
#define    PWM_LED_ID_NUM    3    /**<  Total number of PWM LED */
/// @brief PWM_MODULE <BR>
typedef enum
{
    PWM_0,
    PWM_1,
    PWM_2,
} pwm_module;

/// @brief PWM LED mode <BR>
typedef enum
{
    PWM_LED_ALWAYS_ON_MODE,   ///< PWM LED ALWAYS ON MODE
    PWM_LED_PWM_MODE,         ///< PWM LED PWM MODE
    PWM_LED_FLASH_MODE,       ///< PWM LED FLASH MODE
    PWM_LED_BREATH_MODE,      ///< PWM LED BREATH MODE
} pwm_mode;

/**
 * A structure to PWM setting
 */
typedef struct
{

    uint8_t  module;   //模块 使用那个pwm模块

    /**
    * @name flash mode
    */
    /*@{*/
    uint8_t  T1;  /**< T1 value(high level time) for flash mode. Unit:1/32 s */

    uint8_t  T2;  /**< T2 value(T1 + low level time) for flash mode. Unit:1/32 S*/

    uint16_t T3;  /**< T3 value(low level time) for flash mode. Unit:1/32 S*/

    uint8_t  N1;  /**< repeat "T1-T2" N1 times. If N1 set 0x80 then always repeat "T1-T2" and No T3*/

    uint8_t  N2;  /**< repeat "flash" N2 times. If N2 set 0x80 then always repeat "flash".*/
    /*@}*/

    /**
    * @name breath mode
    */
    /*@{*/
    uint8_t  T4;  /**< breath mode , flash period. Unit:1/2ms */

    uint8_t  BR_TH_MAX;   /**< breath mode maxinum briteness threshlod of the LED.Unit:1/2ms */

    uint8_t  BR_TH_MIN;   /**< breath mode minnum briteness threshlod of the LED.Unit:1/2ms  */

    uint8_t  BR_SP;          /**<   BR_SP : breath mode speed.indicate the breath speed. Unit:1/32 ms  */
    /*@}*/

    /**
    * @name PWM mode
    */
    /*@{*/
    uint8_t  PWM_N;    /**< N value for PWM mode. Unit:1/32 ms */

    uint8_t  PWM_M;    /**< M value for PWM mode. Unit:1/32 ms */

    uint8_t  MODE;      /**<  1 : PWM mode   2 : flash mode.   3 : breath mode. */
    /*@}*/
} pwm_parameter;

/// @brief Set PWM enable
/// @param module Module id selection<BR>Valid range 0~2 @ref PWM_MODULE
/// @param pwm_parameter PMW setting structure  @ref PWM_PARAMETER
void pwm_enable(pwm_module module, pwm_parameter parameter);
/// @brief Set PWM disable
/// @param module Module id selection<BR>Valid range 0~2 @ref PWM_MODULE
void pwm_disable(pwm_module module);
/// @brief Set PWM LED repeat
/// @param id Module id selection<BR>Valid range 0~5
/// @param repeat 0 - no repeat<BR>1 - always repeat duty cycle
/// <BR>2 - always repeat (duty cycle+delay time after end of duty cycle repeat)
void pwm_led_flash_repeat(pwm_module module, uint32_t repeat);
/// @brief Set PWM LED flash mode
/// @param id Module id selection<BR>Valid range 0~2
/// @param duty_kept_length length of duty kept of flash mode, Unit: 1/32S
/// @param duty_cycle_length length of duty cycle of flash mode, Unit: 1/32S
/// @param duty_cycle_count count of duty cycle of flash mode, repeat duty cycle (duty_cycle_count+1) times
/// @param duty_cycle_end_delay delay time after end of duty cycle repeat, Unit: 1/32S
/// @param duty_cycles_count count of (duty cycle repeat+duty_cycle_end_delay), repeat duty_cycles_count times
void pwm_led_set_flash(pwm_module module, uint32_t duty_kept_length, uint32_t duty_cycle_length,
                       uint32_t duty_cycle_count, uint32_t duty_cycle_end_delay, uint32_t duty_cycles_count);

/// @brief Set PWM LED breath mode
/// @param id Module id selection<BR>Valid range 0~2
/// @param min breath mode minnum briteness threshlod of the LED, Unit: 1/2ms, initial offset = 15/32 ms = 0.46875ms
/// @param max breath mode maxinum briteness threshlod of the LED, Unit: 1/2ms, initial offset = 15/32 ms = 0.46875ms
/// @param period flash period of breath mode, Unit: 1/2ms
/// @param step breath mode speed, indicate the breath speed, Unit: 1/32ms
void pwm_led_set_breath(pwm_module module, uint32_t min, uint32_t max, uint32_t period, uint32_t step);
/// @brief Set PWM LED always on mode
/// @param id Module id selection<BR>Valid range 0~2
void pwm_led_set_always_on(pwm_module module);
/// @brief Set PWM LED PWM mode
/// @param id Module id selection<BR>Valid range 0~2
/// @param start_high_period Period of PWM high, Unit: 1/32ms
/// @param total_period Total period of PWM, Unit: 1/32ms
void pwm_led_set_pwm(pwm_module module, uint32_t start_high_period, uint32_t total_period);
/// @brief Start PWM LED
/// @param id Module id selection<BR>Valid range 0~2
void pwm_led_start(pwm_module module);
/// @brief Stop PWM LED
/// @param id Module id selection<BR>Valid range 0~2
void pwm_led_stop(pwm_module module);
/// @brief Set the pwm polarity
/// @param id Module id selection<BR>Valid range 0~2
/// @param high pwm polarity 1 or 0
void pwm_led_set_polarity(pwm_module module, int high);
/// @}
#endif

