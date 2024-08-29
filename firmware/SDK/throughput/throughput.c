#include "throughput.h"
#include "pxi_par2860_ble_lib.h"
#include "debug.h"
#include "ble.h"
#include <string.h>

#define UPLOAD_TEST_MAX 1024
#define NOTIFY_BUF_MAX 20

static uint32_t _tp_evt = 0;
static uint16_t _receive_length = 0;
static uint16_t _receive_num = 0;
static uint16_t _num_upload_test = 0;
static uint8_t _is_download_start = false;
static uint8_t _is_upload_start = false;
static uint8_t test_buf[NOTIFY_BUF_MAX];
static uint8_t _cmd_handle_idx;
static uint8_t _data_handle_idx;

static uint8_t _is_burst_mode=0;
static uint16_t _burst_checksum_RX=0;
static uint16_t _burst_checksum_TX=0;
static uint16_t _burst_tx_sn=0;
static uint16_t _burst_count_RX=0;
static uint16_t _burst_count_TX=0;

void tp_data(uint8_t* p_cmd, uint8_t sz)
{
    if(_is_download_start)
    {
        _receive_length+=sz;
        _receive_num++;
    }

    if(_is_burst_mode)
    {
        uint8_t i;
        for(i=0;i<sz;i++)
            _burst_checksum_RX += p_cmd[i];
        _burst_count_RX++;
    }

}

void tp_set_evt(uint32_t evt)
{
    CS_IN();
    _tp_evt |= evt;
    CS_OUT();
}

uint8_t tp_check_evt(uint32_t evt)
{
    if ((_tp_evt & evt) == evt)
        return 1;
    else
        return 0;
}

void tp_clr_evt(uint32_t evt)
{
    CS_IN();
    _tp_evt &= (~evt);
    CS_OUT();
}

void _set_test_count(uint8_t* p_buf,uint8_t length)
{
    if((length!=1)&&(length!=2))
        _num_upload_test = UPLOAD_TEST_MAX;
    else
    {
        uint8_t idx;
        _num_upload_test =0;
        for(idx=0;idx<length;idx++)
        {
            _num_upload_test = _num_upload_test<<8;
            _num_upload_test = _num_upload_test + p_buf[idx];
        }
    }
    DBGPRINTF(("%lu \n",_num_upload_test));
}

uint8_t tp_get_burst_mode_status(void)
{
    return _is_burst_mode;
}

void tp_tx_burst_mode(uint8_t* p_data, uint8_t sz)
{
    memset(test_buf,0,sizeof(test_buf));

    test_buf[0]=(uint8_t)(_burst_tx_sn>>8);
    test_buf[1]=(uint8_t)(_burst_tx_sn&0xff);

    if(sz>(NOTIFY_BUF_MAX-2))
    {
        sz = NOTIFY_BUF_MAX-2;
    }
    memcpy(&test_buf[2],p_data,sz);
    tp_set_evt(TP_BURST_TX);
}

void tp_cmd(uint8_t* p_cmd, uint8_t sz)
{

    if(p_cmd[0] == CMD_THROUGHPUT_READY)
    {
        _set_test_count(&p_cmd[1],sz-1);
        memset(test_buf,0xAA,sizeof(test_buf));
        DBGPRINTF(("_UPLOAD \n"));
        tp_set_evt(TP_UPLOAD_TEST);
    }
    else if(p_cmd[0] == CMD_THROUGHPUT_START)
    {
        DBGPRINTF(("_DOWNLOAD \n"));
        tp_set_evt(TP_DOWNLOAD_TEST);
    }
    else if(p_cmd[0] == CMD_THROUGHPUT_STOP)
    {
        DBGPRINTF(("_TEST_STOP (%d %d) \n",_receive_num,_receive_length));
        tp_set_evt(TP_DOWNLOAD_RESULT);
    }

    else if(p_cmd[0] == 0xC6)
    {
        DBGPRINTF(("_Burst_start\n"));
        _burst_checksum_RX=0;
        _burst_checksum_TX=0;
        _is_burst_mode = true;
        _burst_tx_sn = 0;
        _burst_count_RX = 0;
        _burst_count_TX = 0;
    }
    else if(p_cmd[0] == 0xC7)
    {
        DBGPRINTF(("_Burst_stop\n"));
        _is_burst_mode = false;
        tp_set_evt(TP_BURST_END);
    }

}
void tp_set_handle(uint8_t cmd_report_idx, uint8_t data_report_idx)
{
    _cmd_handle_idx = cmd_report_idx;
    _data_handle_idx = data_report_idx;
}

