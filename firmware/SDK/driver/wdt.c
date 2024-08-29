#include "wdt.h"

static WDT_CTRL_TYPE* _wdt_ctrl = ((WDT_CTRL_TYPE*) WDT_BASE);

void wdt_init(uint16_t count)
{

    _wdt_ctrl->WDT_CLR = 1;
    _wdt_ctrl->WDT_TIME = count;
}

void wdt_clear()
{
    _wdt_ctrl->WDT_CLR = 1;
}

void wdt_start(void)
{
    _wdt_ctrl->WDT_EN = 1;
}

void wdt_stop(void)
{
    _wdt_ctrl->WDT_EN = 0;
}


