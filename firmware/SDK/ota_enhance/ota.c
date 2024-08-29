#define  _OTA_C

#include "ota.h"
#include "ota_imp.h"
#include "debug.h"
#include <string.h>


typedef struct
{
    app_sched_event_handler_t handler;          /**< Pointer to event handler to receive the event. */
    uint16_t                  event_data_size;  /**< Size of event data. */
} event_header_t;


uint8_t ota_init = 0;
uint8_t ota_start = 0;

uint8_t ota_spec_check(struct cmd_fw_ota_init_new* p_cmd);
void cmd_fw_write(uint8_t* p_cmd, uint16_t sz);
void ota_cmd_exe(uint8_t* p_cmd, uint16_t sz);
void ota_reset(void);
void ota_restart(void);

static uint8_t _evt_buf[sizeof(struct hci_evt)];
struct hci_evt* pevt = (struct hci_evt*)_evt_buf;


//OTA FW control variables
static uint16_t _mtu = 0;
static uint16_t _prn = 0;
static uint16_t _prn_threshold = 0;
static uint16_t _ota_checksum = 0;
static uint16_t _last_object_checksum = 0;
static uint32_t _last_object_offset = 0;
static uint32_t _fw_length = 0;
static volatile uint32_t _fw_fast_write_size = 0;
static volatile uint32_t _fw_receive_cnt = 0;
static volatile uint32_t _fw_receive_total = 0;
static volatile uint32_t _fw_write_total = 0;
static uint32_t _code_write_offset = 0;

//OTA buffer
static uint32_t _ota_evt_buffer[OTA_BUFFER_SIZE / sizeof(uint32_t)];
static uint8_t _ota_evt_header_length = sizeof(event_header_t);
static uint16_t _ota_evt_data_length = 0;
static uint16_t _ota_evt_total_size = 0;
static uint16_t _ota_evt_cnt = 0;
static volatile uint8_t _ota_evt_start;
static volatile uint8_t _ota_evt_end;
static event_header_t* _ota_evt_header;  //Array for holding the ota event headers
static uint8_t* _ota_evt_data;           //Array for holding the ota event data

//OTA callback
ota_spec_check_cb_t     ota_spec_check_cb;
ota_upgrade_cb_t        ota_upgrade_cb;
ota_mtu_cb_t            ota_mtu_cb;
ota_notify_cb_t         ota_notify_cb;
ota_fw_write_cb_t       ota_fw_write_cb;
ota_erase_object_cb_t   ota_erase_object_cb;
ota_disconnect_cb_t     ota_disconnect_cb;
ota_get_info_cb_t       ota_get_info_cb;


static void _evt_command_complete(uint8_t opcode, uint8_t* ret_parm, uint8_t ret_len)
{
    pevt->evtcode = EVT_COMMAND_COMPLETE;
    pevt->evtlen = ret_len + CMD_COMPLETE_HDR_SZ;
    pevt->evtparm.evt_command_complete.opcode = opcode;

    memcpy((uint8_t*)&pevt->evtparm.evt_command_complete.ret_param, ret_parm, ret_len);
}

static void _checksum(uint16_t* checksum, uint16_t sz, uint8_t* data)
{
    uint32_t idx;

    for (idx = 0; idx < sz; idx++)
        *checksum += (uint16_t)data[idx];
}

static uint8_t _next_index(uint8_t index, uint16_t buf_size)
{
    return (index < (buf_size - 1)) ? (index + 1) : 0;
}

void cmd_fw_ota_init(struct cmd_fw_set_address* p_cmd)
{
    struct ret_fw_ota_init_cmd ret;

    ota_start = 1;
    ota_init = 1;
    ret.status = ERR_COMMAND_SUCCESS;
    _evt_command_complete(CMD_FW_OTA_INIT, (uint8_t*)&ret, sizeof (ret));

    DBGPRINTF(("cmd_fw_ota_init\r\n"));
}

