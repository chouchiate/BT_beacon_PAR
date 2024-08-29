#ifndef _PXI_PAR_2860_LIB_H_
#define _PXI_PAR_2860_LIB_H_

#include "ARMCM0.h"

#define BD_ADDR_SZ          6
#define MAX_EDIV_SZ         2
#define MAX_RAND_SZ     8
#define MAX_KEY_SZ          16
#define MAX_IRK_SZ          16
#define LL_WIN_OFFSET_SZ    6
#define MAX_ATT_DATA_SZ 23
#define MAX_ADV_DATA_SZ 31
#define MAX_ATT_REPORT_HDL 20
#define MAX_ATT_HANDLE_SZ 2
#define MAX_LOGICAL_ATT_DATA_SZ	512
#define MAX_ATT_HANDLE_SZ 2
#define CS_IN() { uint32_t old_primask = __get_PRIMASK(); __disable_irq()
#define CS_OUT() __set_PRIMASK(old_primask); }

#pragma pack(1)
/// @file pxi_par2860_ble_lib.h
/// @brief PAR BLE 2860 PXI Style driver
/// @defgroup PAR BLE 2860 PXI Style driver
/// @{
///
/// @brief MCU Clock Select
enum sys_mcu_clock_sel
{
    SYSTEM_CLOCK_64M_RCOSC  = 0x00, ///< MCU Clock 64Mhz
    SYSTEM_CLOCK_32M_RCOSC  = 0x01, ///< MCU Clock 31Mhz
    SYSTEM_CLOCK_16M_RCOSC  = 0x02, ///< MCU Clock 16Mhz

    SYSTEM_CLOCK_32M_XOSC       = 0x08, ///< MCU Clock 32Mhz
};
/// @brief 32K XO Clock source Select
enum sys_32k_xosc_sorce_sel
{
    SYSTEM_32K_XOSC_MODULE_0   = 0x00, ///< System 32K XO setting from GPIO_0,GPIO_1
    SYSTEM_32K_XOSC_MODULE_1 	= 0x01, ///< System 32K XO setting from GPIO_5,GPIO_6

};
/// @brief 32K Clock Select
enum sys_32k_clock_sel
{
    SYSTEM_32K_CLOCK_RCOSC          = 0x00,     ///< 32k Clock RCOSC
    SYSTEM_32K_CLOCK_XOSC               = 0x01,         ///< 32k Clock XTAL
    SYSTEM_32K_CLOCK_32MXO_DIV  = 0x02, ///< 32k Clock From 32M div
};
/// @brief PMU wakeup type configuration
enum wakeup_type
{
    SLEEP_WAKEUP            = 0, ///< Sleep wakeup
    POWERDOWN_WAKEUP    = 1, ///< Power down wakeup
};
/// @brief SMP IO CAPABILITY
enum smp_io_capability
{
    IO_DISPLAY_ONLY     = 0x00, ///< Have the ability to display
    IO_DISPLAY_YESNO        = 0x01, ///< Can indicate either 'yes' or 'no'
    IO_KEYBOARD_ONLY        = 0x02, ///< Device has a numeric keyboard that can input the numbers '0' through '9' and a confirmation.
    IO_NO_INPUT_OUTPUT  = 0x03, ///< Does not have the ability to indicate and display
    IO_KEYBOARD_DISPLAY = 0x04, ///< Device has the ability to display or communicate a 6 digit decimal number
};
/// @brief SMP OOB FLAG
enum smp_oob_flag
{
    OOB_AUTH_NOT_PRESENT = 0x00, ///< Authentication not present
    OOB_AUTH_PRESENT        = 0x01, ///< Authentication present
};
/// @brief SMP BONDING FLAGS
enum smp_bonding_flags
{
    AUTHREQ_NO_BONDING  = 0x00, ///< Authentication request no binding
    AUTHREQ_BONDING     = 0x01, ///< Authentication request binding
};
/// @brief SMP key distribution
enum smp_key_distribution
{
    SMP_KEY_MASTER_IDEN = 0x01,
    SMP_KEY_ADDR_INFO   = 0x02,
    SMP_KEY_SIGNIN_INFO = 0x04,
};
/// @brief Advertising Packet type
enum adv_ch_pkt_type
{
    ADV_IND             = 0x00, ///< Connectable and Scannable Undirected advertising
    ADV_DIRECT_IND  = 0x01, ///< Connectable Directed advertising
    ADV_NOCONN_IND  = 0x02, ///< Non-Connectable and Non-Scannable Undirected advertising
    SCAN_REQ            = 0x03, ///< Scan Request
    SCAN_RSP            = 0x04, ///< Scan Response
    CONNECT_REQ     = 0x05, ///< Connect Request
    ADV_SCAN_IND        = 0x06, ///< Scannable undirected advertising
};
/// @brief BLE Send type
enum ble_send_type
{
    BLE_GATT_NOTIFICATION   = 0x0001,   ///< BLE GATT notification
    BLE_GATT_INDICATION     = 0x0002,   ///< BLE GATT indication
};
/// @brief GAP Event list
enum gap_evt
{
    GAP_EVT_ADV_END                                 = 0x00000001,///< Advertising End
    GAP_EVT_CONNECTED                               = 0x00000002,///< Connected
    GAP_EVT_DISCONNECTED                        = 0x00000004,///< Disconnected
    GAP_EVT_PAIRING_COMP                        = 0x00000008,
    GAP_EVT_PASSKEY_REQ                         = 0x00000010,///< Passkey Request
    GAP_EVT_SHOW_PASSKEY_REQ                = 0x00000020,///< Show Passkey Request
    GAP_EVT_CONNECTION_INTERVAL         = 0x00000040,///< Connection interval
    GAP_EVT_CONNECTION_SLEEP                = 0x00000080,
    GAP_EVT_ATT_READ                                = 0x00000100,///< ATT Read
    GAP_EVT_ATT_WRITE                               = 0x00000200,///< ATT Write
    GAP_EVT_ATT_PREPARE_WRITE               = 0x00000400,///< ATT Prepare Write
    GAP_EVT_ATT_EXECUTE_WRITE               = 0x00000800,///< ATT Execute Write
    GAP_EVT_ATT_HANDLE_CONFIRMATION = 0x00001000,///< ATT Handle Confirmation
    GAP_EVT_ATT_HANDLE_CONFIGURE        = 0x00002000,///< ATT Handle Configure
    GAP_EVT_ENC_START                               = 0x00004000,///< Start Encryption
    GAP_EVT_L2CAP_UPDATE_RSP                = 0x00008000,///< L2CAP update response
    GAP_EVT_CONN_UPDATE_COMP                = 0x00010000,///< Connection update complete
    GAP_EVT_SCAN_REPORT                         = 0x00020000,///< Get Scan Report
    GAP_EVT_SCAN_END                                = 0x00040000,///< Scan end
    GAP_EVT_PAIRING_START                       = 0x00080000,///< Pairing start
    GAP_EVT_LL_REJECT_IND                    = 0x00100000,///< LL Reject Ind
    GAP_EVT_FLASH_WRITE_DONE      							= 0x00200000, /// Flash Write Done
    GAP_EVT_TX_CLOSED_EVT                   = 0x00400000,	///< Tx Close
    GAP_EVT_MTU_EXCHANGE                   = 0x00800000,///< MTU exchange event
};
/// @brief BLE Address type
enum ble_address_type
{
    PUBLIC_ADDRESS_TYPE = 0x00, ///< Public address
    RANDOM_ADDRESS_TYPE     = 0x01, ///< Random address
};
enum att_cmd_code
{
    ATT_ERR_RSP                 = 0x01,
    ATT_MTU_REQ                 = 0x02,
    ATT_MTU_RSP                 = 0x03,
    ATT_FIND_INFO_REQ               = 0x04,
    ATT_FIND_INFO_RSP               = 0x05,
    ATT_FIND_BY_TYPE_VALUE_REQ  = 0x06,
    ATT_FIND_BY_TYPE_VALUE_RSP  = 0x07,
    ATT_READ_BY_TYPE_REQ            = 0x08,
    ATT_READ_BY_TYPE_RSP            = 0x09,
    ATT_READ_REQ                    = 0x0A,
    ATT_READ_RSP                    = 0x0B,
    ATT_READ_BLOB_REQ               = 0x0C,
    ATT_READ_BLOB_RSP               = 0x0D,
    ATT_READ_MULTIPLE_REQ           = 0x0E,
    ATT_READ_MULTIPLE_RSP           = 0x0F,
    ATT_READ_BY_GROUP_TYPE_REQ  = 0x10,
    ATT_READ_BY_GROUP_TYPE_RSP  = 0x11,
    ATT_WRITE_REQ                   = 0x12,
    ATT_WRITE_RSP                   = 0x13,
    ATT_WRITE_CMD                   = 0x52,
    ATT_PREPARE_WRITE_REQ       = 0x16,
    ATT_PREPARE_WRITE_RSP           = 0x17,
    ATT_EXECUTE_WRITE_REQ       = 0x18,
    ATT_EXECUTE_WRITE_RSP           = 0x19,
    ATT_HANDLE_VAL_NOTIFICATION = 0x1B,
    ATT_HANDLE_VAL_INDICATION   = 0x1D,
    ATT_HANDLE_VAL_CONFIRMATION = 0x1E,
    ATT_SIGNED_WRITE_CMD            = 0xD2,
};

