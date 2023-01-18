#include "../../include/execution.h"

void redirecting_input();
void redirecting_output();


void exec_redir(struct list_redir *list_redir)
{
    if (list_redir->type == REDIR_LESS )
        redirecting_input();
    else if (list_redir->type == REDIR_GREAT || list_redir->type == REDIR_DGREAT)
        redirecting_output();
}