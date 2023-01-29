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
        if (argv[i][strlen(argv[i]) - 1] == '\n')
            argv[i][strlen(argv[i]) - 1] = '\0';
        char *argument = argv[i];
        // Check if elements are quoted.
        size_t current_arg_len = strlen(argv[i]);
        if (current_arg_len > 1
            && (argv[i][0] == '"' && argv[i][current_arg_len - 1] == '"'))
        {
            argv[i][current_arg_len - 1] = '\0';
            argument++;
        }

        // Try all possible expansions.
        char *expansion = tilde_expansion(argument);
        if (expansion)
        {
            xfree(argv[i]);
            argv[i] = expansion;
        }

        if (expansion == NULL)
        {
            expansion = arithmetic_expansion(argument);
            if (expansion)
            {
                xfree(argv[i]);
                argv[i] = expansion;
            }
        }
        if (expansion == NULL)
        {
            expansion = command_substitution(argument);
            if (expansion)
            {
                xfree(argv[i]);
                argv[i] = expansion;
            }
        }
    }
}

int exec_builtins(char **argv)
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
    else if (strcmp(*argv, "true") == 0)
    {
        return 0;
    }
    else if (strcmp(*argv, "false") == 0)
    {
        return 1;
    }
    fflush(stdout);
    return true;
}