void tp_evt_task(void)
{
    if (tp_check_evt(TP_UPLOAD_TEST))
    {
        static int _send_count = 0;
        static int _ret_upload = 0;

        if (_is_upload_start == false)
        {
            uint8_t test_buf_start[]={CMD_THROUGHPUT_START};
            if (ble_tx_data(_cmd_handle_idx, sizeof(test_buf_start), test_buf_start, BLE_GATT_NOTIFICATION))
            {
                _is_upload_start = true;
                DBGPRINTF(("start test \n"));
            }
        }

        if (_send_count < _num_upload_test)
        {
            if (pxi_gap_s_check_fifo_sz() == 0)
            {
                test_buf[0]=(uint8_t)((_send_count&0xff00)>>8);
                test_buf[1]=(uint8_t)(_send_count&0xff);
                _ret_upload+=ble_tx_data(_data_handle_idx, sizeof(test_buf), test_buf, BLE_GATT_NOTIFICATION);
                _send_count++;
            }
        }
        else
        {
            uint8_t buf[3];
            buf[0] = CMD_THROUGHPUT_STOP;
            buf[1] = (uint8_t)(_ret_upload>>8);
            buf[2] = (uint8_t)(_ret_upload&0xff);

            if (ble_tx_data(_cmd_handle_idx, sizeof(buf), buf, BLE_GATT_NOTIFICATION))
            {
                _is_upload_start = false;
                DBGPRINTF(("end test (%d) \n",_ret_upload));
                _ret_upload = 0;
                _send_count = 0;
                tp_clr_evt(TP_UPLOAD_TEST);
            }
        }
    }
    else if (tp_check_evt(TP_DOWNLOAD_TEST))
    {
        uint8_t buf[1];
        buf[0] = CMD_THROUGHPUT_READY;

        _receive_length = 0;
        _receive_num = 0;
        _is_download_start = true;

        DBGPRINTF(("start test \n"));
        if (ble_tx_data(_cmd_handle_idx, sizeof(buf), buf, BLE_GATT_NOTIFICATION))
            tp_clr_evt(TP_DOWNLOAD_TEST);
    }
    else if (tp_check_evt(TP_DOWNLOAD_RESULT))
    {
        uint8_t buf[5];
        buf[0] = CMD_THROUGHPUT_END;
        buf[1] = (uint8_t)(_receive_num>>8);
        buf[2] = (uint8_t)(_receive_num&0xff);
        buf[3] = (uint8_t)(_receive_length>>8);
        buf[4] = (uint8_t)(_receive_length&0xff);

        _is_download_start = false;

        if (ble_tx_data(_cmd_handle_idx, sizeof(buf), buf, BLE_GATT_NOTIFICATION))
        {
            _receive_length = 0;
            _receive_num = 0;
            tp_clr_evt(TP_DOWNLOAD_RESULT);
        }
    }
    else if (tp_check_evt(TP_BURST_TX))
    {
        if (pxi_gap_s_check_fifo_sz() == 0)
        {
            uint8_t i;
            ble_tx_data(_data_handle_idx, sizeof(test_buf), test_buf, BLE_GATT_NOTIFICATION);
            for(i=0;i<sizeof(test_buf);i++)
                _burst_checksum_TX += test_buf[i];
            _burst_tx_sn++;
            _burst_count_TX++;
            tp_clr_evt(TP_BURST_TX);
        }

    }
    else if (tp_check_evt(TP_BURST_END))
    {
        uint8_t buf[9];
        buf[0] = 0xC8;
        buf[1] = (uint8_t)(_burst_checksum_RX>>8);
        buf[2] = (uint8_t)(_burst_checksum_RX&0xff);
        buf[3] = (uint8_t)(_burst_checksum_TX>>8);
        buf[4] = (uint8_t)(_burst_checksum_TX&0xff);
        buf[5] = (uint8_t)(_burst_count_RX>>8);
        buf[6] = (uint8_t)(_burst_count_RX&0xff);
        buf[7] = (uint8_t)(_burst_count_TX>>8);
        buf[8] = (uint8_t)(_burst_count_TX&0xff);

        if (ble_tx_data(_cmd_handle_idx, sizeof(buf), buf, BLE_GATT_NOTIFICATION))
        {
            tp_clr_evt(TP_BURST_END);
            DBGPRINTF(("BURST RX:%lu %lu TX:%lu %lu\n",_burst_checksum_RX,_burst_count_RX,_burst_checksum_TX,_burst_count_TX));
            _burst_checksum_RX = 0;
            _burst_checksum_TX = 0;
            _burst_count_RX = 0;
            _burst_count_TX = 0;
        }
    }
}
