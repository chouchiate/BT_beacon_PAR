/// @file ota.h
/// @brief OTA driver 3.1
///

#ifndef _OTA_H_
#define _OTA_H_

#include "ARMCM0.h"

/// @defgroup OTARoutines OTA
/// @{
///     @brief driver document
///
///     Interface
///

#pragma pack(1)
#define CMD_FW_OTA_INIT 0x10
#define CMD_FW_WRITE    0x17
#define CMD_FW_UPGRADE  0x18
#define CMD_FW_MCU_RESET    0x22    //Todo: remove this OP code handle after windows OTA app ready
#define CMD_FW_GET_INFO     0x23

#define CMD_FW_OBJECT_CREATE 0x25
#define CMD_FW_OTA_INIT_NEW 0x27
#define CMD_FW_OTA_RETRANSMIT 0x28
#define CMD_FW_OTA_DISCONNECT 0x29

#define ERR_COMMAND_SUCCESS           0x00
#define ERR_COMMAND_UPDATE_FAIL       0xFF
#define EVT_COMMAND_COMPLETE        (0x0E)
#define CMD_COMPLETE_HDR_SZ     1

#define OTA_BUFFER_SIZE 256
#define MAX_OBJECT_SIZE 4096

/// @brief OTA event handler type
/// @param p_event_data Pointer to OTA event
/// @param event_size OTA event size
typedef void (*app_sched_event_handler_t)(uint8_t* p_event_data, uint16_t event_size);

struct cmd_fw_ota_init_new;
struct ret_fw_info_get;

/// @brief OTA spec check callback type
/// @param p_cmd @ref cmd_fw_ota_init_new
typedef uint8_t (*ota_spec_check_cb_t)(struct cmd_fw_ota_init_new* p_cmd);

/// @brief OTA upgrade callback type
/// @param p_desc update target description
/// @param p_ver version
/// @param sz checksum buffer size
/// @param checksum checksum
/// @return 0 if error <BR>1 if success
typedef uint8_t (*ota_upgrade_cb_t)(uint8_t* p_desc, uint8_t* p_ver, \
                                    uint32_t sz, uint16_t checksum);
/// @brief Get mtu callback type
/// @return return MTU size
typedef uint16_t (*ota_mtu_cb_t)(void);

/// @brief OTA notify callback type
/// @param sz size of notify data
/// @param p_data notify data
/// @return 0 if error <BR>1 if success
typedef uint8_t (*ota_notify_cb_t)(uint8_t sz, uint8_t* p_data);

/// @brief OTA firmware write callback type
/// @param offset offset address
/// @param len write buffer size
/// @param p_buf write buffer
/// @return 0 if error <BR>1 if success
typedef uint8_t (*ota_fw_write_cb_t) (uint32_t offset, uint32_t len, uint8_t* p_buf);

/// @brief OTA erase one object callback type
/// @param offset offset
/// @return 0 if error <BR>1 if success
typedef uint8_t (*ota_erase_object_cb_t)(uint32_t offset);

/// @brief OTA disconnect callback type
/// @param reason disconnect reason
typedef void (*ota_disconnect_cb_t)(uint8_t reason);

/// @brief Get OTA information callback type
/// @param info @ref ret_fw_info_get
typedef void (*ota_get_info_cb_t)(struct ret_fw_info_get* info);

/// @brief Paremeter of CMD_FW_OTA_INIT command
struct cmd_fw_set_address
{
    uint16_t sz;        ///< OTA data size
    uint32_t addr;      ///< OTA address
};

/// @brief Paremeter of CMD_FW_UPGRADE command
struct cmd_fw_upgrade
{
    uint32_t sz;            ///< Firmware size
    uint16_t checksum;      ///< Firmware checksum
    uint8_t version[10];    ///< Firmware version
};

/// @brief Paremeter of CMD_FW_OTA_INIT_NEW command
struct cmd_fw_ota_init_new
{
    uint32_t fw_length;         ///< OTA firmware length
    uint8_t ota_setting;        ///< OTA setting
    uint8_t fw_version[10];     ///< Firmware version
};

/// @brief Paremeter of CMD_FW_OBJECT_CREATE command
struct cmd_fw_object_create
{
    uint32_t fw_addr;       ///< Firmware address to erase
    uint32_t object_size;   ///< Object size
};

/// @brief Paremeter of CMD_FW_OTA_DISCONNECT command
struct cmd_fw_ota_disconnect
{
    uint8_t reason;     ///< Disconnect reason
};

/// @brief Command parameter
union cmd_parm
{
    struct cmd_fw_set_address fw_set_address;
    struct cmd_fw_upgrade fw_upgrade;
    struct cmd_fw_ota_init_new fw_ota_init_new;
    struct cmd_fw_object_create fw_object_create;
    struct cmd_fw_ota_disconnect fw_ota_disconnect;
};

/// @brief Return paremeter of CMD_FW_OTA_INIT command
struct ret_fw_ota_init_cmd
{
    uint8_t status;     ///< Status
};

