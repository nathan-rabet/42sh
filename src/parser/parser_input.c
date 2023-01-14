// GRAMMAR
// input =
//list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../../include/parser.h"

struct ast *parser_input(struct token *list_tokens)
{
    struct ast *ast = NULL;
    struct token_list *tokens = xmalloc(1, sizeof(struct tokens*));
    tokens->current_token = list_tokens;
    switch (tokens->current_token->type)
    {
        case NEWLINE :
            eat(tokens, NEWLINE);
            break;
        case EOF :
            eat(tokens, EOF);
            break;
        default:
            ast = parser_list(tokens);
            //printf("input %i\n", tokens->current_token->type);
            if (tokens->current_token == NULL)
                return ast;
            switch (tokens->current_token->type) {
                case NEWLINE :
                    eat(tokens, NEWLINE);
                    break;
                case EOF :
                    eat(tokens, EOF);
                    break;
                default:
                    wrong_look_ahead(tokens, "parse_input");
            }
    }

    return ast;
}