enum att_error_code
{
    ATT_INVALID_HANDLE          = 0x01,
    ATT_READ_NOT_PEMITTED           = 0x02,
    ATT_WRITE_NOT_PEMITTED      = 0x03,
    ATT_INVALID_PDU             = 0x04,
    ATT_INSUFFICIENT_AUTHEN     = 0x05,
    ATT_REQ_NOT_SUPPORTED       = 0x06,
    ATT_INVALID_OFFSET              = 0x07,
    ATT_INSUFFICIENT_AUTHOR     = 0x08,
    ATT_PREPARE_QUEUE_FULL      = 0x09,
    ATT_ATTRIBUTE_NOT_FOUND     = 0x0A,
    ATT_ATTRIBUTE_NOT_LONG      = 0x0B,
    ATT_INSUFFICIENT_ENC_KEY_SZ = 0x0C,
    ATT_INVALID_ATTRIBUTE_VAL_LEN = 0x0D,
    ATT_UNLIKELY_ERROR          = 0x0E,
    ATT_INSUFFICIENT_ENC            = 0x0F,
    ATT_UNSUPPORTED_GROUP_TYPE  = 0x10,
    ATT_INSUFFICIENT_RESOURCES  = 0x11,
};

enum att_char_property
{
    ATT_CHAR_BROADCAST          = 0x01,
    ATT_CHAR_READ                   = 0x02,
    ATT_CHAR_WEIRE_WO_RSP       = 0x04,
    ATT_CHAR_WEIRE              = 0x08,
    ATT_CHAR_NOTIFY             = 0x10,
    ATT_CHAR_INDICATE               = 0x20,
    ATT_CHAR_AUTH_SIGNED_WRITE  = 0x40,
    ATT_CHAR_EXTEND_PROPERTY    = 0x80,
};
enum gatt_find_info_uuid_type_
{
    GATT_FIND_INFO_UUID_16  = 0x01,
    GATT_FIND_INFO_UUID_128     = 0x02,
};

typedef enum
{
    PXI_INSTANT_UPDATE_POLLING,
    PXI_INSTANT_UPDATE_CURRENT,
} pxi_instant_update_t;


/// @brief PMU wakeup source configuration
typedef struct gap_wakeup_config
{
    uint8_t wakeup_type;        ///< Set wake up type @ref wakeup_type
    uint8_t timer_wakeup_en;       ///< Enable timer wakeup
    uint8_t gpi_wakeup_en;         ///< Enable I/O wakeup
    uint8_t wdt_wakeup_en;         ///< Enable WatchDog wakeup
    uint8_t rtc_wakeup_en;         ///< Enable RTC wakeup
    uint8_t capdet_wakeup_en;  ///< Enable capdet wakeup
    uint8_t ana_wakeup_en;         ///< Enable analog wakeup
    uint32_t gpi_wakeup_cfg;///< Set wake gpio
} gap_wakeup_config_t;

/// @brief BLE Address
typedef struct gap_ble_addr
{
    uint8_t type;   ///< Address type
    uint8_t addr[BD_ADDR_SZ];   ///< BD_ADDR
} gap_ble_addr_t;
/// @brief Advertising Parameters
typedef struct gap_adv_params
{
    uint8_t     type;                                           ///< Advertising type
    struct      gap_ble_addr    peer_addr;  ///< Peer Address
    uint8_t   policy;                    ///< Policy
    uint8_t   channel;          ///< Channel
    uint16_t  interval;             ///< Advertising Interval in 0.625 ms units (20ms to 10.24s)
    uint16_t    timeout;                ///<< Between 0x0001 and 0x3FFF in seconds, 0x0000 is disable
    uint8_t             hop_interval;  ///< HOP Interval in 336 us units
} gap_adv_params_t;

