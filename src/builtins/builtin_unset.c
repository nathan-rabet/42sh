#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

/**
 * @brief unset the attributes of a variable
 *
 * @param argv -v variable, -f functions, no_options -> unset var
 */
void builtin_unset(char **argv)
{
    if (strcmp(argv[0], "-v") == 0)
    {
        int i = 1;
        while (argv[i] != NULL)
        {
            if (unsetenv(argv[i]) != 0)
            {
                fprintf(stderr,
                        "Error: Failed to unset environment variable '%s'\n",
                        argv[i]);
                exit(1);
            }
            i++;
        }
    }
    else if (strcmp(argv[0], "-v") != 0)
    {
        int i = 0;
        while (argv[i] != NULL)
        {
            if (unsetenv(argv[i]) != 0)
            {
                fprintf(stderr,
                        "Error: Failed to unset environment variable '%s'\n",
                        argv[i]);
                exit(1);
            }
            i++;
        }
    }
}
