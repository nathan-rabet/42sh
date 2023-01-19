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

enum ast_redir_type redirection_type(enum token_types type)
{
    switch (type) {
        case LESS:
            return REDIR_LESS;
        case GREAT:
            return REDIR_GREAT;
        case DGREAT:
            return REDIR_DGREAT;
        case GREATAND:
            return REDIR_GREATAND;
        case LESSAND:
            return REDIR_LESSAND;
        case CLOBBER:
            return REDIR_CLOBBER;
        case LESSGREAT:
            return REDIR_LESSGREAT;
        default:
            return REDIR_DLESSDASH;
    }
}

bool is_shell_command(struct token *token)
{
    if (strcmp(token->value, "(") == 0)
        return true;
    switch (token->type) {
        case IF:
        case WHILE:
        case UNTIL:
        case FOR:
        case LBRACE:
        case CASE:
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
    if (!(tokens != NULL && tokens->current_token != NULL
        && tokens->current_token->next != NULL && tokens->current_token->next->next != NULL))
        return false;

    if (look_ahead(tokens) == WORD && strcmp(tokens->current_token->next->value, "(") == 0
            && strcmp(tokens->current_token->next->next->value, ")") == 0)
            return true;
    return false;
}

char *IONumbertype(enum ast_redir_type type)
{
    switch (type) {
        case REDIR_LESS:
        case REDIR_LESSGREAT:
            return "0";
        default:
            return "1";
    }
}

void parser_grammar_return_error_2(struct token *token)
{
    xfree_all();
    err(2, "42sh: syntax error near unexpected token '%s'\n", (char *)token->value);
}
