#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xalloc.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

// TODO(robertnant): need to mark that result of command substitution shall not
// be processed by any tilde expansion, param expand, command subs and arith
// expand.

/**
 * Splits given input into command, options and parameters.
 * Result should be terminated by NULL pointer.
 */
static char **split_command(char *input)
{
    char **argv = xcalloc(1, sizeof(char *));
    int i = 0;

    // Get command name.
    char *token = strtok(input, " ");
    argv[i] = token;
    i++;

    // Get command options and parameters (NULL pointer is added at end).
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        argv = xrealloc(argv, i + 1, sizeof(char *));
        argv[i] = token;
        i++;
    }

    return argv;
}

/**
 * Performs command substitution and saves result as string.
 * NULL returned otherwise.
*/
char *command_substitution(char *input)
{
    if (input == NULL)
        return NULL;

    char *substitution = xcalloc(1, sizeof(char));
    size_t substitution_length = 0;

    // Check if input is command substitution.
    if (strncmp(input, "$(", 2) == 0 || strncmp(input, "`", 1) == 0)
    {
        int fd[2];
        pipe(fd);

        pid_t pid = fork();
        if (pid == -1)
            err(1, "Command substitution: Fork failed");
        if (pid)
        {
            // Parent process.
            close(fd[1]);

            // Read output from child process.
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(fd[0], buffer, BUFFER_SIZE)))
            {
                if (bytes_read == -1)
                    err(1,
                        "Command substitution: failed to read input from child "
                        "process");
                substitution =
                    xrealloc(substitution, substitution_length + bytes_read + 1,
                             sizeof(char));
                memcpy(substitution + substitution_length, buffer, bytes_read);
                substitution_length += bytes_read;
            }

            // Null terminate result.
            if (substitution_length != 0)
                substitution[substitution_length - 1] = '\0';

            // Wait for child process end.
            wait(NULL);
        }
        else
        {
            close(fd[0]);

            // Redirect standard output.
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            // Remove substitution delimiters.
            input[strlen(input) - 1] = '\0';
            if (input[0] == '`')
                input++;
            else
                input += 2;            

            // Split command into command itself and parameters.
            char **argv = split_command(input);
            execvp(argv[0], argv);
            err(1, "Command substitution: Execution failed in child process");
        }
    }

    return substitution;
}