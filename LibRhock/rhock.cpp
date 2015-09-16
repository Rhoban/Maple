#include <stdint.h>
#include <stdio.h>
#include <terminal.h>
#include <rhock/stream.h>
#include <rhock/vm.h>
#include "rhock.h"

const char rhock_exit[] = "!rhock\r";
const int rhock_exit_len = 7;
int rhock_exit_pos = 0;
bool rhock_mode = false;

bool is_rhock_mode()
{
    return rhock_mode;
}

void rhock_stream_send(uint8_t c)
{
    if (rhock_mode) {
        terminal_io()->write(&c, 1);
    }
}

TERMINAL_COMMAND(rhock, "Enter rhock mode")
{
    rhock_mode = true;
    terminal_disable();
}

/**
 * Main loop
 */
void rhock_tick()
{
    if (rhock_mode) {
        while (terminal_io()->io->available()) {
            char c = terminal_io()->io->read();
            if (rhock_exit[rhock_exit_pos] == c) {
                rhock_exit_pos++;
                if (rhock_exit_pos >= rhock_exit_len) {
                    rhock_mode = false;
                    terminal_enable();
                }
            } else {
                rhock_exit_pos = 0;
            }

            rhock_stream_recv(c);
        }
    }

    rhock_vm_tick();
}
