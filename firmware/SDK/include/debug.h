#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "ARMCM0.h"
#include "config.h"



#ifdef CONFIG_DEBUG_MSG
    
    #define CONFIG_DEBUG_MSG_USING_LIB
    #define CONFIG_DEBUG_UART0_OUTPUT

    #ifdef CONFIG_DEBUG_UART0_OUTPUT
        #define DBG_UART_ID                                 0
    #else
        #define DBG_UART_ID                                 1
    #endif
    
#endif





#ifdef CONFIG_DEBUG_MSG
    #define DBGPRINTF(s)                                                                dbg_printf s
    #define DBGTO(id, FMT, ARGS...)                     dbg_printf_sel(id, FMT, ##ARGS)
    #define DBG(FMT, ARGS...)                           DBGTO(DBG_UART_ID, FMT, ##ARGS)
    #define DBGHEXDUMP(title,buf,sz)                                        dbg_hexdump(title,buf,sz)
#else
    #define DBGPRINTF(s)
    #define DBGTO(id, FMT, ARGS...)
    #define DBG(FMT, ARGS...)
    #define DBGHEXDUMP(title,buf,sz)
#endif


    extern void dbg_init(void);
    extern void dbg_printf(char* format, ...) ;
    extern void dbg_hexdump(char* title, uint8_t* buf, uint16_t sz);
    extern void dbg_printf_sel(uint8_t id, char* format, ...);



#endif