/// @brief Scan Parameters
typedef struct gap_scan_params
{
    uint8_t mode;           ///< Scan type
    uint8_t channel;        ///< Scan channel
    uint16_t interval;  ///< Scan Interval
    uint16_t window;    ///< Scan Window
    uint16_t timeout;   ///< Scan time out
} gap_scan_params_t;
/// @brief GAP connection update parameters
typedef struct gap_update_params
{
    uint16_t  updateitv_min;    ///< Min Connection Interval
    uint16_t  updateitv_max;    ///< Max Connection Interval
    uint16_t  updatelatency;    ///< Connection Slave Latency
    uint16_t  updatesvto;   ///< Timeout
} gap_update_params_t;
/// @brief GAP Pairing Request
typedef struct smp_pairing_req
{
    uint8_t io;             ///< IO
    uint8_t oob;        ///< OOB
    uint8_t flags: 2;   ///< flags
    uint8_t mitm: 1;    ///< MITM
    uint8_t rsvd: 5; ///< Reserved
    uint8_t max_enc_sz; ///< Maximum Encrypt size
    uint8_t init_key; ///< Initialize Key
    uint8_t rsp_key;    ///< Response
} smp_pairing_req_t;
/// @brief GAP ATT Report
typedef struct gap_att_report
{
    uint16_t    primary;    ///< Primary
    uint16_t    uuid;       ///< UUID
    uint16_t    hdl;        ///< Handle
    uint16_t    config;     ///< Configure
    uint16_t    value;      ///< Value
} gap_att_report_t;
/// @brief GAP ATT Report Handle
typedef struct gap_att_report_handle
{
    uint8_t cnt;    ///< handle count
    struct  gap_att_report report[MAX_ATT_REPORT_HDL];  ///< Report Handle
} gap_att_report_handle_t;
/// @brief Generation of Distributed Keys Parameters
typedef struct gap_key_params
{
    uint8_t ediv[MAX_EDIV_SZ];  ///< EDIV
    uint8_t rand[MAX_RAND_SZ];  ///< RAND
    uint8_t ltk[MAX_KEY_SZ];        ///< LTK
    uint8_t iden[MAX_KEY_SZ];       ///< IDENTITY
} gap_key_params_t;
/// @brief GAP Bond Device
typedef struct gap_bond_dev
{
    struct gap_ble_addr     addr;   ///< BLE Address
    struct gap_key_params   key;    ///< Key Parameter
} gap_bond_dev_t;
/// @brief GAP Profile
typedef struct gatt_profile_struct
{
    uint32_t report_handle_address; ///< Report Handle Address
    uint32_t primary_address;   ///< Primary Address
    uint32_t include_address;   ///< Include Address
    uint32_t characteristic_address;    ///< Characteristic Address
    uint32_t value_address; ///< Value Address
} gatt_profile_struct_t;
/// @brief GAP Disconnect Event
typedef struct gap_disconnected_evt
{
    uint8_t reason;///< GAP Disconnect Event
} gap_disconnected_evt_t;
/// @brief GAP SMP Pairing complete Event
typedef struct gap_pairing_comp_evt
{
    uint8_t status;
    uint8_t dir;
    uint8_t reason;
    struct gap_key_params enc_key;///@ref gap_key_params
} gap_pairing_comp_evt_t;
/// @brief GAP ATT Read Event
typedef struct gap_att_read_evt
{
    uint16_t    primary;    ///< Primary
    uint16_t    uuid;       ///< UUID
    uint16_t    hdl;        ///< Handle
    uint16_t    offset;     ///< Offset
} gap_att_read_evt_t;
/// @brief GAP ATT Write Event
typedef struct gap_att_write_evt
{
    uint16_t    primary;    ///< Primary
    uint16_t    uuid;       ///< UUID
    uint16_t    hdl;        ///< Handle
    uint16_t 	sz;             ///< Write Data Size
    uint8_t* data;  ///< Write data
} gap_att_write_evt_t;
/// @brief GAP ATT prepare write event
typedef struct gap_att_pre_write_evt
{
    uint16_t    primary;  ///< Primary
    uint16_t    uuid;           ///< UUID
    uint16_t    hdl;            ///< Handle
    uint16_t    offset;
    uint8_t sz;                   ///< Write Data Size
    uint8_t data[MAX_ATT_DATA_SZ];  ///< Write data
} gap_att_pre_write_evt_t;
/// @brief GAP ATT Execute Write Event
typedef struct gap_att_exec_write_evt
{
    uint8_t flags;      ///< Execute Write Flag
} gap_att_exec_write_evt_t;
/// @brief GAP ATT Handle Configure Event
typedef struct gap_att_handle_configure_evt
{
    uint16_t    uuid;   ///< UUID
    uint16_t    hdl;    ///< Handle
    uint16_t    value;  ///< Value
} gap_att_handle_configure_evt_t;
/// @brief L2CAP Connection Update Response Event
typedef struct gap_l2cap_update_rsp_evt
{
    uint16_t result; ///< Connection Update Result
} gap_l2cap_update_rsp_evt_t;
/// @brief GAP Link Parameters
typedef struct gap_link_params
{
    uint16_t    interval;   ///< Connection Interval
    uint16_t    latency;    ///< Connection Slave Latency
    uint16_t    svto;   ///< Timeout
} gap_link_params_t;
/// @brief GAP scan devices
typedef struct gap_scan_dev
{
    uint8_t adv_type;
    struct gap_ble_addr peer_addr;
} gap_scan_dev_t;
///
/// @brief GAP scan report
typedef struct gap_scan_report_evt
{
    struct gap_scan_dev dev; ///@ref gap_scan_dev
    uint8_t data_len;///< data length
    uint8_t adv_data[MAX_ADV_DATA_SZ]; ///< adv data
    uint8_t rssi; ///< rssi value
} gap_scan_report_evt_t;
///@brief GAP scan devices evt and scan device list
typedef struct gap_scan_end_evt
{
    uint8_t dev_cnt;
    struct gap_scan_dev* p_dev_list;
} gap_scan_end_evt_t;
/// @brief GAP BLE Event
typedef struct gap_ble_evt
{
    uint8_t evt_type;   ///< Event Type
    uint32_t    evt_code;   ///< Event Code
    union
    {
        struct gap_disconnected_evt         disconn_evt;            ///@ref gap_disconnected_evt
        struct gap_ble_addr             bond_dev_evt;                       ///@ref gap_ble_addr
        struct gap_pairing_comp_evt     pairing_comp_evt;       ///@ref gap_pairing_comp_evt
        struct gap_att_read_evt         att_read_evt;                   ///@ref gap_att_read_evt
        struct gap_att_write_evt            att_write_evt;          ///@ref gap_att_write_evt
        struct gap_att_pre_write_evt        att_pre_write_evt;///@ref gap_att_pre_write_evt
        struct gap_att_exec_write_evt       att_exec_write_evt;///@ref gap_att_exec_write_evt
        struct gap_att_handle_configure_evt att_handle_config_evt;///@ref gap_att_handle_configure_evt
        struct gap_l2cap_update_rsp_evt l2cap_update_rsp_evt;///@ref gap_l2cap_update_rsp_evt
        struct gap_link_params          conn_update_complete_evt;///@ref gap_link_params
        struct gap_scan_report_evt          scan_report_evt;///@ref gap_scan_report_evt
        struct gap_scan_end_evt         scan_end_evt;           ///@ref gap_scan_end_evt
        uint8_t 						flash_operation_st;
    } evt;
} gap_ble_evt_t ;
/// @brief GAP Event Callback
typedef struct gap_evt_callback
{
    uint32_t    evt_mask;   ///< Event Mask
    void    (*p_callback)(struct gap_ble_evt* p_evt);   ///< Callback Function @ref gap_ble_evt
} gap_evt_callback_t;

