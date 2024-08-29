#include "debug.h"
#include "uart.h"
#include <stdarg.h>
#include <stdio.h>



#define MAX_FORMAT_BUFFER_SIZE	(128)
static uint8_t s_formatBuffer[MAX_FORMAT_BUFFER_SIZE];


void dbg_init(void)
{
#ifdef CONFIG_DEBUG_UART0_OUTPUT
    uart_0_init(UART_RTS_CTS_DISABLE, UART_BAUD_115200);
#else
    uart_1_init(UART_RTS_CTS_DISABLE, UART_BAUD_115200);
#endif
}

void dbg_printf(char *format,...)
{

    int32_t iWriteNum = 0;
    va_list  ap;

    if(!format)
        return;

    va_start(ap,format);

    iWriteNum = vsnprintf((char *)s_formatBuffer,MAX_FORMAT_BUFFER_SIZE,format,ap);

    va_end(ap);

    if(iWriteNum < 0)
        return;

    if(iWriteNum > MAX_FORMAT_BUFFER_SIZE)
    {
        iWriteNum = MAX_FORMAT_BUFFER_SIZE;
    }

#ifdef CONFIG_DEBUG_UART0_OUTPUT
    uart_0_write(s_formatBuffer, iWriteNum);
#else
    uart_1_write(s_formatBuffer, iWriteNum);
#endif

}

void dbg_hexdump(char *title, uint8_t *buf, uint16_t sz)
{
    int i = 0;

    if (title)
        dbg_printf((title));

    for(i = 0; i < sz; i++)
    {
        if((i%8) == 0)
            dbg_printf("[%04x] ",(uint16_t)i);

        dbg_printf("%02x ",(uint16_t)buf[i]);

        if(((i+1)%8) == 0)
            dbg_printf("\r\n");

    }

    if((i%8) != 0)
        dbg_printf("\r\n");

}

void dbg_printf_sel(uint8_t id, char *format, ...)
{

    uint8_t iWriteNum = 0;
    va_list  ap;

    if(!format)
        return;

    va_start(ap,format);

    iWriteNum = vsprintf((char *)s_formatBuffer,format,ap);

    va_end(ap);


    if(iWriteNum > MAX_FORMAT_BUFFER_SIZE)
        iWriteNum = MAX_FORMAT_BUFFER_SIZE;

    if(id == 0) {

        uart_0_write(s_formatBuffer, iWriteNum);

    } else {

        uart_1_write(s_formatBuffer, iWriteNum);
    }


}


