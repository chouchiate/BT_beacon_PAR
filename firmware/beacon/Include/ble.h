#ifndef _BLE_H_
#define _BLE_H_

#include "ARMCM0.h"


#define CONNECTION_INTERVAL_MIN 12
#define CONNECTION_INTERVAL_MAX 12
#define CONNECTION_LATENCY 0
#define CONNECTION_SVTO 100

#define SECOND_CONNECTION_INTERVAL_MIN 8
#define SECOND_CONNECTION_INTERVAL_MAX 16
#define SECOND_CONNECTION_LATENCY 23
#define SECOND_CONNECTION_SVTO 200

enum _BLE_EVT_TYPE_
{
    BLE_ADV_END                 = 0x01,
    BLE_CONNECTED               = 0x02,
    BLE_DISCONNECTED            = 0x04,
    BLE_NOTIFY_ENABLE           = 0x08,

    BLE_LATENCY_DISABLE         = 0x10,
    BLE_START_ENC               = 0x20,

    BLE_GATT_WRITE          = 0x80,
    BLE_GATT_READ           = 0x100,
};


enum _ST_TYPE_
{
    BLE_ST          = 0x01,
    SYS_ST          = 0x02,
};

enum _SYS_EVT_TYPE_
{
    SYS_TIMER           = 0x01,
    SYS_PASSKEYKEY  = 0x02,
    SYS_PAIRING         = 0x04,
    SYS_SEL_CH      = 0x08,
    SYS_SLP_TIMER       = 0x10,
    SYS_GPIO_CALLBACK   = 0x20,
    SYS_POWER_DOWN  = 0x40,
};




void ble_init(void*   p_callback);
void ble_task(void);
void ble_power_done(uint32_t wakeup_io, uint32_t io_pol);
void ble_adv_start(uint8_t pairing_en);
uint8_t ble_check_notify_enable(uint8_t report_idx);
void ble_set_passkey(uint32_t passkey);
uint8_t ble_tx_data(uint8_t report_idx, uint8_t sz, uint8_t* p_data, uint8_t send_type);
uint8_t ble_notify_data(uint8_t report_idx, uint8_t sz, uint8_t* p_data);
uint8_t ble_is_connected(void);
void ble_disconnect(void);
void ble_dev_sel_update(void);
uint8_t ble_dev_sel_get(void);
void ble_slave_latency_en(uint8_t en);
void ble_mcu_off(uint32_t wakeup_io, uint32_t io_pol);

uint8_t ble_ota_code_erase(void);
#endif



