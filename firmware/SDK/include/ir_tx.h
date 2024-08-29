/// @file ir_tx.h
/// @brief PAR2860 IR_TX driver

#ifndef _IR_TX_H_
#define _IR_TX_H_

#include "ARMCM0.h"

/// @file ir_tx.h
/// @defgroup IR_TX
/// @{
///     @brief Driver document
///
///     Register
///
///     @ref IR_CTRL_TYPE

/// @brief IR NEC carrier space base on duty cycle 30%
#define IR_NEC_CARRIER_SPACE    (3)
/// @brief IR NEC one first half time 560us
#define IR_NEC_ONE_FH_TIME      (0.56)      // 560us
/// @brief IR NEC logical one space
#define IR_NEC_ONE_SPACE        (3)
/// @brief IR RC5 carrier space base on duty cycle 30%
#define IR_RC5_CARRIER_SPACE    (3)
/// @brief IR RC5 one first half time 889us
#define IR_RC5_ONE_FH_TIME      (0.889)     // 889us
/// @brief IR RC5 logical one space
#define IR_RC5_ONE_SPACE        (1)

/// @brief IR NEC carrier space base on duty cycle 30%
#define IR_NEC_CARRIER_SPACE    (3)
/// @brief IR NEC one first half time 560us
#define IR_NEC_ONE_FH_TIME      (0.56)      // 560us
/// @brief IR NEC logical one space
#define IR_NEC_ONE_SPACE        (3)
/// @brief IR RC5 carrier space base on duty cycle 30%
#define IR_RC5_CARRIER_SPACE    (3)
/// @brief IR RC5 one first half time 889us
#define IR_RC5_ONE_FH_TIME      (0.889)     // 889us
/// @brief IR RC5 logical one space
#define IR_RC5_ONE_SPACE        (1)

/// @brief Initialize IR module
/// Must be invoked before configurations
extern void ir_init(void);
/// @brief IR NEC initial function
/// @param carrier_hz  The carrier KHz
extern void ir_nec_init(uint16_t carrier_hz);
/// @brief IR NEC transmit data function
/// @param custom_code_t  Custom code data
/// @param data_code_t   Custom code data
extern void ir_nec_tx_data(uint16_t custom_code_t, uint16_t data_code_t);
/// @brief IR NEC transmit protocal data function
/// @param custom_code_t  Custom code data
/// @param data_code_t   Custom code data
extern void ir_nec_tx_reverse_data(uint8_t custom_code_t, uint8_t data_code_t);
/// @brief IR NEC transmit protocal repeat data function
/// @param custom_code_t  Custom code data
/// @param data_code_t   Custom code data
extern void ir_nec_tx_repeat_data(uint16_t custom_code_t, uint16_t data_code_t);
/// @brief IR RC5 initial function
/// @param carrier_hz  The carrier KHz
extern void ir_rc5_init(uint16_t carrier_kHz);
/// @private
extern void ir_rc5_test_hard(void);
/// @private
extern void ir_tx(uint32_t dr);
/// @private
extern void ir_stop_tx(void);
/// @brief IR RC5 transmit protocal data function
/// @param custom_code_t  Custom code data
/// @param data_code_t   Custom code data
/// @param toggle   toggle
extern void ir_rc5_tx_data(uint8_t custom_code, uint8_t data_code, uint8_t toggle);
/// @brief IR RC5 transmit protocal repeat data function
/// @param custom_code_t  Custom code data
/// @param data_code_t   Custom code data
/// @param toggle   toggle
extern void ir_rc5_tx_repeat_data(uint8_t custom_code, uint8_t data_code, uint8_t toggle);
/// @private
extern uint8_t bit_reverse(uint8_t b);
/// @}

#endif

