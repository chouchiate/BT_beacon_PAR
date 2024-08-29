#ifndef _BEACON_H_
#define _BEACON_H_

#include "ARMCM0.h"

#define dev_name  "USBeacon"

#ifdef _BEACON_C
#else
    extern uint16_t _beacon_major;
    extern uint16_t _beacon_minor;
    extern void beacon_cmd(uint8_t* p_cmd, uint8_t sz);
    extern void beacon_rsp(uint8_t* p_rsp, uint8_t* p_sz);
#endif
#endif

