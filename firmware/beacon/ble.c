#define  _BLE_C

#include "ble.h"
#include "debug.h"
#include "beacon.h"
#include "Beacon_profile.h"
#include "pxi_par2860_ble_lib.h"
#include <string.h>

struct gap_att_report_handle* g_report;
static void (*evt_callback)(uint8_t evt);

uint8_t ADV_DATA[] = {  0x02, // length
                        0x01, // AD Type: Flags
                        0x06, // LE General Discoverable Mode & BR/EDR Not Supported
                        0x1a, // length
                        0xff, // AD Type: Manufacturer Specific  
                        0x4c, // Manufacturer ID: Apple Inc.
                        0x00, // Manufacturer ID: Apple Inc.
                        0x02, // Type: Beacon
                        0x15, // Length of data: 21bytes
                        0xe2, // UUID: e2c56db5-dffb-48d2-b060-d0f5a71096e0
                        0xc5, // UUID
                        0x6d, // UUID
                        0xb5, // UUID
                        0xdf, // UUID
                        0xfb, // UUID
                        0x48, // UUID
                        0xd2, // UUID
                        0xb0, // UUID
                        0x60, // UUID
                        0xd0, // UUID
                        0xf5, // UUID
                        0xa7, // UUID
                        0x10, // UUID
                        0x96, // UUID
                        0xe0, // UUID
                        0x00, // Major: 0
                        0x00, // Major
                        0x00, // Minor: 0
                        0x00, // Minor
                        0xd0, // TX Power
                     };
uint16_t    ADV_DATA_SZ = sizeof(ADV_DATA);
uint8_t     SCAN_DATA[] = {  0x09,  // length
                             0x09,  // AD Type: Complete local name
                             0x55,  // Text: USBeacon
                             0x53,  // Text: USBeacon
                             0x42,  // Text: USBeacon
                             0x65,  // Text: USBeacon
                             0x61,  // Text: USBeacon
                             0x63,  // Text: USBeacon
                             0x6f,  // Text: USBeacon
                             0x6e,  // Text: USBeacon
                             0x05,  // length
                             0x12,  // AD Type: Slave Connection Interval Range
                             0x0c,  // Minimun: 15
                             0x00,  // Minimun: 15
                             0x0c,  // Maximun: 15
                             0x00,  // Maximun: 15
                             0x02,  // length
                             0x0a,  // AD Type: TX power level
                             0x05,  // 0x05
                          };
uint16_t    SCAN_DATA_SZ = sizeof(SCAN_DATA);

static uint8_t addr_1_base = 0;
static struct gap_ble_addr dev_addr;
static uint8_t device_idx = 0;
static uint8_t currect_addr = 0;
static uint8_t pairing_mode = 0;

static uint8_t update_en = 0;

static uint8_t isConnected = 0;

static void ble_gatt_read(struct gap_att_read_evt evt)
{
    if (evt.uuid == BEACON_READ_WRITE_UUID)
    {
        uint8_t sz = 0;
        uint8_t rsp[32] = {0};

        beacon_rsp(rsp, &sz);
        DBGHEXDUMP("rsp", rsp,sz);

        pxi_gap_s_gatt_read_rsp_set(sz, rsp);
    }
}

static void ble_gatt_write(struct gap_att_write_evt evt)
{
    if (evt.uuid == BEACON_READ_WRITE_UUID)
    {
        uint8_t i;
        
        DBG("evt.uuid = %x, evt.sz = %x\n", evt.uuid, evt.sz);
        
        for (i = 0; i < evt.sz; i++)
        {
            DBG("data[%d] = %x,\n", i, evt.data[i]);
        }
        
        beacon_cmd(evt.data, evt.sz);
    }
}

static void _ble_update_dev_addr(uint8_t inc)
{
    uint8_t tmp1;
    uint8_t tmp2;

    tmp1 = ((addr_1_base & 0x1f) + ((currect_addr + inc) & 0x1f)) & 0x1f;
    tmp2 = (((addr_1_base >> 5) & 0x07) + device_idx) & 0x07;
    dev_addr.addr[1] = tmp1 | (tmp2 << 5);

    pxi_gap_s_ble_address_set(&dev_addr);
}

