#include "../include/execution.h"

int exec_builtins(char **argv, int *error)
{
    (void)argv;
    (void)error; // if (strcmp(*argv, "echo") exec_echo(argv);
    fflush(stdout);
    return true;
}
