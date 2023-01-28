#include <err.h>
#include <stdio.h>
#include <string.h>
#include <xalloc.h>

#include "evalexpr.h"

// Performs arithmetic expansion. NULL is returned if no expansion is possible.
char *arithmetic_expansion(char *input)
{
    if (input == NULL)
        return NULL;

    // Check if input is arithmetic expansion.
    if (strncmp(input, "$((", 3) == 0)
    {
        // NULL terminate arithmetic expansion end delimiters and get expansion
        // result.
        size_t input_len = strlen(input);
        if (input[input_len - 1] == ')' && input[input_len - 2] == ')')
        {
            input[input_len - 1] = '\0';
            input[input_len - 2] = '\0';

            struct fifo *expr = NULL;
            int error;

            if ((error = evalexpr_lexer(input + 3, 0, &expr)) != 0)
                err(1, "Arithmetic expansion: error while lexing");

            int result = evalexpr_infix(expr, &error);

            // Check for arithmetic error.
            if (error)
                err(1, "Arithmetic expansion: arithmetic error found");

            // Allocate enough space to hold an int.
            char *arithmetic_result = xcalloc(12, sizeof(char));
            snprintf(arithmetic_result, 12, "%i", result);

            return arithmetic_result;
        }
    }

    return NULL;
}