static void ble_set_adv_param()
{
    struct gap_adv_params adv_params;

    adv_params.type = ADV_IND;
    adv_params.channel = 0x07;      // advertising channel : 37 & 38 & 39
    adv_params.interval = 0x64;     // advertising interval :100 ms (100 * 1ms)
    adv_params.timeout = 0x1e;      // timeout : 30s
    adv_params.policy = 0x00;
    adv_params.hop_interval = 0x03;
    pxi_gap_s_adv_parameters_set(&adv_params);

    ADV_DATA[0x02] = 0x05;
}

static void ble_check_bond_and_set_param()
{
    uint8_t val[4];
    struct gap_bond_dev bonds_device;
    struct gap_adv_params adv_params;

    if (pxi_bm_s_bond_info_get(&bonds_device) == 1)
    {
        //DBGPRINTF(("bond\r\n"));

        if (bonds_device.addr.type == PUBLIC_ADDRESS_TYPE)
        {
            adv_params.type = ADV_DIRECT_IND;

            memcpy(&adv_params.peer_addr, &bonds_device.addr, sizeof(struct gap_ble_addr));
            adv_params.channel = 0x07;
            adv_params.interval = 0x08;
            //adv_params.interval = 0x10;
            adv_params.timeout = 0x03;
            adv_params.policy = 0x00;
            adv_params.hop_interval = 0x03;
            DBGPRINTF(("ADV_DIRECT_IND\r\n"));
        }
        else
        {
            adv_params.type = ADV_IND;
            adv_params.channel = 0x07;
            adv_params.interval = 0x64;
            adv_params.timeout = 0x28;
            adv_params.policy = 0x00;
            adv_params.hop_interval = 0x03;
        }

        ADV_DATA[0x02] = 0x04;
    }
    else
    {
        DBGPRINTF(("no bond\r\n"));
        adv_params.type = ADV_IND;
        adv_params.channel = 0x07;
        adv_params.interval = 0x190;    //250 ms
        adv_params.timeout = 0x1e;      //30s
        adv_params.policy = 0x00;
        adv_params.hop_interval = 0x03;
    }
    
    pxi_gap_s_profile_data_read(0x10, sizeof(val), val);
    
    _beacon_major = ((val[0] << 8) | val[1]);
    _beacon_minor = ((val[2] << 8) | val[3]);
        
    DBG("##_beacon_major = %x\n", _beacon_major);
    DBG("##_beacon_minor = %x\n", _beacon_minor);
    
    ADV_DATA[25] = (_beacon_major >> 8) & 0xFF;
    ADV_DATA[26] = (_beacon_major & 0xFF);
    ADV_DATA[27] = (_beacon_minor >> 8) & 0xFF;
    ADV_DATA[28] = (_beacon_minor & 0xFF);
    memcpy(&SCAN_DATA[2], &dev_name, sizeof(dev_name));

    pxi_gap_s_adv_parameters_set(&adv_params);
    pxi_gap_s_adv_data_set(ADV_DATA, ADV_DATA_SZ, SCAN_DATA, SCAN_DATA_SZ);
}

