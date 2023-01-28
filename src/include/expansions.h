#ifndef EXPANSIONS_H_
#define EXPANSIONS_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Performs arithmetic expansion. NULL is returned if no expansion is possible.
char *arithmetic_expansion(char *input);

/**
 * Performs command substitution and saves result as string.
 * NULL returned otherwise.
*/
char *command_substitution(char *input);

// Generates tilde expansion. Returns NULL if no expansion is possible.
char *tilde_expansion(char *input);

#endif /* !EXPANSIONS_H_ */