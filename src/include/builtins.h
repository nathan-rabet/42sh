#ifndef BUILTINS_H_
#define BUILTINS_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Structure for echo options.
struct echo_options
{
    int flag_n;
    // Interpretation of '\n', '\t' and '\\' is off by default.
    int flag_e;
};

void my_echo(int argc, char **argv);
void my_cd(char **argv);

/**
 * @brief `alias` builtin command
 *
 * @param name The name of the alias
 * @param value The value of the alias
 */
void builtin_alias(const char *name, const char *value);

/**
 * @brief `unalias` builtin command
 *
 * @param name The name of the alias to remove
 */
void builtin_unalias(const char *name);

#endif /* BUILTINS_H */
