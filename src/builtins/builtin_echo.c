#include <stdio.h>
#include <string.h>

#include "builtins.h"

/**
 * Prints given character. If an escape sequence is passed, it is printer as a
 * string and is not interpreted.
 */
static void non_interpreted_print(char *str)
{
    while (*str)
    {
        switch (*str)
        {
        case '\n':
            putchar('\\');
            putchar('n');
            break;
        case '\t':
            putchar('\\');
            putchar('t');
            break;
        case '\\':
            putchar('\\');
            putchar('\\');
            break;
        default:
            putchar(*str);
        }
        str++;
    }
}

// Strips leading and trailing quotes from strings to be printed.
char *strip_quotes(char *str)
{
    size_t i = 0;
    size_t j = strlen(str);
    char quote;

    // Strip quotes if string is not empty.
    if (j != 0 && (str[0] == '"' || str[0] == '\''))
    {
        if (str[0] == '"')
            quote = '"';
        else if (str[0] == '\'')
            quote = '\'';

        while (str[i] != '\0' && (str[i] == quote && str[j - 1] == quote))
        {
            i++;
            str[j - 1] = '\0';
            j--;
        }
    }

    return str + i;
}

// Prints elements passed in argv with options given for echo command.
static void print_elements(int elements_count, char **elements,
                           struct echo_options *options)
{
    // Strip leading or trailing quotes.
    for (int i = 0; i < elements_count; i++)
    {
        elements[i] = strip_quotes(elements[i]);
    }

    if (options->flag_e)
    {
        for (int i = 0; i < elements_count - 1; i++)
        {
            printf("%s ", elements[i]);
        }
        if (elements_count - 1 >= 0)
            printf("%s", elements[elements_count - 1]);
    }
    else
    {
        for (int i = 0; i < elements_count - 1; i++)
        {
            non_interpreted_print(elements[i]);
            printf("%s", " ");
        }
        if (elements_count - 1 >= 0)
            non_interpreted_print(elements[elements_count - 1]);
    }

    // Prints newline if should not be omitted according to options.
    if (!options->flag_n)
        printf("%s", "\n");
}

// Builtin echo command.
void my_echo(int argc, char **argv)
{
    // Parse echo options.
    struct echo_options echo_options;
    echo_options.flag_e = 0;
    echo_options.flag_n = 0;

    int i = 0;
    while (i < argc && argv[i][0] == '-')
    {
        int j = 1;
        while (argv[i][j] != '\0')
        {
            if (argv[i][j] == 'n')
            {
                echo_options.flag_n = 1;
            }
            else if (argv[i][j] == 'e')
            {
                echo_options.flag_e = 1;
            }
            else if (argv[i][j] == 'E')
            {
                echo_options.flag_e = 0;
            }

            j++;
        }

        i++;
    }

    // Print elements.
    print_elements(argc - i, argv + i, &echo_options);
}