/// @brief efuse qfn type
enum efuse_qfn_type {
    QFN_48			= 0x00,
    QFN_32			= 0x01,
};

/// @brief FLASH Write event list
enum flash_write_done_event
{
    FLASH_WRITE_USER_PROFILE_DONE,
    FLASH_WRITE_BONDING_INFO_DONE,
};

/// @brief SMP Failed code
enum SMP_FAILED_CODE {
    SMP_RESERVED                = 0x00,
    SMP_PASSKEY_ENTRY_FAILED    = 0x01,
    SMP_OOB_NOT_AVAILABLE       = 0x02,
    SMP_AUTH_REQUIREMENTS       = 0x03,
    SMP_CONFIRM_VALUE_FAILED    = 0x04,
    SMP_PAIRING_NOT_SUPPORTED   = 0x05,
    SMP_ENCTYPTION_KEY_SZ       = 0x06,
    SMP_COMMAND_NOT_SUPPORTED   = 0x07,
    SMP_UNSPECIFIED_REASON      = 0x08,
    SMP_REPEATED_ATTEMPTS       = 0x09,
    SMP_INVALID_PARAMETERS      = 0x0A,
    SMP_FAIL_TIMEOUT            = 0xFF,
};

typedef struct att_err_rsp
{
    uint8_t opcode; // \ref ATT_CMD_CODE
    uint16_t hdl;
    uint8_t err;    // \ref ATT_ERROR_CODE
} att_err_rsp_t;

typedef struct att_mtu_req
{
    uint16_t mtu;
} att_mtu_req_t;

typedef struct att_mtu_rsp
{
    uint16_t mtu;
} att_mtu_rsp_t;

typedef struct att_find_info_req
{
    uint16_t start_hdl;
    uint16_t end_hdl;
} att_find_info_req_t;

typedef struct att_find_info_16
{
    uint16_t hdl;
    uint8_t uuid[2];
} att_find_info_16_t;

typedef struct att_find_info_128
{
    uint16_t hdl;
    uint8_t uuid[16];
} att_find_info_128_t;

union  att_find_info_payload
{
    struct att_find_info_16   uuid16[5];
    struct att_find_info_128 uuid128;
};

typedef struct att_find_info_rsp
{
    uint8_t format;
    union att_find_info_payload pair;
} att_find_info_rsp_t;

typedef struct att_find_by_type_val_req
{
    uint16_t start_hdl;
    uint16_t end_hdl;
    uint16_t att_type;
    uint8_t att_val[MAX_ATT_DATA_SZ - 7];
} att_find_by_type_val_req_t;

typedef struct att_find_by_type_val_rsp
{
    uint8_t list[MAX_ATT_DATA_SZ - 1];
} att_find_by_type_val_rsp_t;

typedef struct att_read_by_type_req
{
    uint16_t start_hdl;
    uint16_t end_hdl;
    uint8_t att_type[16];
} att_read_by_type_req_t;

typedef struct att_read_by_type_service_16
{
    uint16_t hdl;
    uint8_t uuid[2];
} att_read_by_type_service_16_t;

typedef struct att_read_by_type_service_128
{
    uint16_t hdl;
    uint8_t uuid[16];
} att_read_by_type_service_128_t;

union  att_read_by_type_service_payload
{
    struct att_read_by_type_service_16   uuid16[3];
    struct att_read_by_type_service_128 uuid128;
};

typedef struct att_read_by_type_service_rsp
{
    uint8_t length;
    union att_read_by_type_service_payload pair;
} att_read_by_type_service_rsp_t;

typedef struct att_read_by_type_16
{
    uint16_t hdl;
    uint8_t property;
    uint16_t val_hdl;
    uint8_t char_uuid[2];
} att_read_by_type_16_t;

typedef struct att_read_by_type_128
{
    uint16_t hdl;
    uint8_t property;
    uint16_t val_hdl;
    uint8_t char_uuid[16];
} att_read_by_type_128_t;

union  att_read_by_type_payload
{
    struct att_read_by_type_16   uuid16[3];
    struct att_read_by_type_128 uuid128;
};

typedef struct att_read_by_type_rsp
{
    uint8_t length;
    union att_read_by_type_payload pair;
} att_read_by_type_rsp_t;

typedef struct att_read_by_type_pair_val
{
    uint16_t hdl;
    uint8_t val[10];
} att_read_by_type_pair_val_t;

typedef struct att_read_by_type_val_rsp
{
    uint8_t length;
    struct att_read_by_type_pair_val pair[1];
} att_read_by_type_val_rsp_t;

typedef struct att_read_by_type_chartextend_rsp
{
    uint8_t length;
    uint16_t hdl;
    uint8_t val[MAX_ATT_DATA_SZ - 4];
} att_read_by_type_chartextend_rsp_t;

typedef struct att_read_by_type_include_rsp
{
    uint8_t length;
    uint16_t hdl;
    uint8_t buf[MAX_ATT_DATA_SZ - 2];
} att_read_by_type_include_rsp_t;

typedef struct att_read_req
{
    uint16_t hdl;
} att_read_req_t;

typedef struct att_read_rsp
{
    uint8_t buf[MAX_ATT_DATA_SZ-1];
} att_read_rsp_t;

typedef struct att_read_blob_req
{
    uint16_t hdl;
    uint16_t offset;
} att_read_blob_req_t;

typedef struct att_read_blob_rsp
{
    uint8_t buf[MAX_ATT_DATA_SZ-1];
} att_read_blob_rsp_t;

typedef struct att_read_multiple_req
{
    uint16_t hdl[MAX_ATT_DATA_SZ - 1];
} att_read_multiple_req_t;

typedef struct att_read_multiple_rsp
{
    uint8_t val[MAX_ATT_DATA_SZ-1];
} att_read_multiple_rsp_t;

typedef struct att_read_by_group_type_req
{
    uint16_t start_hdl;
    uint16_t end_hdl;
    uint8_t att_group_type[16];
} att_read_by_group_type_req_t;

