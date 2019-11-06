#include <stdint.h>
#ifndef _LEWAN_H
#define _LEWAN_H

#if defined (BOARD_maple_mini)
#define LEWAN_DIRECTION    2
#define LEWAN_DIRECTION_TX LOW
#define LEWAN_DIRECTION_RX HIGH
#define LEWAN_DEVICE       Serial3
#define LEWAN_AVAILABLE
#endif

void lewan_init();
void lewan_set_pos(uint8_t id, uint16_t angle, uint16_t duration);

#endif