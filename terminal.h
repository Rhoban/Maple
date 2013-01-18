#ifndef _TERMINAL_H
#define _TERMINAL_H

#define TERMINAL_BUFFER_SIZE 30

#define TERMINAL_MAX_COMMANDS 16

/**
 * Prototype of a terminal command
 */
typedef void terminal_command_fn(char *parameters);

/**
 * A command definition for the terminal
 */
struct terminal_command
{
    int num;
    char *name;
    char *description;
    terminal_command_fn *command;
};

/**
 * Registers a command
 */
void terminal_register(const struct terminal_command *command);

#define TERMINAL_COMMAND(name, description) terminal_command_fn terminal_command_ ## name; \
    \
    char terminal_command_name_ ## name [] = #name; \
    char terminal_command_description_ ## name [] = description; \
    \
    struct terminal_command terminal_command_definition_ ## name = { \
        123, terminal_command_name_ ## name , terminal_command_description_ ## name , terminal_command_ ## name \
    }; \
    \
    __attribute__((constructor)) \
    void terminal_command_init_ ## name () {  \
        terminal_register(&terminal_command_definition_ ## name ); \
    } \
    \
    void terminal_command_ ## name (char *parameters)

/**
 * Terminal ticking
 */
void terminal_tick();

/**
 * Initializing terminal
 */
void terminal_init();

#endif // _TERMINAL_H
