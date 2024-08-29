#ifndef _PXI_PAR_EXTRA_2860_LIB_H_
#define _PXI_PAR_EXTRA_2860_LIB_H_

#include "ARMCM0.h"


typedef enum
{
    PSEUDO_RANDOM_BIT_SEQUENCE_9,
    ALTERNATING_BITS_11110000,
    ALTERNATING_BITS_10101010,
    PSEUDO_RANDOM_BIT_SEQUENCE_15,
    ALL_1_BITS,
    ALL_0_BITS,
    ALTERNATING_BITS_00001111,
    ALTERNATING_BITS_0101,

} payload_type;


uint8_t pxi_rf_freq_to_index(uint16_t freq);
void pxi_rf_le_start_tx(uint16_t freq, payload_type payload,uint8_t len);
void pxi_rf_le_start_rx(uint16_t freq);
uint32_t pxi_rf_le_stop(void);
void pxi_rf_single_tone_start(uint16_t freq);
void pxi_rf_single_tone_stop(void);
void pxi_rf_single_tone_set_mode(uint8_t value);
void pxi_rf_set_xtal_trim_value(uint8_t value);
uint8_t pxi_rf_get_xtal_trim_value(void);
#endif


