#include "watchdog.h"
#include <libmaple/rcc.h>
#include <series/rcc.h>
#include <libmaple/iwdg.h>

void watchdog_start(WatchDogMode mode)
{
    rcc_clk_enable(RCC_WWDG);

    WWDG_BASE->CFR |= (mode<<7);
    WWDG_BASE->CR = 0xff;
}

void watchdog_feed()
{
    WWDG_BASE->CR = 0x7f;
}
