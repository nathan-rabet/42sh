#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/xalloc.h"
#include "builtins.h"

/**
 * @brief set the attributes of a variable
 *
 * @param arg char* argument should be : "NAME=VALUE" or "NAME"
 */
void builtin_export(char *arg)
{
    if (strchr(arg, '='))
    {
        char *name = strtok(arg, "=");
        char *value = strtok(NULL, "");

        setenv(name, value, 1);
    }
    else
    {
        char *value = getenv(arg);
        if (value)
        {
            printf("%s=%s\n", arg, value);
        }
    }
}
