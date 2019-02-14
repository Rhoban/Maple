#ifndef _RHOCK_TICK_H
#define _RHOCK_TICK_H

#define RHOCK_PAGES     10

bool is_rhock_mode();
void rhock_tick();
void rhock_killall();

struct rhock_obj **rhock_get_programs();

#endif
