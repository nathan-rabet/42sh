#include "../include/ast.h"
#include "../include/execution.h"

int exec_pipeline(struct ast_pipe *ast)
{
    int res = 0;
    int fds[2];
    for (size_t i = 0; i < ast->nb_command; i++)
    {
        if (pipe(fds) == -1)
            return_and_free("Failed to create pipe file descriptor", 1);
        pid_t pid = fork();

        if (pid < 0)
            return_and_free("Failed to fork", 1);

    }
}