static void GAP_Evt_Callback(struct gap_ble_evt* p_evt)
{
    if (p_evt->evt_code == GAP_EVT_ADV_END)
    {
        DBGPRINTF(("ble: advend\r\n"));

        (*evt_callback)(BLE_ADV_END);

    }
    else if (p_evt->evt_code == GAP_EVT_CONNECTED)
    {
        struct gap_update_params update;

        DBGPRINTF(("ble: connection\r\n"));

        isConnected = 1;

        update_en = 0;
        
        update.updateitv_min = CONNECTION_INTERVAL_MIN;
        update.updateitv_max = CONNECTION_INTERVAL_MAX;
        update.updatelatency = CONNECTION_LATENCY;
        update.updatesvto = CONNECTION_SVTO;
        DBG(">pxi_gap_s_connection_update\r\n");
        pxi_gap_s_connection_update(&update);
        DBG("<pxi_gap_s_connection_update\r\n");
        update_en = 1;
        
        //DBGPRINTF(("ble: GAP_EVT_ATT_HANDLE_CONFIGURE\r\n"));

        (*evt_callback)(BLE_CONNECTED);
    }
    else if (p_evt->evt_code == GAP_EVT_DISCONNECTED)
    {
        DBGPRINTF(("ble: GAP_EVT_DISCONNECTED(%02x)\r\n", p_evt->evt.disconn_evt.reason));

        isConnected = 0;

        //latency_mode = 0;

        (*evt_callback)(BLE_DISCONNECTED);
    }
    else if (p_evt->evt_code == GAP_EVT_ATT_HANDLE_CONFIGURE)
    {
        if (pairing_mode == 1)
        {
            pairing_mode = 0 ;

            currect_addr++;

            pxi_gap_s_profile_data_write(2 + device_idx, 1, &currect_addr);
        }

        //DBGPRINTF(("ble: GAP_EVT_ATT_HANDLE_CONFIGURE\r\n"));

        (*evt_callback)(BLE_NOTIFY_ENABLE);

        DBG("hdl %x\r\n", p_evt->evt.att_handle_config_evt.hdl);
        DBG("value %x\r\n", p_evt->evt.att_handle_config_evt.value);
        DBG("uuid %x\r\n", p_evt->evt.att_handle_config_evt.uuid);

    }
    else if (p_evt->evt_code == GAP_EVT_ENC_START)
    {
        struct gap_update_params update;


        update.updateitv_min = CONNECTION_INTERVAL_MIN;
        update.updateitv_max = CONNECTION_INTERVAL_MAX;
        update.updatelatency = CONNECTION_LATENCY;
        update.updatesvto = CONNECTION_SVTO;
        DBG(">pxi_gap_s_connection_update\r\n");
        pxi_gap_s_connection_update(&update);
        DBG("<pxi_gap_s_connection_update\r\n");
        update_en = 1;

        (*evt_callback)(BLE_START_ENC);
    }
    else if (p_evt->evt_code == GAP_EVT_L2CAP_UPDATE_RSP)
    {
        if ((p_evt->evt.l2cap_update_rsp_evt.result == 0x0001) && (update_en == 1))
        {

            struct gap_update_params update;
            DBG("L2cap update reject\r\n");
            update.updateitv_min = SECOND_CONNECTION_INTERVAL_MIN;
            update.updateitv_max = SECOND_CONNECTION_INTERVAL_MAX;
            update.updatelatency = SECOND_CONNECTION_LATENCY;
            update.updatesvto = SECOND_CONNECTION_SVTO;
            pxi_gap_s_connection_update(&update);

            update_en = 2;

        }
    }
    else if (p_evt->evt_code == GAP_EVT_ATT_READ)
        ble_gatt_read(p_evt->evt.att_read_evt);

    else if (p_evt->evt_code == GAP_EVT_ATT_WRITE)
        ble_gatt_write(p_evt->evt.att_write_evt);

    else if (p_evt->evt_code == GAP_EVT_PAIRING_COMP)
    {
        DBG("GAP_EVT_PAIRING_COMP\r\n");
        pxi_bm_s_bond_manager_idx_set(5);
        pxi_bm_s_bond_info_save();
    }

}

void ble_adv_start(uint8_t pairing_en)
{
    pairing_mode = pairing_en;

    if (pairing_en == 1)
    {
        _ble_update_dev_addr(1);

        ble_set_adv_param();
    }
    else
        ble_check_bond_and_set_param();

    DBGPRINTF((">>>pxi_gap_s_adv_start\r\n"));
    pxi_gap_s_adv_start();
    DBGPRINTF(("<<<pxi_gap_s_adv_start\r\n"));
}

uint8_t ble_check_notify_enable(uint8_t report_idx)
{
    //DBG("g_report->report[%x].value = %x\r\n" ,report_idx,g_report->report[report_idx].value);

    return g_report->report[report_idx].value;
}

uint8_t ble_is_connected(void)
{
    return isConnected;
}

void ble_disconnect(void)
{
    pxi_gap_s_disconnect();
}

uint8_t ble_tx_data(uint8_t report_idx, uint8_t sz, uint8_t* p_data, uint8_t send_type)
{
    uint8_t ret = 0 ;

    if (g_report->report[report_idx].value == 0)
        return 0;

    __disable_irq();

    if (pxi_gap_s_check_fifo_sz() == 0)
        ret = pxi_gap_s_gatt_data_send(send_type, &g_report->report[report_idx], sz,  p_data);

    __enable_irq();

    return ret;
}

