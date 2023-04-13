#pragma once

typedef enum
{
    IGNORE,
    MASK,
    PENDING,
    HANDLER,
    INVALID
} Command;

/** Gets command enum from string argument */
Command get_command(const char arg[]);

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, const char *argv[]);

/** Functions checks if sent signal is pending in queue */
void check_if_signal_is_pending();