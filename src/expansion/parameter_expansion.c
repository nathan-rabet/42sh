#include <fnmatch.h>
#include <stdio.h>
#include <string.h>

#include "xalloc.h"

// Returns expansion form of input.
char *get_expansion_form(char *str)
{
    char *delimiters = ":%#";
    while (*str != '\0')
    {
        int i = 0;
        while (delimiters[i] != '\0' && delimiters[i] != *str)
        {
            i++;
        }

        if (delimiters[i] == '\0')
            str++;
        else
            break;
    }

    return *str != '\0' ? str : NULL;
}

// Counts number of digits in a given number.
size_t count_digits(size_t n)
{
    size_t count = 0;
    while (n != 0)
    {
        n /= 10;
        count++;
    }

    return count;
}

/**
 * Returns value of word after expansion/substitution and substitutes
 * parameter if needed. NULL is return in case of no expansion.
 */
char *parameter_expansion(char *input, int is_quoted)
{
    if (input == NULL)
        return NULL;

    // TODO(robertnant): maybe duplicate input before use of strtok.
    // TODO(robertnant): recursively handle expansion using while loop "while
    // token = ${"
    // TODO: duplicate value of parameter in shell when assigned to expansion
    char *parameter = strtok(input, "${}");
    char **parameter_in_shell = get_shell_variable(parameter);
    char *expansion_form = get_expansion_form(input + strlen(parameter));
    char *word = NULL;
    char *expansion = NULL;

    if (expansion_form != NULL)
    {
        switch (expansion_form[0])
        {
        case ':':
            // Case where parameter is tested as null or unset (':').
            word = (expansion_form[1] != '\0') ? expansion_form + 2 : NULL;
            if (word)
            {
                switch (expansion_form[1])
                {
                case '-':
                    expansion =
                        *parameter_in_shell ? *parameter_in_shell : word;
                    break;
                case '=':
                    // TODO (actually update parameter value). Need to free
                    // previous parameter_in_shell val.
                    expansion = *parameter_in_shell
                        ? *parameter_in_shell
                        : (*parameter_in_shell = word);
                    break;
                case '?':
                    expansion =
                        *parameter_in_shell ? *parameter_in_shell : NULL;
                    if (expansion == NULL && word)
                        err(1, "%s", word);
                    else if (expansion == NULL)
                        err(1, "Expansion: parameter is unset or null");
                    break;
                case '+':
                    expansion =
                        (*parameter_in_shell && **parameter_in_shell != '\0')
                        ? word
                        : "";
                    break;
                default:
                    break;
                }
            }
            // TODO(robertnant): possibly handle case when there's no word
            break;
        case '#':
            // TODO(robertnant): get parameter from shell and get length.
            size_t parameter_len = strlen(*parameter_in_shell);
            expansion = xcalloc(count_digits(parameter_len) + 1, sizeof(char));
            break;
        case '%':
            word = (expansion_form[1] != '\0') ? expansion_form + 1 : NULL;
            if (expansion_form[1] != '\0' && expansion_form[1] != '%')
            {
                if (fnmatch(word, parameter, FNM_PERIOD) == 0)
                {
                    expansion =
                        xcalloc(strlen(*parameter_in_shell) - strlen(word) + 1,
                                sizeof(char));
                    strncpy(expansion, *parameter_in_shell,
                            strlen(*parameter_in_shell) - strlen(word));
                }
                else
                {
                    expansion =
                        xcalloc(strlen(*parameter_in_shell) + 1, sizeof(char));
                    strncpy(expansion, *parameter_in_shell,
                            strlen(*parameter_in_shell));
                }
            }
            else if (expansion_form[1] != '\0')
            {
                word++;
                int parameter_len = strlen(*parameter_in_shell);
                expansion = xcalloc(parameter_len + 1, sizeof(char));
                strncpy(expansion, *parameter_in_shell, parameter_len);
                for (int i = parameter_len - 1; i >= 0; i--)
                {
                    expansion[i] = '\0';
                    if (fnmatch(word, word, FNM_PERIOD) == 0)
                        break;
                }
            }
            break;
        default:
            break;
        }
    }

    return expansion;
}