void cmd_fw_write(uint8_t* p_cmd, uint16_t sz)
{
    if (sz > _fw_fast_write_size)
        sz = _fw_fast_write_size;

    ota_fw_write_cb(_code_write_offset, sz, p_cmd);
    //DBG("_code_write_offset = %x, sz = %x\n", _code_write_offset, sz);
    _checksum(&_ota_checksum, sz, p_cmd);
    _code_write_offset += sz;
    _fw_fast_write_size -= sz;
}

void cmd_fw_upgrade(struct cmd_fw_upgrade* p_cmd)
{
    struct ret_fw_notify ret;
    struct ret_fw_upgrade_cmd Ret;

    DBGPRINTF(("cmd_fw_upgrade\r\n"));
    DBG("p_cmd->sz %x p_cmd->checksum %x\r\n", p_cmd->sz, p_cmd->checksum);

    if (ota_upgrade_cb(NULL, p_cmd->version, p_cmd->sz, p_cmd->checksum) == 1)
    {
        Ret.status = ERR_COMMAND_SUCCESS;
        DBGPRINTF(("ERR_COMMAND_SUCCESS\r\n"));
    }
    else
    {
        Ret.status = ERR_COMMAND_UPDATE_FAIL;
        DBGPRINTF(("ERR_COMMAND_UPDATE_FAIL\r\n"));
    }

    ret.opcode = CMD_FW_UPGRADE;
    ret.status = Ret.status;

    while (ota_notify_cb(sizeof(ret), (uint8_t*)&ret) == 0) ;

    _evt_command_complete(CMD_FW_UPGRADE, (uint8_t*)&Ret, sizeof (Ret));

    ota_start = 0;
}

void cmd_fw_info_get()
{
    struct ret_fw_info_get fw_into_ret;

    ota_get_info_cb(&fw_into_ret);

    fw_into_ret.status = ERR_COMMAND_SUCCESS;

    _evt_command_complete(CMD_FW_GET_INFO, (uint8_t*)&fw_into_ret, \
                          sizeof (fw_into_ret));
}

void ota_scheduled_init()
{
    _mtu = ota_mtu_cb();

    _ota_evt_start = 0;
    _ota_evt_end = 0;
    _ota_evt_data_length = (_mtu > (sizeof(struct hci_cmd))) ? _mtu : (sizeof(struct hci_cmd)); //Event data length must be the larger one between mtu and sizeof(struct hci_cmd)
    _ota_evt_total_size = (OTA_BUFFER_SIZE / (_ota_evt_header_length + _ota_evt_data_length)); //Calculate how much event can be stored in buffer
    _ota_evt_header = (void*)_ota_evt_buffer;
    _ota_evt_data = &((uint8_t*)_ota_evt_buffer)[(_ota_evt_total_size * sizeof(event_header_t))];

    _prn_threshold = (uint16_t)(_ota_evt_total_size - 1);

    DBG("_ota_evt_data_length = %d\n", _ota_evt_data_length);
    DBG("OTA_BUFFER_SIZE = %d\n", OTA_BUFFER_SIZE);
    DBG("_ota_evt_total_size = %d\n", _ota_evt_total_size);
    DBG("_prn_threshold %d\r\n", _prn_threshold);
}

void ota_scheduled_task(void)
{
    void* evt_data = NULL;
    uint16_t evt_data_size;
    app_sched_event_handler_t evt_handler;

    CS_IN();

    if (_ota_evt_cnt != 0)
    {
        DBG("evtcnt= %x \r\n", _ota_evt_cnt);
        evt_data = &_ota_evt_data[_ota_evt_start * _ota_evt_data_length];
        evt_data_size = _ota_evt_header[_ota_evt_start].event_data_size;
        evt_handler = _ota_evt_header[_ota_evt_start].handler;

        if (*evt_handler != &ota_cmd_exe)
            DBG("_ota_evt_start %d evt_handler %x \r\n", _ota_evt_start, evt_handler);

        --_ota_evt_cnt;
        _ota_evt_start = _next_index(_ota_evt_start, _ota_evt_total_size);
        //DBG("###_ota_evt_start = %d\n", _ota_evt_start);
    }

    CS_OUT();

    if (evt_data != NULL)
        evt_handler(evt_data, evt_data_size);
}