typedef struct att_read_by_group_type_16
{
    uint16_t hdl;
    uint16_t end_hdl;
    uint8_t uuid[2];
} att_read_by_group_type_16_t;

typedef struct att_read_by_group_type_128
{
    uint16_t hdl;
    uint16_t end_hdl;
    uint8_t uuid[16];
} att_read_by_group_type_128_t;

union  att_read_by_group_type_payload
{
    struct att_read_by_group_type_16   uuid16[3];
    struct att_read_by_group_type_128 uuid128;
};

typedef struct att_read_by_group_type_rsp
{
    uint8_t length;
    union  att_read_by_group_type_payload pair;
} att_read_by_group_type_rsp_t;

typedef struct att_write_req
{
    uint16_t hdl;
    uint8_t buf[MAX_ATT_DATA_SZ - 3];
} att_write_req_t;

typedef struct att_write_cmd
{
    uint16_t hdl;
    uint8_t buf[MAX_ATT_DATA_SZ - 3];
} att_write_cmd_t;

typedef struct att_prepare_write_req
{
    uint16_t hdl;
    uint16_t offset;
    uint8_t buf[MAX_ATT_DATA_SZ - 5];
} att_prepare_write_req_t;

typedef struct att_prepare_write_rsp
{
    uint16_t hdl;
    uint16_t offset;
    uint8_t buf[MAX_ATT_DATA_SZ - 5];
} att_prepare_write_rsp_t;

typedef struct att_execute_write_req
{
    uint8_t flags;
} att_execute_write_req_t;

typedef struct att_hdl_val_notifivation
{
    uint16_t hdl;
    uint8_t buf[MAX_ATT_DATA_SZ-3];
} att_hdl_val_notifivation_t;

typedef struct att_hdl_val_indication
{
    uint16_t hdl;
    uint8_t buf[MAX_ATT_DATA_SZ-3];
} att_hdl_val_indication_t;
union attc
{
    struct att_mtu_rsp AttMtuRsp;
    struct att_find_info_rsp AttFindInfoRsp;
    struct att_find_by_type_val_rsp AttFindByTypeValRsp;
    struct att_read_by_type_rsp AttReadByTypeRsp;
    struct att_read_by_type_include_rsp AttReadByTypeIncludeRsp;
    struct att_read_by_type_val_rsp AttReadByTypeValRsp;
    struct att_read_by_type_service_rsp AttReadByTypeServiceRsp;
    struct att_read_by_type_chartextend_rsp AttReadByTypeChartExtendRsp;
    struct att_read_rsp AttReadRsp;
    struct att_read_blob_rsp AttReadBlobRsp;
    struct att_read_multiple_rsp AttReadMultipleRsp;
    struct att_read_by_group_type_rsp AttReadByGroupTypeRsp;
    struct att_hdl_val_notifivation AttHdlValNotification;
    struct att_hdl_val_indication AttHdlValIndication;
    struct att_err_rsp AttErrRsp;
};
typedef struct attc_ble_evt
{
    uint8_t attc_code;  // \ref ATT_CMD_CODE
    uint8_t attc_sz;
    union attc* attc_ptr;

} attc_ble_evt_t;

#pragma pack()


/// @brief ATT client Event Callback
typedef void (*p_attc_callback)(struct attc_ble_evt* p_evt);
/**
*@brief BLE init
*@details ble init function
*
*@note
*/
uint8_t pxi_gap_s_ble_init(uint8_t* pkey);
/**
* @brief BLE disconnect
* @details use to disconnect Ble
*
* @note
*/
uint8_t pxi_gap_s_disconnect(void);
/**
* @brief BLE set device address
* @details use to set BLE device address
*
* @param BLE gap address structure @ref gap_ble_addr
* @note
*/
uint8_t pxi_gap_s_ble_address_set(struct gap_ble_addr* p_dev_addr);
/**
* @brief BLE get device address
* @details use to get BLE advertising address
*
* @param BLE gap address structure @ref gap_ble_addr
* @return is success
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_ble_address_get(struct gap_ble_addr* p_dev_addr);
/**
* @brief BLE set LE feature
* @details use to set BLE LL_feature parameter
*
* @param *p_feature BLE LL_feature in byte
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @param BLE LL_feature in each bit
* @note
*/
uint8_t pxi_gap_s_ble_slave_feature_set(uint8_t* p_feature);
/**
* @brief BLE set advertising parameter
* @details use to set BLE advertising parameter.
*
* @param BLE gap advertising parameter structure @ref gap_adv_params
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_adv_parameters_set(struct gap_adv_params* p_adv_params);
/**
* @brief BLE set advertising data
* @details use to set BLE advertising data.
*
* @param *p_adv BLE advertising data
* @param adv_sz BLE advertising data size
* @param *p_scan BLE scan data
* @param sacn_sz  BLE scan data size
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_adv_data_set(uint8_t* p_adv, uint8_t adv_sz, uint8_t* p_scan, uint8_t sacn_sz);
/**
* @brief BLE start advertising
* @details use to start BLE advertising
*
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_adv_start(void);
/**
* @brief BLE stop advertising
* @details use to stop BLE advertising
*
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_adv_stop(void);
/**
* @brief BLE start to scan
* @details use to start BLE scan
*
* @param *p_scan_params sacn_sz  BLE scan data parameter structure @ref p_scan_params
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_scan_parameters_set(struct gap_scan_params* p_scan_params);
/**
* @brief BLE start scan
* @details use to start scan
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_scan_start(void);
/**
* @brief BLE stop scanning
* @details use to stop BLE scanning
*
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_scan_stop(void);
/**
* @brief BLE set security parameter
* @details use to set BLE security parameter  for pairing requset
*
* @param *p_sec_params BLE security parameter structure @ref smp_pairing_req
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_security_parameters_set(struct smp_pairing_req* p_sec_params);
/**
* @brief BLE set pass key
* @details use to pass key for BLE  security
*
* @param passkey BLE security passkey
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_passkey_set(uint32_t passkey);
/**
* @brief BLE set auth request
* @details use to set auth request
*
* @param flag  BLE bonding flag
* @param mitm  BLE mitm protect
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_security_req(uint8_t flag, uint8_t mitm);
/**
* @brief BLE set connection update parameter
* @details use to set BLE connection update parameter
*
* @param *p_update_params connection update parameter structure  @ref gap_update_params
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_connection_update(struct gap_update_params* p_update_params);
/**
* @brief BLE get current LL_LINK parameter
* @details use to get BLE LL_LINK parameter
*
* @param *p_link current link parameter structure  @ref gap_link_params
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_link_parameters_get(struct gap_link_params* p_link);
/**
* @brief BLE set win wide minus cont
* @details BLE set win wide minus cont
*
* @param cnt for count
* @return is sucess or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_set_win_wide_minus_cnt(uint8_t cnt);
/**
* @brief BLE enter slave latency mode
* @details use to  enter BLE slave latency mode
*
* @param en  1:enter BLE slave latency mode 0:leave BLE slave latency mode
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_connection_latency_mode(uint8_t en);
/**
* @brief BLE get device gatt report handle
* @details use to  get BLE device gatt report handle
*
* @param ** p_hdl  gatt report handler structure@ref gap_att_report
* @return is success or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_gatt_report_handle_get(struct gap_att_report_handle** p_hdl);
/**
* @brief BLE set gatt read response
* @details use to set BLE gatt read response
*
* @param len  size of read response data
* @param *p_data  data of read response data
* @return is sucess or not
*     @retval 1 success
*     @retval 0 fail
* @note
*/
uint8_t pxi_gap_s_gatt_read_rsp_set(uint16_t len, uint8_t* p_data);
/**
* @brief BLE check tx fifo available sz
* @details use to get  BLE tx fifo available sz
*
* @return tx fifo available sz
* @note
*/
uint8_t pxi_gap_s_get_available_fifo_sz(void);
/**
* @brief BLE check tx fifo is full or not
* @details use to check BLE tx fifo is full or not
*
* @return is full or not
*     @retval 0 not full
*     @retval 1 full
* @note
*/
uint8_t pxi_gap_s_check_fifo_full(void);
/**
* @brief BLE check tx fifo is empty or not
* @details use to check BLE tx fifo is empty or not
*
* @return is empty or not
*     @retval 1 empty
*     @retval 0 not empty
* @note
*/
uint8_t pxi_gap_s_check_fifo_empty(void);
/**
* @brief BLE check tx fifo is full or not
* @details use to check BLE tx fifo is full or not
*
* @return is full or not
*     @retval 0 not full
*     @retval 1 full
* @note
*/
uint8_t pxi_gap_s_check_fifo_sz(void);
/**
* @brief BLE gatt send  data
* @details use to send gatt data by notification or indication notification.
*
* @param type notification or indication notification
* @param *p_report  gap att report structure @ref gap_att_report
* @param len  size of gatt data
* @param *p_data  gatt data
* @return is success or not
*     @retval 1 success
*     @retval 0 not success
* @note
*/
uint8_t pxi_gap_s_gatt_data_send(uint8_t type, struct gap_att_report* p_report, uint16_t len, uint8_t* p_data);
/**
* @brief Get rand number
* @details use to get rand number
*
* @return rand number
* @note
*/
uint8_t pxi_rand_number_get(void);
/**
* @brief Delay micro second
* @details use to delay micro second
*
* @param dly micro second
* @note
*/
void pxi_delay_us(uint16_t dly);
/**
* @brief Delay mini second
* @details use to delay mini second
*
* @param dly mini second
* @note
*/
void pxi_delay_ms(uint32_t dly);
/**
* @brief Get company ID
* @details use to  get company ID
*
* @return company ID
* @param null
* @note
*/


