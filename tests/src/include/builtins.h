#ifndef BUILTINS_H_
#define BUILTINS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Structure for echo options.
struct echo_options
{
    int flag_n;
    // Interpretation of '\n', '\t' and '\\' is off by default.
    int flag_e;
};

void my_echo(int argc, char **argv);
void my_cd(char **argv);

void builtin_export(char *arg);
void builtin_unset(const char *args);

/**
 * @brief `alias` builtin command
 *
 * @param argv The arguments to the alias command
 */
void builtin_alias(char **argv);

/**
 * @brief `unalias` builtin command
 *
 * @param name argv The arguments to the unalias command
 */
void builtin_unalias(char **argv);

#endif /* BUILTINS_H */
