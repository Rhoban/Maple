#ifndef _RHOBAN_WATCHDOG_H
#define _RHOBAN_WATCHDOG_H

typedef enum {
    WATCHDOG_7MS = 0,
    WATCHDOG_14MS = 1,
    WATCHDOG_29MS = 2,
    WATCHDOG_58MS = 3
} WatchDogMode;

void watchdog_start(WatchDogMode mode);

void watchdog_feed();

#endif
