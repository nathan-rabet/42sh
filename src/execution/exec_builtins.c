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
    else if (strcmp(*argv, "export") == 0)
    {
        builtin_export(argv[1]);
        return 0;
    }
    else if (strcmp(*argv, "unset") == 0)
    {
        builtin_unset(argv + 1);
        return 0;
    }
        else if (strcmp(*argv, ".") == 0)
    {
        return builtin_dot(argv + 1);
        return 0;
    }

    (void)error; // if (strcmp(*argv, "echo") exec_echo(argv);
    fflush(stdout);
    return true;
}
