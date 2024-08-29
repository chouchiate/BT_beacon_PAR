#define  _BEACON_C

#include "beacon.h"
#include "pxi_par2860_ble_lib.h"
#include "debug.h"
#include <string.h>

uint16_t _beacon_major = 0x0001;
uint16_t _beacon_minor = 0xFFFF;

void beacon_cmd(uint8_t* p_cmd, uint8_t sz)
{
    _beacon_major = (p_cmd[0] << 8);    // Major
    _beacon_major |= p_cmd[1];          // Major
    _beacon_minor = (p_cmd[2] << 8);    // Minor
    _beacon_minor |= p_cmd[3];          // Minor
    
    DBG("Major = %x, Minor = %x\n", _beacon_major, _beacon_minor);
    DBG("sz = %x, p_cmd[0] = %x, p_cmd[1] = %x, p_cmd[2] = %x, p_cmd[3] = %x\n", sz, p_cmd[0], p_cmd[1], p_cmd[2], p_cmd[3]);
    
    pxi_gap_s_profile_data_write(0x10, sz, p_cmd);
}

void beacon_rsp(uint8_t* p_rsp, uint8_t* p_sz)
{
    p_rsp[0] = (_beacon_major >> 8);
    p_rsp[1] = (_beacon_major & 0xFF);
    p_rsp[2] = (_beacon_minor >> 8);
    p_rsp[3] = (_beacon_minor & 0xFF);
    
    *p_sz = sizeof(_beacon_major) + sizeof(_beacon_minor);
}
