#include "../../include/execution.h"

int exec_cmd(char **argv, int *error)
{
    if (is_builtin(*argv))
        return exec_builtins(argv, error);
    (void) error;
    pid_t pid = fork();
    int status = 0;

    if (pid == 0)
    {
        if (execvp(*argv, argv) == -1)
            fprintf(stderr, "42sh : %s command not found\n", *argv);
    }
    else
    {
        waitpid(pid, &status, 0);
        status = WEXITSTATUS(status);
    }
    return status;
}
