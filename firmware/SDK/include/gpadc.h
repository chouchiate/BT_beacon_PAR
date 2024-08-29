/// @file  gpadc.h
/// @brief PAR2860 GPADC driver
#ifndef _GPADC_H_
#define _GPADC_H_

#include "ARMCM0.h"

#define GPADC_CH_MAX    11

/// @defgroup GPADCRoutines GPADC
/// @{
///     @brief driver document
///
///     Register
///
///     @ref GPADC_CTRL_TYPE
/// @brief GPADC channel setting definition
typedef enum
{

    GPIO2_CHANNEL = 0,  ///<    GPIO2           ADCGP_CH[0]
    GPIO3_CHANNEL = 1,  ///<    GPIO3           ADCGP_CH[1]
    GPIO4_CHANNEL = 2,  ///<  GPIO4         ADCGP_CH[2]
    GPIO5_CHANNEL = 3,  ///<    GPIO5           ADCGP_CH[3]

    GPIO28_CHANNEL = 4,  ///< GPIO28        ADCGP_CH[4]
    GPIO29_CHANNEL = 5, ///<    GPIO29      ADCGP_CH[5]
    GPIO30_CHANNEL = 6,  ///< GPIO30      ADCGP_CH[6]
    GPIO31_CHANNEL = 7, ///<    GPIO31      ADCGP_CH[7]
    VBAT_CHANNEL  = 8,  ///<  VBAT            ADCGP_CH[8]

} gpadc_channel_type;

/// @brief GPADC adc value mode
typedef enum
{
    ONESHOT_MODE,///<   only sample one time
    AVERAGE_MODE,///<   only get mutiple samples and average it
} gpadc_mode;
/// @brief GPADC interrupt callback type
typedef void (*gpadc_irq_cb_type)(uint16_t adc_val);
/// @brief GPADC init
/// @param adc_mode GPADC adc value mode @ref GPADC_MODE
/// @param channel GPADC source channel  @ref GPADC_CHANNEL_TYPE
/// @param gpadc_int_en  enable interrupt or not
/// @param gpadc_irq_cb_type  gpadc interrupt callback if enable interrupt is enabled
void gpadc_init(gpadc_mode mode, gpadc_channel_type channel, uint8_t gpadc_int_en, gpadc_irq_cb_type cb);
/// @brief Start tp GPADC adc sampling
void gpadc_start(void);
/// @brief Stop tp GPADC adc sampling
void gpadc_stop(void);
/// @brief Get GPADC adc vlaue
/// @return adc value
uint16_t gpadc_get_value(void);
/// @brief Get GPADC adc event
/// @return adc event
uint8_t gpadc_get_evt(void);
/// @brief Clear GPADC adc event
void gpadc_clr_evt(void);
/// @brief GPADC set interrupt callback
void gpadc_set_callback(gpadc_irq_cb_type cb);
/// @brief  Select GPADC adc channel
/// @return ch channel value
void gpadc_channel_sel(uint8_t ch);
///@}
#endif

