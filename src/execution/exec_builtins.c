#include "../include/execution.h"
#include "../include/builtins.h"


int count_argc(char **argv)
{
    int i = 0;
    while (argv[i])
    {
        i++;
    }

    return i;
}

int exec_builtins(char **argv, int *error)
{
    if (strcmp(*argv, "echo") == 0)
    {
        int argc = count_argc(argv + 1);
        my_echo(argc, argv + 1);
        return 0;
    }
    else if (strcmp(*argv, "cd") == 0)
    {
        my_cd(argv + 1);
        return 0;
    }
    (void) error; // if (strcmp(*argv, "echo") exec_echo(argv);
    fflush(stdout);
    return true;
}
