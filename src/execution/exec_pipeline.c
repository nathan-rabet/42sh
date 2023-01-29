#include "../include/ast.h"
#include "../include/execution.h"

int exec_pipeline(struct ast_pipe *ast)
{
    int res = 0;
    int error;
    int fds[2];
    int fd_in = STDIN_FILENO;
    int fd_out = STDOUT_FILENO;

    for (size_t i = 0; i < ast->nb_command; i++)
    {
        if (pipe(fds) == -1)
            return_and_free("Failed to create pipe file descriptor", 1);

        push_top_dup(fds[1], STDOUT_FILENO);
        push_top_dup(fd_in, STDIN_FILENO);

        struct ast_cmd *cmd = (struct ast_cmd *)ast->command[i];
        res = exec_cmd(cmd->words, &error);
    }
}
