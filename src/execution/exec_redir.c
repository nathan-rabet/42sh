#include "../include/ast.h"
#include "../include/execution.h"

void exec_redir(struct list_redir *redir)
{
    int file_fd;

    while (redir != NULL)
    {
        switch (redir->type)
        {
        case REDIR_LESS:
            file_fd =
                open(redir->target, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
            break;
        case REDIR_GREAT:
        case REDIR_CLOBBER:
            file_fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC,
                           S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            break;
        case REDIR_DGREAT:
            file_fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND,
                           S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            break;
        case REDIR_LESSGREAT:
            file_fd = open(redir->target, O_RDWR | O_CREAT | O_TRUNC,
                           S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            break;
        default:
            return_and_free("Error on type", 1);
        }
        if (file_fd == -1)
        {
            if (errno == EACCES)

                return_and_free("Permission denied", 1);
            else
                return_and_free("Could not open file for redirection", 1);
        }
        push_top_dup(file_fd, atoi(redir->IONumber));
        redir = redir->next;
    }
}