#ifndef _THROUGHPUT_H_
#define _THROUGHPUT_H_

#include "ARMCM0.h"

typedef enum 
{
    TP_UPLOAD_TEST = 0x01,
    TP_DOWNLOAD_TEST = 0x02,
    TP_DOWNLOAD_RESULT = 0x04,
    TP_BURST_TX = 0x08,
    TP_BURST_END = 0x10,
}tp_evt_type_t;

#define CMD_THROUGHPUT_READY    0xC1
#define CMD_THROUGHPUT_START    0xC2
#define CMD_THROUGHPUT_DATA     0xC3
#define CMD_THROUGHPUT_STOP     0xC4
#define CMD_THROUGHPUT_END      0xC5

extern void tp_evt_task(void);
extern void tp_cmd(uint8_t* p_cmd, uint8_t sz);
extern void tp_data(uint8_t* p_cmd, uint8_t sz);
extern void tp_set_handle(uint8_t cmd_report_idx, uint8_t data_report_idx);
extern uint8_t tp_get_burst_mode_status(void);
extern void tp_tx_burst_mode(uint8_t* p_data, uint8_t sz);
#endif