void pxi_ble_sched_execute(void);
/**
* @brief BLE fw schedule execute finish
* @details use to finish executing schedule task
*
* @note
*/
uint8_t pxi_ble_sched_finish(void);
/**
* @brief BLE LPO calibration
* @details use to calibrate BLE LPO
*
* @note
*/
uint8_t pxi_sys_32k_lpo_calibration(void);
/**
* @brief BLE RC oscillator calibration
* @details use to calibrate BLE RC oscillator calibration
*
* @note
*/
uint8_t pxi_sys_mcu_rc_calibration(void);
/**
* @brief BLE MCU clock switch
* @details use to switch BLE device MCU clock
*
* @param sel  mcu clock
* @note
*/
uint8_t pxi_sys_mcu_clock_set(uint8_t sel);
/**
* @brief BLE Init System 32k XOSC Clock Source
* @details use to init System 32k XOSC Clock Source
*
* @param sel  32k clock source @ref 32k_xosc_sorce_clock_sel
* @note
*/
void  pxi_sys_32k_xosc_init(uint8_t sel);
/**
* @brief BLE MCU clock source switch
* @details use to switch BLE device 32k clock source
*
* @param sel  32 clock source
* @note
*/
uint8_t pxi_sys_32k_clock_set(uint8_t sel);
/**
* @brief BLE MCU get clock switch
* @details use to  get switch BLE device MCU clock
*
* @param *p_sel  mcu clock
* @note
*/
uint8_t pxi_sys_mcu_clock_get(uint8_t* p_sel);
/**
* @brief BLE MCU get clock switch
* @details use to get switch BLE device MCU clock source
*
* @param *p_sel  muc clock  source
* @note
*/
uint8_t pxi_sys_32k_clock_get(uint8_t* p_sel);

