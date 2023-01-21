#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "./include/ast.h"
#include "./include/execution.h"
#include "./include/lexer.h"
#include "./include/parser.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int opt;
    opterr = 0; // deletes the error line generated by getopt function as we
                // handle the error on the default case
    bool input_from_stdin = true;
    char *input_string = NULL;
    char input_stdin[BUFFER_SIZE];

    // Parse command-line options
    while ((opt = getopt(argc, argv, "c:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            // Read input from string
            input_from_stdin = false;
            input_string = optarg;
            size_t input_size = strlen(input_string);

            // print stdin input (debug)
            // printf("reading from -c:\n %s", input_string);
            // send the input_string to the lexer & parser
            xalloc_init();
            struct token *input_tokens = get_tokens(input_string, input_size);
            struct ast *ast = parser_input(input_tokens);
            ast->vtable->run(ast);
            // ast->vtable->pretty_print(ast);
            xalloc_deinit();

            break;
        default:
            // Invalid option
            fprintf(stderr, "Error: Invalid option '%c'\n", optopt);
            fprintf(stderr, "Usage: %s [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n",
                    argv[0]);
            exit(2);
        }
    }

    // If no input source was specified, check if a file was given as a
    // positional argument
    if (input_from_stdin)
    {
        if (optind < argc)
        {
            // Read input from file
            input_from_stdin = false;

            // Open file
            const char *filename = argv[optind];
            FILE *fp = fopen(filename, "r");
            if (!fp)
            {
                fprintf(stderr, "Error opening file\n");
                exit(2);
            }

            // Allocate memory for file contents
            char *file_contents = malloc(BUFFER_SIZE);
            if (file_contents == NULL)
            {
                fprintf(stderr, "Error: Failed to allocate memory\n");
                fclose(fp);
                exit(2);
            }
            memset(file_contents, 0, BUFFER_SIZE); // zero the memory

            // Read file contents line by line and store them in the string
            // file_contents
            char *line = NULL;
            size_t len = 0;
            while ((getline(&line, &len, fp)) != -1)
            {
                // Reallocate memory for file_contents
                file_contents = realloc(
                    file_contents, strlen(file_contents) + strlen(line) + 1);
                if (file_contents == NULL)
                {
                    fprintf(stderr, "Error: Failed to reallocate memory\n");
                    free(line);
                    fclose(fp);
                    exit(2);
                }
                strcat(file_contents, line);
            }

            // print from file (debug)
            printf("reading from file:\n %s", file_contents);

            // send the file_contents to the lexer & parser

            // Free memory
            free(line);
            free(file_contents);
            // Close file
            fclose(fp);
        }
    }

    // If no input source was specified, read from standard input
    if (input_from_stdin)
    {
        if (fgets(input_stdin, BUFFER_SIZE, stdin) == NULL)
        {
            fprintf(stderr, "Error: Failed to read input\n");
            exit(2);
        }

        // print stdin input (debug)
        printf("reading from stdin:\n %s", input_stdin);

        // send the input_string to the lexer & parser
        // struct Node *node = parse_simple_command(input_stdin);
    }

    return 0;
}


