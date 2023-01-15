#include "../../include/parser.h"

bool is_redirection(enum token_types type)
{
    switch (type) {
        case LESS:
        case GREAT:
        case DGREAT:
        case GREATAND:
        case LESSAND:
        case CLOBBER:
        case LESSGREAT:
            return true;
        default:
            return false;
    }
}

bool is_shell_command(struct token *token)
{
    switch (token->type) {
        case IF:
        case WHILE:
        case UNTIL:
            return true;
        default:
            return false;
    }
    return false;
}

bool is_prefix(struct token *token)
{
    return is_redirection(token->type);
}