/**
* @brief BLE disable system timer
* @details use to disable BLE device timer
*
* @param type type of system timer
* @note
*/
void pxi_sys_timer_stop(uint16_t type);
/**
* @brief BLE config wake up setting
* @details use to config BLE wake up setting
*
* @param *p_cfg configuration of wake up @res gap_wakeup_config
* @note
*/
uint8_t pxi_pmu_wakeup_config(struct gap_wakeup_config* p_cfg);
/**
* @brief BLE sleep LL
* @details use to sleep BLE LL
*
* @note
*/
uint8_t pxi_link_layer_sleep(void);
/**
* @brief Skip mcu sleep routine one time
* @details call this function to skip mcu sleep
*
* @note
*/
void pxi_pmu_vote_not_sleep(void);
/**
* @brief BLE enter system sleep mode
* @details use to enter BLE device system sleep mode
*
* @note
*/
uint8_t pxi_system_sleep(void);
/**
* @brief BLE enter system power down mode
* @details use to power down BLE device system
*
* @note
*/
uint8_t pxi_system_power_down(void);
/**
* @brief BLE enter deep sleep mode
* @details use to deep sleep BLE device system
*
* @note
*/
void pxi_system_deep_sleep(void);
/**
* @brief BLE reset system
* @details use to reset BLE device system
*
* @note
*/
void pxi_system_reset(void);
/**
* @brief BLE  RF sleep
* @details use to enable RF sleep
*
* @note
*/
uint8_t pxi_rf_sleep(void);
/**
* @brief BLE  RF power down
* @details use to power down
*
* @note
*/
uint8_t pxi_rf_power_down(void);
/**
* @brief BLE RF wake up
* @details use to enable RF wake up
*
* @note
*/
uint8_t pxi_rf_wake_up(void);
/**
* @brief BLE 32m xosc  enable
* @details use to enable 32m xosc
*
* @note
*/
uint8_t pxi_32m_clock_enable(uint8_t en);
/**
* @brief BLE 32m xosc  enable
* @details use to enable 32m xosc
*
* @note
*/
uint8_t pxi_sys_32m_xosc_clock_enable(uint8_t en);
/**
* @brief BLE set bond manager index
* @details use to set bond manager index
*
* @param idx index of bond manager
* @note
*/
uint8_t pxi_bm_s_bond_manager_idx_set(uint8_t idx);
/**
* @brief BLE get bond devices information
* @details use to get BLE bond devices information
*
* @param *p_device bond devices structure @eef gap_bond_dev
* @note
*/
uint8_t pxi_bm_s_bond_info_get(struct gap_bond_dev* p_device);
/**
* @brief BLE add bond devices information
* @details use to add BLE bond devices information
*
* @param *p_device bond devices structure @eef gap_bond_dev
* @note
*/
uint8_t pxi_bm_s_bond_info_add(struct gap_bond_dev* p_device);
/**
* @brief BLE delete single bond device information
* @details use to delete single bond BLE device information
*
* @note
*/
uint8_t pxi_bm_s_bond_info_delete(void);
/**
* @brief BLE delete all bond device information
* @details use to delete all bond BLE device information
*
* @note
*/
uint8_t pxi_bm_s_bond_info_delete_all(void);
/**
* @brief BLE read fw profile data from flash
* @details use to BLE read BLE fw profile data from flash
*
* @param addr       the profile flash data address index
* @param len        length of profile flash data
* @param *p_buf data for profile reading
* @note
*/
uint8_t pxi_gap_s_profile_data_read(uint16_t addr, uint16_t len, uint8_t* p_buf);
/**
* @brief BLE write fw profile data from flash
* @details use to BLE write BLE fw profile data from flash
*
* @param addr       the profile flash data address index
* @param len        length of profile flash data
* @param *p_buf data for profile writing
* @note par 2860 default reserve 4096 byte flash space for application using
* @warm the parameter of length maximum is 1024 byte ,if you want to write more than 1024 byte,please splice it manually
*/
uint8_t pxi_gap_s_profile_data_write(uint16_t addr, uint16_t len, uint8_t* p_buf);
/**
* @brief BLE erase fw cdoe in flash
* @details use erase BLE fw cdoe in flash
*
* @note
*/
uint8_t pxi_code_erase(void);
/**
* @brief BLE write fw bin into device flash
* @details use to write fw bin into BLE device flash
*
* @param offset   flash address offset
* @param len        length of fw bin  data
* @param *p_buf fw bin data for flash writing
* @note
*/
uint8_t pxi_code_write(uint32_t offset, uint16_t len, uint8_t* p_buf);
/**
* @brief BLE check fw bin for updating
* @details use to write fw bin into BLE device flash
*
* @param *p_desc     fw descriptor
* @param *p_ver      fw version info
* @param    sz     size of fw descriptor
* @param checksum    fw checksum for updating
* @note
*/
uint8_t pxi_code_update(uint8_t* p_desc, uint8_t* p_ver, uint32_t sz, uint16_t checksum);

