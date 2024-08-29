#ifndef _CONSOLE_H_
#define _CONSOLE_H_



#include "stdbool.h"
#include <string.h>
#include <stdlib.h>
#include "ARMCM0.h"


extern char* token[10];

void console_enable_task(bool en);
bool console_push_char(uint8_t ch);
void console_task(void);


#endif //_CONSOLE_H_