/// @brief Return paremeter of CMD_FW_OTA_INIT_NEW command
struct ret_fw_ota_init_new_cmd
{
    uint8_t status;             ///< Status
    uint8_t new_flow;           ///< Inform OTA app to run new OTA flow
    uint16_t offset;            ///< Current object offset already upgrade to flash
    uint16_t checksum;          ///< Current checksum of data already upgrade to flash
    uint32_t max_object_size;   ///< Max object size
    uint16_t mtu_size;          ///< MTU size
    uint16_t prn_threshold;     ///< Packet Receipt Notification(PRN) threshold
    uint8_t spec_check_result;  ///< Spec check result
};

/// @brief Return paremeter of CMD_FW_UPGRADE command
struct ret_fw_upgrade_cmd
{
    uint8_t status;     ///< Status
};

/// @brief Return paremeter of CMD_FW_GET_INFO command
struct ret_fw_info_get
{
    uint8_t status;         ///< Status
    uint8_t version[5];     ///< Firmware version
    uint16_t checksum;      ///< Firmware checksum
};

/// @brief Return paremeter of OTA notify
struct ret_fw_notify
{
    uint8_t opcode;     ///< OP code
    uint8_t status;     ///< Status
    uint16_t checksum;  ///< Checksum
};

/// @brief Return paremeter
union ret_parm
{
    struct ret_fw_upgrade_cmd fw_upgrade;
    struct ret_fw_info_get fw_info_get;
    struct ret_fw_ota_init_cmd fw_ota_init;
    struct ret_fw_ota_init_new_cmd fw_ota_init_new;
};

/// @brief HCI command parameter
struct hci_cmd
{
    uint8_t opcode;
    union cmd_parm cmdparm;
};

/// @brief Return paremeter of HCI command
struct evt_command_complete
{
    uint8_t opcode;
    union ret_parm ret_param;
};

/// @brief Event parameter
union  evt_parm
{
    struct evt_command_complete evt_command_complete;
};

/// @brief HCI event
struct hci_evt
{
    uint8_t evtcode;            ///< Event code
    uint8_t evtlen;             ///< Event length
    union evt_parm evtparm;     ///< Event parameter
};

/// @brief OTA target selection <BR>
enum ota_process_setting
{
    OTA_MAIN_FW,                ///< Main firmware
    OTA_HELPER_FW,              ///< Helper firmware
    OTA_EXTERNAL_RESOURCE,      ///< External resource
};

/// @brief OTA spec check result <BR>
enum ota_spec_check_result
{
    OTA_SPEC_CHECK_OK = 1,              ///< Spec check ok
    OTA_FW_OUT_OF_BOUNDS = 2,           ///< OTA firmware size out of bound
    OTA_PROCESS_ILLEGAL = 3,            ///< Illegal OTA process
    OTA_RECONNECT = 4,                  ///< Inform OTA app do reconnect
    OTA_FW_IMG_VERSION_ERROR = 5,       ///< Spec check ok
};

/// @brief OTA disconnect reason <BR>
enum ota_disconnect_reason
{
    OTA_CODE_JUMP = 1,          ///< OTA code jump
    OTA_UPDATE_DONE = 2,        ///< OTA update done
    OTA_RESET,                  ///< OTA reset
};

/// @brief OTA firmware information <BR>
struct ota_fw_info
{
    uint8_t     fw_desc[32];        ///< Firmware description
    uint8_t     fw_version[8];      ///< Firmware version
    uint32_t    fw_size;            ///< Firmware size
    uint16_t    fw_checksum;        ///< Firmware checksum
};

#pragma pack()

/// @brief Receive OTA command or data
/// @param p_cmd OTA command or data
/// @param sz OTA command or data size
/// @param force_cmd 0 - command or data<BR>1 - force to be command
void ota_cmd(uint8_t* p_cmd, uint16_t sz, uint8_t force_cmd);

/// @brief OTA response
/// @param p_rsp OTA response data
/// @param p_sz OTA response data size
void ota_rsp(uint8_t* p_rsp, uint8_t* p_sz);

/// @brief Check if OTA initialized or not
/// @return 0 not initialized <BR>1 if initialized
uint8_t ota_init_check(void);

/// @brief Check if OTA started or not
/// @return 0 not started <BR>1 if started
uint8_t ota_check(void);

/// @brief Init OTA scheduled task parameters
void ota_scheduled_init(void);

/// @brief Scheduled run OTA task
void ota_scheduled_task(void);

/// @brief Reset OTA parameters
void ota_reset(void);

/// @brief Put event to event buffer
/// @param p_event_data OTA event data
/// @param event_data_size OTA event data size
/// @param handler @ref app_sched_event_handler_t
/// @return 0 - Fail<BR>1 - Success
uint32_t ota_scheduled_event_put(uint8_t const* p_event_data,
                                 uint16_t event_data_size,
                                 app_sched_event_handler_t handler);

// data
extern ota_spec_check_cb_t      ota_spec_check_cb;
extern ota_upgrade_cb_t         ota_upgrade_cb;
extern ota_mtu_cb_t             ota_mtu_cb;
extern ota_notify_cb_t          ota_notify_cb;
extern ota_fw_write_cb_t        ota_fw_write_cb;
extern ota_erase_object_cb_t    ota_erase_object_cb;
extern ota_disconnect_cb_t      ota_disconnect_cb;
extern ota_get_info_cb_t        ota_get_info_cb;

/// @}

#endif //_OTA_H_