void cmd_fw_ota_init_new(struct cmd_fw_ota_init_new* p_cmd)
{
    uint8_t spec_check = OTA_SPEC_CHECK_OK;
    struct ret_fw_ota_init_new_cmd ret;

    ota_start = 1;
    ota_init = 1;
    _fw_fast_write_size = 0;

    DBG("p_cmd->fw_length = %x\n", p_cmd->fw_length);

    //Check OTA spec
    spec_check = ota_spec_check_cb(p_cmd);
    DBG("spec_check = %d\n", spec_check);

    if ((spec_check == OTA_SPEC_CHECK_OK) || (spec_check == OTA_FW_IMG_VERSION_ERROR))
        _fw_length = p_cmd->fw_length;

    ota_scheduled_init();
    DBG("mtu = %d\n", _mtu);

    //Inform controller running new ota flow
    ret.new_flow = 1;

    //Read image offset
    ret.offset = _last_object_offset;

    //Read image checksum
    ret.checksum = _last_object_checksum;

    //Read max object size
    ret.max_object_size = MAX_OBJECT_SIZE;

    //Read mtu size
    ret.mtu_size = _mtu;
    //DBG("ret.mtu_size = %x\n", ret.mtu_size);

    ret.prn_threshold = _prn_threshold;

    ret.spec_check_result = spec_check;

    ret.status = ERR_COMMAND_SUCCESS;
    _evt_command_complete(CMD_FW_OTA_INIT_NEW, (uint8_t*)&ret, sizeof (ret));

    DBGPRINTF(("cmd_fw_ota_init_new\r\n"));
}

void cmd_fw_object_create(struct cmd_fw_object_create* p_cmd)
{
    struct ret_fw_notify ret;

    //Set fw address
    _code_write_offset = p_cmd->fw_addr;

    if (_code_write_offset == 0)
        ota_restart();

    //Set object size
    _fw_fast_write_size = p_cmd->object_size;
    _fw_receive_cnt = p_cmd->object_size;

    ret.opcode = CMD_FW_OBJECT_CREATE;

    while (ota_notify_cb(sizeof(ret), (uint8_t*)&ret) == 0) ;

    ota_erase_object_cb(_code_write_offset);

    DBGPRINTF(("cmd_fw_object_create\r\n"));
}

void ota_cmd_exe(uint8_t* p_cmd, uint16_t sz)
{
    struct ret_fw_notify ret;
    struct hci_cmd* pcmd = (struct hci_cmd*)p_cmd;

    if ((_fw_fast_write_size != 0) && (_fw_write_total < _fw_length))
    {
        CS_IN();

        cmd_fw_write(p_cmd, sz);
        _fw_write_total += sz;
        _prn++;
        DBG("prn = %x, sz = %x, _fw_fast_write_size = %x, _fw_write_total = %x\r\n", _prn, sz, _fw_fast_write_size, _fw_write_total);

        //After write element to flash and PRN is full, notify to device
        if ((_prn >= _prn_threshold) || (_fw_fast_write_size == 0) || (_fw_write_total >= _fw_length))
        {
            _prn = 0;

            if (_fw_fast_write_size == 0)
            {
                _last_object_offset += 1;
                DBG("_last_object_offset = %x\n", _last_object_offset);
                _last_object_checksum = _ota_checksum;
            }

            ret.opcode = CMD_FW_WRITE;
            ret.checksum = _ota_checksum;
            DBG("ret.checksum = %x\n", ret.checksum);

            while (ota_notify_cb(sizeof(ret), (uint8_t*)&ret) == 0) ;
        }

        CS_OUT();
        return;
    }

    switch (pcmd->opcode)
    {
        case CMD_FW_UPGRADE:
            DBG("#OTA CMD_FW_UPGRADE \r\n");
            cmd_fw_upgrade(&pcmd->cmdparm.fw_upgrade);
            break;

        case CMD_FW_MCU_RESET:
            DBG("#OTA CMD_FW_MCU_RESET \r\n");
            ota_disconnect_cb(OTA_RESET);
            break;

        case CMD_FW_GET_INFO:
            DBG("OTA CMD_FW_GET_INFO \r\n");
            cmd_fw_info_get();
            break;

        case CMD_FW_OTA_INIT_NEW:
            DBG("#OTA CMD_FW_OTA_INIT_NEW \r\n");
            cmd_fw_ota_init_new(&pcmd->cmdparm.fw_ota_init_new);
            break;

        case CMD_FW_OBJECT_CREATE:
            DBG("#OTA CMD_FW_OBJECT_CREATE \r\n");
            cmd_fw_object_create(&pcmd->cmdparm.fw_object_create);
            break;
    }
}

