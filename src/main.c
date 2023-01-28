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

#define BUFFER_SIZE 4096

int exec_final(struct token *input_tokens)
{
    if (input_tokens == NULL)
        return 0;
    struct ast *ast = NULL;
    int status = 0;
    struct token_list *tokens = xmalloc(1, sizeof(struct tokens *));
    tokens->current_token = input_tokens;

    while (tokens->current_token && (ast = parser_input(tokens)) != NULL)
        status = ast->vtable->run(ast);

    return status;
}

int exec_stdin_noargs()
{
    char input_stdin[BUFFER_SIZE];

    if (fgets(input_stdin, BUFFER_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "Error: Failed to read input\n");
        exit(2);
    }

    if (input_stdin[0] == '\n')
        exit(0);
    size_t input_size = strlen(input_stdin);
    xalloc_init();
    struct token *input_tokens = get_tokens(input_stdin, input_size);
    int status = exec_final(input_tokens);
    if (status != 0)
    {
        xalloc_deinit();
        exit(status);
    }
    xalloc_deinit();
    return status;
}

int exec_stdin()
{
    char input_stdin[BUFFER_SIZE];
    xalloc_init();
    char *stdin_contents = xmalloc(BUFFER_SIZE, sizeof(char));
    size_t contents_size = 0;

    // printf("going stdin :\n");
    while (fgets(input_stdin, BUFFER_SIZE, stdin) != NULL)
    {
        if (input_stdin[0] == '\n')
            continue;

        size_t line_size = strlen(input_stdin);

        // Allocate memory for the input string and concatenate the
        // current line
        stdin_contents = xrealloc(stdin_contents, contents_size + line_size + 1,
                                  sizeof(char));
        memcpy(stdin_contents + contents_size, input_stdin, line_size);
        contents_size += line_size;
        stdin_contents[contents_size] = '\0';
        // printf("stdin :\n%s", stdin_contents);
    }
    struct token *input_tokens = get_tokens(stdin_contents, contents_size);
    int status = exec_final(input_tokens);

    if (status != 0)
    {
        xalloc_deinit();
        exit(status);
    }
    xalloc_deinit();
    return status;
}

int exec_file(char *argv)
{
    const char *filename = argv;
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening file\n");
        exit(2);
    }
    xalloc_init();
    // Allocate memory for file contents
    char *file_contents = xmalloc(BUFFER_SIZE, sizeof(char));
    memset(file_contents, 0, BUFFER_SIZE); // zero the memory

    // Read file contents line by line and store them in the string
    // file_contents
    char *line = NULL;
    size_t len = 0;
    size_t file_contents_size = 0;
    while ((getline(&line, &len, fp)) != -1)
    {
        // Reallocate memory for file_contents
        file_contents = xrealloc(
            file_contents, file_contents_size + strlen(line) + 1, sizeof(char));
        file_contents_size += strlen(line);
        strncat(file_contents, line, strlen(line));
    }
    // send the file_contents to the lexer & parser
    struct token *input_tokens = get_tokens(file_contents, file_contents_size);
    int status = exec_final(input_tokens);

    if (status != 0)
    {
        free(line);
        fclose(fp);
        xalloc_deinit();
        exit(status);
    }
    xalloc_deinit();

    // Free memory
    free(line);
    // Close file
    fclose(fp);

    return status;
}

int exec_argument(char *input_string)
{
    size_t input_size = strlen(input_string);

    // send the input_string to the lexer & parser
    xalloc_init();
    struct token *input_tokens = get_tokens(input_string, input_size);

    /*while (input_tokens != NULL)
    {
        printf("tok : %s\n", input_tokens->value);
        input_tokens = input_tokens->next;
    }*/

    int status = exec_final(input_tokens);

    if (status != 0)
    {
        xalloc_deinit();
        exit(status);
    }
    xalloc_deinit();
    return status;
}

int main(int argc, char *argv[])
{
    int status = 0;
    int opt;

    opterr = 0; // deletes the error line generated by getopt function as we
                // handle the error on the default case
    bool input_from_stdin = true;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "c:a:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            // Read input from string
            input_from_stdin = false;
            if (argv[optind] == NULL)
                return exec_argument(optarg);
            else
            {
                status = exec_argument(optarg);
                int i = optind;
                while (argv[i] != NULL)
                {
                    status = exec_argument(argv[i]);
                    i++;
                }
            }
            return status;
            break;
        case 'a':
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
            input_from_stdin = false;
            int i = optind;
            while (argv[i] != NULL)
            {
                status = exec_file(argv[i]);
                i++;
            }
            return status;
        }
    }
    // If no input source was specified, read from standard input
    if ((isatty(STDIN_FILENO)))
    {
        // stdin from terminal
        return exec_stdin_noargs();
    }
    else
    {
        // stdin from a file
        return exec_stdin();
    }

    return 0;
}
