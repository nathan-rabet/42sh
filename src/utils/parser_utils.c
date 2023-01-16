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
        case FOR:
        case LBRACE:
            return true;
        default:
            return false;
    }
}

bool is_prefix(struct token_list *token)
{
    return is_redirection(token->current_token->type) || is_assignment_word(token->current_token->value);
}

bool is_name_before_equal(char *name)
{
    if (isdigit(name[0]))
        return false;
    for (size_t i = 1; name[i] != '\0' && name[i] != '='; i++)
        if (!isalnum(name[i]) || name[i] == '_')
            return false;
    return true;
}

bool is_assignment_word(char *name)
{
    if (is_name_before_equal(name) && strchr(name, '='))
        return true;
    return false;
}

bool is_funcdec(struct token_list *tokens)
{
    assert(tokens != NULL && tokens->current_token != NULL
        && tokens->current_token->next != NULL && tokens->current_token->next->next != NULL);

    if (look_ahead(tokens) == WORD && strcmp(tokens->current_token->next->value, "(") == 0
            && strcmp(tokens->current_token->next->next->value, ")") == 0)
            return true;
    return false;
}