uint8_t ble_notify_data(uint8_t report_idx, uint8_t sz, uint8_t* p_data)
{
    uint8_t ret = 0 ;

    if (g_report->report[report_idx].value == 0)
        return 0;

    CS_IN();

    if (pxi_gap_s_check_fifo_sz() == 0)
        ret = pxi_gap_s_gatt_data_send(BLE_GATT_NOTIFICATION, &g_report->report[report_idx], sz,  p_data);

    CS_OUT();

    return ret;
}

void ble_slave_latency_en(uint8_t en)
{
    //latency_mode = en;
    DBG("ble_slave_latency_en %x\r\n", en);
    pxi_gap_s_connection_latency_mode(en);
}

void ble_task()
{
    pxi_ble_sched_execute();
}
void ble_init(void*   p_callback)
{
    struct gap_evt_callback evt;
    struct smp_pairing_req sec_para;
    struct gatt_profile_struct gatt;

    evt_callback = ((void (*)(uint8_t))p_callback);

    gatt.report_handle_address = (uint32_t)_gatt_database_report_handle;
    gatt.primary_address = (uint32_t)_gatt_database_primary;
    gatt.include_address = (uint32_t)_gatt_database_include;
    gatt.characteristic_address = (uint32_t)_gatt_database_characteristic;
    gatt.value_address = (uint32_t)_gatt_database_value;
    DBG(">GAPProfile\r\n");
    pxi_gap_s_gatt_profiles_set(&gatt);
    DBG("<GAPProfile\r\n");


    //pxi_ble_config_multiple_bonding(1);
    pxi_ble_set_max_bonding(10);
    DBGPRINTF((">BleInit\r\n"));
    pxi_gap_s_ble_init();
    DBGPRINTF(("<BleInit\r\n"));


    sec_para.io = IO_NO_INPUT_OUTPUT;
    sec_para.mitm = 1;
    sec_para.flags = AUTHREQ_BONDING;
    sec_para.oob = OOB_AUTH_NOT_PRESENT;
    sec_para.max_enc_sz = 0x10;
    sec_para.init_key = 0x00;
    sec_para.rsp_key = (SMP_KEY_MASTER_IDEN | SMP_KEY_ADDR_INFO);
    DBGPRINTF((">pxi_gap_s_security_parameters_set\r\n"));
    pxi_gap_s_security_parameters_set(&sec_para);
    DBGPRINTF(("<pxi_gap_s_security_parameters_set\r\n"));

    evt.evt_mask = GAP_EVT_CONNECTION_SLEEP | GAP_EVT_CONNECTION_INTERVAL;
    evt.p_callback = &GAP_Evt_Callback;
    pxi_gap_s_evt_handler_set(&evt);

    DBGPRINTF((">pxi_gap_s_gatt_report_handle_get\r\n"));
    pxi_gap_s_gatt_report_handle_get(&g_report);
    DBGPRINTF(("<pxi_gap_s_gatt_report_handle_get\r\n"));
    DBG("g_report->cnt %x\r\n", g_report->cnt);


    //DBGHEXDUMP("g_report->report",(uint8_t *)g_report->report,sizeof(g_report->report));

    pxi_gap_s_profile_data_read(1, 1, &device_idx);
    DBGPRINTF(("dev_idx(%02x)\r\n", device_idx));

    if (device_idx > 3)
    {
        device_idx = 0;

        pxi_gap_s_profile_data_write(1, 1, &device_idx);
    }

    pxi_gap_s_profile_data_read(2 + device_idx, 1, &currect_addr);
    DBGPRINTF(("currect_addr(%02x)\r\n", currect_addr));

    pxi_gap_s_ble_address_get(&dev_addr);
    DBGPRINTF(("addr type %02x \r\n", dev_addr.type));
    DBGHEXDUMP("addr", dev_addr.addr, 6);

    addr_1_base = dev_addr.addr[1];

    _ble_update_dev_addr(0);

    pxi_bm_s_bond_manager_idx_set(device_idx);

    //pxi_gap_s_ble_address_set(&dev_addr);
    DBGPRINTF(("addr type %02x \r\n", dev_addr.type));
    DBGHEXDUMP("addr", dev_addr.addr, 6);


}

