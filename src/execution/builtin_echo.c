#include "builtins.h"

#include <stdio.h>

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

// Prints elements passed in argv with options given for echo command.
static void print_elements(int elements_count, char **elements,
               struct echo_options *options)
{
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