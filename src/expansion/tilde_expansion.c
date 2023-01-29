#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xalloc.h>

// Generates tilde expansion. Returns NULL if no expansion is possible.
char *tilde_expansion(char *input)
{
    if (input == NULL)
        return NULL;

    char *expanded_path = NULL;
    if (input[0] == '~')
    {
        if (input[1] == '/' || input[1] == '\0')
        {
            char *home = getenv("HOME");
            size_t expanded_path_len = strlen(home) + strlen(input + 1);
            expanded_path = xcalloc(expanded_path_len + 1, sizeof(char));
            snprintf(expanded_path, expanded_path_len + 1, "%s%s", home,
                     input + 1);
        }
        else
        {
            // Case for ~username expansion.

            // Get username.
            char *user = input + 1;
            char *slash = strchr(user, '/');
            if (slash)
                *slash = '\0';

            // Check if user exists.
            if (getpwnam(user))
            {
                char *prefix = "/home/";
                size_t expanded_path_len = strlen(prefix) + strlen(input + 1);
                expanded_path = xcalloc(expanded_path_len + 1, sizeof(char));
                snprintf(expanded_path, expanded_path_len + 1, "%s%s", prefix,
                         input + 1);
            }
            else
            {
                err(1, "Tilde expansion: given user not found");
            }
        }
    }

    return expanded_path;
}