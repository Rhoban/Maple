#include <stdlib.h>
#include <wirish/wirish.h>
#include <terminal.h>
#include <main.h>
#include <lewan.h>

/**
 * Setup function
 */
void setup()
{
    lewan_init();
    terminal_init(&SerialUSB);
}

/**
 * Loop function
 */
void loop()
{
    terminal_tick();
}