uint32_t ota_scheduled_event_put(uint8_t const* p_event_data,
                                 uint16_t event_data_size,
                                 app_sched_event_handler_t handler)
{
    uint32_t ret = 0;

    if (event_data_size <= _ota_evt_data_length)
    {
        uint16_t event_index = 0xFFFF;

        if ((_ota_evt_cnt + 1) < _ota_evt_total_size)
            //if (!(_next_index(_ota_evt_end, _ota_evt_total_size) == _ota_evt_start))
        {
            event_index = _ota_evt_end;
            ++_ota_evt_cnt;
            _ota_evt_end = _next_index(_ota_evt_end, _ota_evt_total_size);

            DBG("##_ota_evt_end = %d\n", _ota_evt_end);
        }
        else
            DBG("#####_ota_evt_cnt oversize!!!");

        if (event_index != 0xFFFF)
        {
            DBG("#event_index = %d\n", event_index);
            // NOTE: This can be done outside the critical region since the event consumer will
            //       always be called from the main loop, and will thus never interrupt this code.
            _ota_evt_header[event_index].handler = handler;

            if (event_data_size > 0)
            {
                if (p_event_data != NULL)
                {
                    memcpy(&_ota_evt_data[event_index * _ota_evt_data_length],
                           p_event_data,
                           event_data_size);
                }

                _ota_evt_header[event_index].event_data_size = event_data_size;
            }
            else
                _ota_evt_header[event_index].event_data_size = 0;

            ret = 1;
        }
        else
            ret = 0;
    }
    else
        ret = 0;

    DBG("#_ota_evt_cnt = %d\n", _ota_evt_cnt);

    return ret;
}

void ota_restart(void)
{
    ota_init = 1;
    ota_start = 1;
    _prn = 0;
    _ota_checksum = 0;
    _fw_receive_cnt = 0;
    _fw_receive_total = 0;
    _fw_write_total = 0;
    _fw_fast_write_size = 0;
    _ota_evt_start = 0;
    _ota_evt_end = 0;
    //_ota_evt_total_size = 0;
    _ota_evt_cnt = 0;
    _last_object_checksum = 0;
    _last_object_offset = 0;
}

void ota_reset(void)
{
    ota_init = 0;
    ota_start = 0;
    _prn = 0;
    _ota_checksum = 0;
    _fw_receive_cnt = 0;
    _fw_receive_total = 0;
    _fw_write_total = 0;
    _fw_fast_write_size = 0;
    _ota_evt_start = 0;
    _ota_evt_end = 0;
    _ota_evt_total_size = 0;
    _ota_evt_cnt = 0;
    _last_object_checksum = 0;
    _last_object_offset = 0;
}

