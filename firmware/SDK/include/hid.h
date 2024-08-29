/// @file  timer.h
/// @brief PAR2860 HID driver
#ifndef _HID_H_
#define _HID_H_

#include "ARMCM0.h"

/// @defgroup HIDRoutines HID
/// @{
///     @brief driver document
///
///     <B>
///     HID module for access button and sensor motion

typedef enum
{
    HID_SPI_MASTER_MODE_0   = 0, ///< Idle low,  first  edge latch CPOL=0 CPHA=0
    HID_SPI_MASTER_MODE_1   = 1, ///< Idle high, first  edge latch CPOL=1 CPHA=0
    HID_SPI_MASTER_MODE_2   = 2, ///< Idle low,  second edge latch CPOL=0 CPHA=1
    HID_SPI_MASTER_MODE_3   = 3, ///< Idle high, second edge latch CPOL=1 CPHA=1
    HID_SPI_MASTER_MODE_NUM = 4,

} hid_spi_mode_type;

typedef enum
{
    HID_WRITE_MSB_LOW, ///< Most significant bit for address write is 0,
    HID_WRITE_MSB_HIGH,  ///< Most significant bit for address write is 1,

} hid_spi_write_msb_pol_type;


#pragma pack(push, 1)
typedef struct
{
    uint8_t btn_cpi: 1;
    uint8_t btn_left: 1;
    uint8_t btn_right: 1;
    uint8_t btn_middle: 1;
    uint8_t btn_4: 1;
    uint8_t btn_5: 1;
    uint8_t btn_wheel: 1;
    uint8_t btn_whee2: 1;
    uint8_t btn_Z: 1;

} hid_button_report_type;
#pragma pack(pop)





/// @brief Initialize HID sensor and button
/// HID module will be enabled after invoking this
void hid_init(void);
/// @brief Stop HID sensor and button
/// HID module will be deisable after invoking this
void hid_stop(void);
/// @brief Get button result
/// @param report Pointer for get button report
void hid_get_button_raw_data(uint16_t* raw_btn_data);

/// @brief Set up HID SPI mode
/// @param mode CPHA and CPOL setting @ref HID_SPI_MODE_TYPE
void hid_spi_set_mode(hid_spi_mode_type mode);
/// @brief HID SPI single address data write
/// @param addr 7bit address for writing
/// @param data data for writing 7bit
void hid_spi_single_write(uint8_t addr, uint8_t data);
/// @brief HID SPI single address data read
/// @param addr 7bit address for reading
/// @param data data for reading
void hid_spi_single_read(uint8_t addr, uint8_t* p_data);
/// @brief HID SPI pixart burst read mode
/// @brief addr 7bit address for pixart reading ,it can set most 5 at once. the max address is 5
/// @param data data for pixart reading,each address is mapping to one data,it can get most 5 data at once. the max data for reading is 5
/// @param size data buffer for reading ,max value is 5
void hid_spi_read_pixart( uint8_t* addr, uint8_t* data, uint8_t  size);
/// @brief HID SPI msb bit setting for different pixart sensor,sensor read operation MSB usually is low,write operation MSB is high
/// @param msb MSB is high or low @ref HID_SPI_WRITE_MSB_POL_TYPE
void hid_spi_set_write_msb_polarity(hid_spi_write_msb_pol_type msb);
/// @brief HID set wheel and btn debounce value
/// @param val debounce value
void hid_set_debounce_threshold(uint8_t val);
/// @brief HID set wheel z btn div value
/// @param val z btn div value
void hid_set_z_btn_div(uint8_t val);
/// @brief Enable HID interrupt
/// @param HID interrupt callback
void hid_int_enable(void*   p_callback);
/// @brief Disable HID interrupt
void hid_int_disable(void);
/// @}
#endif
