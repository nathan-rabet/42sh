#include "../include/builtins.h"
#include "../include/execution.h"
#include "../include/expansions.h"

int count_argc(char **argv)
{
    int i = 0;
    while (argv[i])
    {
        i++;
    }

    return i;
}

static void expand_arguments(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        // Try all possible expansions.
        char *expansion = tilde_expansion(argv[i]);
        if (expansion)
        {
            xfree(argv[i]);
            argv[i] = expansion;
        }
        /**
         * if (expansion == NULL)
         * {
         *     expansion = parameter_expansion(argv[i]);
         *     if (expansion)
         *     {
         *         xfree(argv[i]);
         *         argv[i] = expansion;
         *     }
         * }
        */
        if (expansion == NULL)
        {
            expansion = arithmetic_expansion(argv[i]);
            if (expansion)
            {
                xfree(argv[i]);
                argv[i] = expansion;
            }
        }
        if (expansion == NULL)
        {
            expansion = command_substitution(argv[i]);
            if (expansion)
            {
                xfree(argv[i]);
                argv[i] = expansion;
            }
        }
    }
}

int exec_builtins(char **argv, int *error)
{
    // Expand arguments.
    int argc = count_argc(argv + 1);
    expand_arguments(argc, argv + 1);

    if (strcmp(*argv, "echo") == 0)
    {
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

    (void)error; // if (strcmp(*argv, "echo") exec_echo(argv);
    fflush(stdout);
    return true;
}
