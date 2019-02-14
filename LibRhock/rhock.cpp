#include <stdint.h>
#include <stdio.h>
#include <terminal.h>
#include <rhock/stream.h>
#include <rhock/vm.h>
#include <rhock/memory.h>
#include <rhock/obj.h>
#include <rhock/program.h>
#include <rhock/chain.h>
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

void rhock_killall()
{
    rhock_program_killall();
}

static struct rhock_obj *objects[RHOCK_PAGES+1];

struct rhock_obj **rhock_get_programs()
{
    struct rhock_obj **obj = &objects[RHOCK_PAGES];
    *obj = NULL;

    rhock_memory_addr addr = rhock_vm_get_objs();
    while (addr != RHOCK_LAST) {
        obj--;
        *obj = rhock_get_obj(addr);
        addr = rhock_chain_next(addr);
    }

    return obj;
}

