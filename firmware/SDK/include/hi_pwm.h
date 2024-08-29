/// @file hi_pwm.h
/// @brief PAR2860 Hi-Speed-PWM driver
///

#ifndef _HI_PWM_H_
#define _HI_PWM_H_

#include "ARMCM0.h"


/// @defgroup Hi-Speed-PWM
/// @{
///     @brief driver document
///
///     Register
///
///     @ref HI_PWM_CTRL_TYPE



/// @brief Hi-Speed-PWM module type  Base clock is MCU clock
typedef enum
{
    HI_PWM_CHANNEL_0 = 0x01,
    HI_PWM_CHANNEL_1 = 0x02,
    HI_PWM_CHANNEL_2 = 0x04,
    HI_PWM_CHANNEL_3 = 0x08,
    HI_PWM_CHANNEL_ALL = 0x0f,

} hi_pwm_channel;

/// @brief Hi-Speed-PWM mode
typedef enum
{
    HI_PWM_DUTY_BASE_ONE_CYCLE = 0, ///< duty time unit is one cycle  base on source clock
    HI_PWM_DUTY_HALF_CYCLE = 1,     ///< duty time unit is half cycle  base on source clock

} hi_pwm_mode;

/// @brief Hi-Speed-PWM interrupt type
typedef enum
{
    HI_PWM_INT_NONE = 0,            ///< No interrupt
    HI_PWM_INT_PERIOD = 1,      ///< interrupt when period
    HI_PWM_INT_TASK_STOP = 2,   ///< interrupt when task stop
    HI_PWM_INT_ALL  = 3,            ///< interrupt all

} hi_pwm_int_type;
/// @brief Hi-Speed-PWM prescaler
typedef enum
{
    Hi_PWM_PERSCALER_1 = 0,          ///< Hi-Speed-PWM clock source is that MCU clock divide 1
    Hi_PWM_PERSCALER_2 = 1,          ///< Hi-Speed-PWM clock source is that MCU clock divide 2
    Hi_PWM_PERSCALER_4 = 2,          ///< Hi-Speed-PWM clock source is that MCU clock divide 4
    Hi_PWM_PERSCALER_8 = 3,          ///< Hi-Speed-PWM clock source is that MCU clock divide 8
    Hi_PWM_PERSCALER_16 = 4,         ///< Hi-Speed-PWM clock source is that MCU clock divide 16
    Hi_PWM_PERSCALER_32 = 5,         ///< Hi-Speed-PWM clock source is that MCU clock divide 32
    Hi_PWM_PERSCALER_64 = 6,         ///< Hi-Speed-PWM clock source is that MCU clock divide 64
    Hi_PWM_PERSCALER_128 = 7,        ///< Hi-Speed-PWM clock source is that MCU clock divide 128
    Hi_PWM_PERSCALER_256 = 8,        ///< Hi-Speed-PWM clock source is that MCU clock divide 256
    Hi_PWM_PERSCALER_512 = 9,        ///< Hi-Speed-PWM clock source is that MCU clock divide 512
    Hi_PWM_PERSCALER_1024 = 10,  ///< Hi-Speed-PWM clock source is that MCU clock divide 1024
    Hi_PWM_PERSCALER_2048 = 11,  ///< Hi-Speed-PWM clock source is that MCU clock divide 2048
    Hi_PWM_PERSCALER_4096 = 12,  ///< Hi-Speed-PWM clock source is that MCU clock divide 4096
    Hi_PWM_PERSCALER_8192 = 13,  ///< Hi-Speed-PWM clock source is that MCU clock divide 8192
    Hi_PWM_PERSCALER_16384 = 14, ///< Hi-Speed-PWM clock source is that MCU clock divide 16384
    Hi_PWM_PERSCALER_32768 = 15  ///< Hi-Speed-PWM clock source is that MCU clock divide 32768

} hi_pwm_perscaler_type;
/// @brief Hi-Speed-PWM begening polarity
typedef enum
{
    HI_PWM_POL_LOW_FIRST = 0,  ///< Hi-Speed-PWM clock start low
    HI_PWM_POL_HIGH_FIRST             ///< Hi-Speed-PWM clock start high

} hi_pwm_polarity_type;

/// @brief Hi-Speed-PWM initial parpameter structure
typedef struct
{
    hi_pwm_channel  channel;
    hi_pwm_mode mode;
    hi_pwm_perscaler_type  prescaler;
    hi_pwm_polarity_type   polarity;
    uint16_t  period;
    uint16_t  duty;

} hi_pwm_parameter;

/// @brief Hi speed pwm interrupt callback type
typedef void (*hi_pwm_irq_cb_type)(uint32_t status);
/// @brief Initial Hi-Speed-PWM setting
/// @param pwm_parameter  @ref HI_PWM_PARAMETER
void high_speed_pwm_init(hi_pwm_parameter* pwm_parameter);
/// @brief Enable Pwm interrupt and set it callback
/// @param cb  @ref HI_PWM_IRQ_CB_TYPE
void high_speed_pwm_enable_interrupt(hi_pwm_irq_cb_type cb);
/// @brief Set Hi-Speed-PWM duty
/// @param channel Hi-Speed-PWM channel @ref HI_PWM_CHANNEL
/// @param compare  duty value
void high_speed_pwm_set_duty(hi_pwm_channel  channel, uint16_t duty);
/// @brief Set Hi-Speed-PWM prescaler
/// @param hi_pwm_perscaler_type prescaler type @ref HI_PWM_PERSCALER_TYPE
void high_speed_pwm_set_prescaler(hi_pwm_perscaler_type ype);
/// @brief Set Hi-Speed-PWM beginning  polarity
/// @param channel Hi-Speed-PWM channel @ref HI_PWM_CHANNEL
/// @param polarity  polarity type  @ref HI_PWM_POLARITY_TYPE
void high_speed_pwm_set_polarity(hi_pwm_channel channel, hi_pwm_polarity_type polarity);
/// @brief Set Hi-Speed-PWM period
/// @param period  Hi-Speed-PWM period value
void high_speed_pwm_set_period(uint16_t period);
/// @brief Set Hi-Speed-PWM mode
/// @param mode  Hi-Speed-PWM period mode @ref HI_PWM_MODE
void high_speed_pwm_set_mode(hi_pwm_mode mode);
/// @brief Set Hi-Speed-PWM start
void high_speed_pwm_start(void);
/// @brief Set Hi-Speed-PWM stop
void high_speed_pwm_stop(void);
///@}
#endif
