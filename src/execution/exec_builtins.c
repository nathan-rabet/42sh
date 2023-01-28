#include "../include/builtins.h"
#include "../include/execution.h"

int count_argc(char **argv)
{
    int i = 0;
    while (argv[i])
    {
        i++;
    }

    return i;
}

int exec_builtins(char **argv)
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
    else if (strcmp(*argv, "alias") == 0)
    {
        builtin_alias(argv + 1);
        return 0;
    }
    else if (strcmp(*argv, "unalias") == 0)
    {
        builtin_unalias(argv + 1);
        return 0;
    }
    else if (strcmp(*argv, "true") == 0)
        return 0;
    else if (strcmp(*argv, "false") == 0)
        return 1;

    fflush(stdout);
    return true;
}