///@brief Get RCClock frequency
///@return RC frequency in Hz
uint32_t pxi_sys_rc_get(void);
/**
* @brief BLE set gatt profile source from header in project

* @param *gatt_profile_struct    gatt profile address
* @warm set gatt prfole before ble init,and if you want to use gatt profile from flash ,don't call this function.
* @code

    gatt.report_handle_address = (uint32_t)_gatt_database_report_handle;
    gatt.primary_address= (uint32_t)_gatt_database_primary;
    gatt.include_address= (uint32_t)_gatt_database_include;
    gatt.characteristic_address= (uint32_t)_gatt_database_characteristic;
    gatt.value_address= (uint32_t)_gatt_database_value;
    DBGPRINTF((">BleInit\r\n"));
    pxi_gap_s_ble_init();
    DBGPRINTF(("<BleInit\r\n"));

    DBG(">GAPProfile\r\n");
    pxi_gap_s_gatt_profiles_set(&gatt);
    DBG("<GAPProfile\r\n");
*   @endcode
*/
void pxi_gap_s_gatt_profiles_set(struct gatt_profile_struct* p_gatt_profile);
/**
* @brief pxi get lib version string
* @details use to get lib version string
*/
const char* pxi_get_lib_version(void);
/**
* @brief pxi get lib branch string
* @details use to get lib branch string
*/
const char* pxi_get_lib_branch(void);
/**
* @brief pxi get lib branch string
* @details use to get lib branch string
*/
const char* pxi_get_lib_build_date(void);
/**
* @brief pxi set gap callback
* @details use to get lib branch string
*/
uint8_t pxi_gap_s_evt_handler_set(struct gap_evt_callback* p_callback);
/**
* @brief pxi set att client callback
* @details use to get lib branch string
*/
uint8_t pxi_att_c_set_callback(p_attc_callback pfn_callback);
/**
* @brief pxi set att client mtu req
*   @param mtu   mtu value
*/
uint8_t pxi_att_c_mtu_req(uint16_t mtu);/* mapping ATTCMTUReq(uint16_t mtu) */
/**
* @brief pxi set att client find info req
*   @param start_hdl     start handle value
*   @param end_hdl       end handle value
*/
uint8_t pxi_att_c_find_info_req(uint16_t start_hdl, uint16_t end_hdl);/* ATTCFindInfoReq(uint16_t start_hdl, uint16_t end_hdl) */
/**
* @brief pxi set att client findbytype value req
*   @param start_hdl     start handle value
*   @param end_hdl       end handle value
*/
uint8_t pxi_att_c_find_by_type_value_req(uint16_t start_hdl, uint16_t end_hdl, uint16_t type, uint8_t val_sz, uint8_t* p_val);/*ATTCFindByTypeValueReq(uint16_t start_hdl, uint16_t end_hdl, uint16_t type, uint8_t val_sz, uint8_t *p_val)*/
/**
* @brief pxi set att client read by type req  req
*   @param start_hdl     start handle value
*   @param end_hdl       end handle value
*/
uint8_t pxi_att_c_read_by_type_req(uint16_t start_hdl, uint16_t end_hdl, uint16_t type_sz, uint8_t* p_type);
/**
* @brief pxi  att client read req
*   @param hdl    handle value
*/
uint8_t pxi_att_c_read_req(uint16_t hdl);
/**
* @brief pxi  att client read blob req
*   @param hdl    handle value
*/
uint8_t pxi_att_c_read_blob_req(uint16_t hdl, uint16_t offset);
/**
* @brief pxi  att client read multiple req
*   @param hdl  handle value
*/
uint8_t pxi_att_c_read_multiple_req(uint8_t hdl_sz, uint8_t* p_hdl);
/**
* @brief pxi  att client read group type req
*   @param start_hdl     start handle value
*   @param end_hdl       end handle value
*/
uint8_t pxi_att_c_read_by_group_type_req(uint16_t start_hdl, uint16_t end_hdl, uint16_t type_sz, uint8_t* p_type);
/**
* @brief pxi  att client write req
*   @param hdl  handle value
*/
uint8_t pxi_att_c_write_req(uint16_t hdl, uint16_t sz, uint8_t* p_buf);
/**
* @brief pxi  att client write cmd req
*   @param hdl  handle value
*/
uint8_t pxi_att_c_write_cmd_req(uint16_t hdl, uint16_t sz, uint8_t* p_buf);
/**
* @brief pxi  att prepare write  req
*   @param hdl  handle value
*/
uint8_t pxi_att_c_prepare_write_req(uint16_t hdl, uint16_t offset, uint16_t sz, uint8_t* p_buf);
/**
* @brief pxi  att execute write  req
*/
uint8_t pxi_att_c_execute_write_req(uint8_t flags);
/**
* @brief pxi  att confirmation
*/
uint8_t pxi_att_c_confirmation(void);
/**
* @brief BLE get fw code related information
* @param *p_fw_ver  fw code version
* @param *p_fw_desc fw code description
* @param *p_fw_sz	fw size
* @param *p_fw_checksum fw checksum
* @warm set gatt profile before ble init,and if you want to use gatt profile from flash ,don't call this function.
*/
uint8_t pxi_code_info_read(uint8_t *p_fw_ver, uint8_t *p_fw_desc, uint32_t* p_fw_sz, uint16_t* p_fw_checksum);
/**
* @brief BLE get fw code related information
* @return value qfn type
*/
uint8_t pxi_get_qfn_type(void);
/**
* @brief Enable link layer auto sleep or not
* @param en , en = 1 ,enable auto sleep when no data communication ,if en = 0 link layer is always on ,no sleep
*/
uint8_t pxi_ble_set_conn_auto_sleep(uint8_t en);
/**
* @brief Enable multiple bonding
* @param en , en = 1 ,enable multiple bonding, if en = 0 disable multiple bonding
*/
void pxi_ble_config_multiple_bonding(uint32_t en);
/**
* @brief Set maximum bonding number
* @param bonding_num, maximum bonding number
* @return if bonding_num <=10 && multiple bonding enable, then return 1,otherwise return 0.
*/
uint8_t pxi_ble_set_max_bonding(uint8_t bonding_num);
/**
* @brief Get maximum bonding number
* @return maximum bonding number
*/
uint8_t pxi_ble_get_max_bonding(void);
/**
* @brief Get used bonding number
* @return used bonding number
*/
uint8_t pxi_ble_get_bonding_used_num(void);
/**
* @brief BLE write fw profile data from flash
* @details use to BLE write BLE fw profile data from flash
*
* @param addr       the profile flash data address index
* @param len        length of profile flash data
* @param *p_buf data for profile writing
* @warning  1.The parameter of length maximum is 4096 byte ,if you want to write more than 4096 byte,please splice it manually\r\n
*       2.This function not erase before using this function ,please erase manually .
*/
uint8_t pxi_sys_user_profile_data_write(uint32_t addr, uint16_t len, uint8_t *pbuf);
/**
* @brief BLE read fw profile data from flash
* @details use to BLE read BLE fw profile data from flash
*
* @param addr       the profile flash data address index
* @param len        length of profile flash data
* @param *p_buf data for profile reading

* @note
*/
uint8_t pxi_sys_user_profile_data_read(uint32_t addr, uint16_t len, uint8_t *pbuf);
/**
* @brief BLE erase fw profile data from flash
* @details use to BLE read BLE fw profile data from flash
*
* @param addr       the profile flash data address index
* @param num_of_sector       number of sector for erasing, each sector is 4096 bytes
*
* @note   if addr is 0x800,  num_of_sector is 3 , and it will erase 4096 bytes from 0x0000 to 0x3000.\r\n
* @note   if addr is 0x14A4,  num_of_sector is 1 , and it will erase 4096 bytes from 0x1000 to 0x2000.\r\n

*/
uint8_t pxi_sys_user_profile_data_erase(uint32_t addr, uint8_t num);
/**

* @brief BLE erase fw cdoe sector in flash
* @param sector  num of sector.
*
* @note
*/
uint8_t pxi_ota_code_erase_sector(uint8_t offset);
/**
* @brief BLE get ota target code bank sector size;
*
* @note
*/
uint8_t pxi_ota_get_code_bank_sector_size(void);
/**
* @brief Let application to control write response
* @return 1 - Write request, 0 - Other ATT Write commands
*/
uint8_t pxi_gap_s_att_skip_current_write_response(void);
/**
* @brief Send ATT write response to host
*/
void pxi_gap_s_att_write_response(void);
/**
* @brief Reset system to another code bank
* @return 0 if error <BR> not return if successfully triggered
*/
uint8_t pxi_ota_code_jump(void);
/**
* @brief Check the opposite partition is enough to program
* @return 0 if error <BR> not return if successfully triggered
*/
uint8_t pxi_ota_check_partition_size(uint32_t sz);
/**
* @brief Update OTA code but do not switch to update bank
* @param p_desc update destination
* @param p_ver version
* @param sz checksum buffer size
* @param checksum checksum buffer
* @return 0 if error <BR>1 if success
*/
uint8_t pxi_ota_code_update_without_code_switch(uint8_t *p_desc, uint8_t *p_ver, uint32_t sz, uint16_t checksum);
/**
* @brief Gen a  ble mac  address
* @param addr  ble mac address 6 bytes
*/
void pxi_ble_gen_mac_addr(uint8_t* addr);
/// @brief Select instant update method
void pxi_selection_instant_update(pxi_instant_update_t sel);
/// @brief Get user profile sapce size
uint32_t pxi_sys_user_profile_data_get_size(void);
/// @brief check ble is enable multi bonding function
uint8_t pxi_ble_check_multi_bonding(void);
/// @brief Trigger device to save bonding information
void pxi_bm_s_bond_info_save(void);
/// @brief Get mtu size
/// @return mtu size
uint16_t pxi_att_c_get_mtu_size(void);
/// @brief pxi erase object. One object = 4096 bytes.
/// @param offset ota code bank address offset
uint8_t pxi_ota_code_erase_object(uint32_t offset);

/// @brief ble response mtu req mtu maximum is 512
void pxi_att_mtu_set(uint16_t mtu);
/// @brief set flag to connect white list
/// @param enable check flag
/// @return 0 if error <BR>1 if success
uint8_t pxi_set_whitelist(uint8_t enable);

/// @brief set rf power
/// @param dBm +0/+4
/// @return 0 if error <BR>1 if success
uint8_t pxi_set_rf_power(uint8_t dBm);

///@}

#endif


