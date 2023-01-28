#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

enum UNSET_OPT
{
    UNSET_VAR,
    UNSET_FUNC,
    UNSET_NONE
};

void builtin_unset(const char *args)
{
    char *arg_copy = strdup(args);
    char *arg = strtok(arg_copy, " ");
    enum UNSET_OPT opt = UNSET_NONE;
    while (arg != NULL)
    {
        if (strcmp(arg, "-v") == 0)
        {
            opt = UNSET_VAR;
        }
        else if (strcmp(arg, "-f") == 0)
        {
            opt = UNSET_FUNC;
        }
        else
        {
            if (opt == UNSET_VAR)
            {
                unsetenv(arg);
            }
            else if (opt == UNSET_FUNC)
            {
                // TODO : handle function
            }
            else
            {
                unsetenv(arg);
            }
        }
        arg = strtok(NULL, " ");
    }
    free(arg_copy);
}