void ota_retransmit(void)
{
    ota_init = 0;
    ota_start = 0;
    _prn = 0;
    _ota_checksum = _last_object_checksum;
    _fw_write_total = (_last_object_offset * MAX_OBJECT_SIZE);
    _fw_fast_write_size = 0;
    _fw_receive_cnt = 0;
    _fw_receive_total = _fw_write_total;
    _ota_evt_start = 0;
    _ota_evt_end = 0;
    _ota_evt_total_size = 0;
    _ota_evt_cnt = 0;
}

void ota_cmd(uint8_t* p_cmd, uint16_t sz, uint8_t force_cmd)
{
    struct hci_cmd* pcmd = (struct hci_cmd*)p_cmd;

    if ((force_cmd == 0) && (_fw_receive_cnt != 0) && (_fw_receive_total < _fw_length))
    {
        //Allocate one element size data buffer to receive data
        if (sz <= _mtu)
        {
            CS_IN();

            if ((_ota_evt_cnt + 1) < _ota_evt_total_size)
            {
                _fw_receive_cnt -= sz;
                _fw_receive_total += sz;

                if (!ota_scheduled_event_put(p_cmd, sz, ota_cmd_exe))
                    DBG("ota_scheduled_event_put fail!\n");
            }
            else
            {
                DBG("Data buffer is full!\n");

                while (1) ;

                //return;
            }

            CS_OUT();
        }


        return;
    }

    if (!ota_init)
    {
        if ((pcmd->opcode != CMD_FW_OTA_INIT) && (pcmd->opcode != CMD_FW_OTA_INIT_NEW))
            return;
    }

    switch (pcmd->opcode)
    {
        case CMD_FW_OTA_INIT:
            DBG("OTA CMD_FW_OTA_INIT \r\n");
            DBG("force_cmd = %d, _fw_receive_cnt = %d, _fw_receive_total = %d, _fw_length = %d\n", force_cmd, _fw_receive_cnt, _fw_receive_total, _fw_length);
            cmd_fw_ota_init(&pcmd->cmdparm.fw_set_address);
            break;

        case CMD_FW_GET_INFO:
            DBG("OTA CMD_FW_GET_INFO11 \r\n");
            cmd_fw_info_get();
            break;

        case CMD_FW_OTA_INIT_NEW:

            DBG("OTA CMD_FW_OTA_INIT_NEW \r\n");

            ota_cmd_exe(p_cmd, sz);
            break;

        case CMD_FW_UPGRADE:
            DBG("OTA CMD_FW_UPGRADE \r\n");

            if (!ota_scheduled_event_put(p_cmd, sz, ota_cmd_exe))
                DBG("ota_scheduled_event_put fail!\n");

            break;

        case CMD_FW_MCU_RESET:
            DBG("OTA CMD_FW_MCU_RESET \r\n");

            if (!ota_scheduled_event_put(p_cmd, sz, ota_cmd_exe))
                DBG("ota_scheduled_event_put fail!\n");

            break;

        case CMD_FW_OBJECT_CREATE:
            DBG("OTA CMD_FW_OBJECT_CREATE \r\n");

            if (!ota_scheduled_event_put(p_cmd, sz, ota_cmd_exe))
                DBG("ota_scheduled_event_put fail!\n");

            break;

        case CMD_FW_OTA_RETRANSMIT:
            DBG("OTA CMD_FW_OTA_RETRANSMIT \r\n");

            ota_retransmit();

            break;

        case CMD_FW_OTA_DISCONNECT:
            DBG("OTA CMD_FW_OTA_DISCONNECT\n");

            ota_disconnect_cb(pcmd->cmdparm.fw_ota_disconnect.reason);

            break;

    }
}

void ota_rsp(uint8_t* p_rsp, uint8_t* p_sz)
{
    memcpy(p_rsp, _evt_buf,  pevt->evtlen + 2);

    *p_sz = pevt->evtlen + 2;
}

uint8_t ota_init_check(void)
{
    return ota_init;
}


uint8_t ota_check(void)
{
    return ota_start;
}




