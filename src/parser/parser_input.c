// GRAMMAR
// input =
//list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../../include/parser.h"

struct ast *parser_input(struct token *current_token)
{
    struct ast *ast = NULL;
    switch (current_token->type)
    {
        case NEWLINE :
            current_token = eat(current_token, NEWLINE);
            break;
        case EOF :
            current_token = eat(current_token, EOF);
            break;
        default:
            ast = parser_list(current_token);
            switch (current_token->type) {
                case NEWLINE :
                    current_token = eat(current_token, NEWLINE);
                    break;
                case EOF :
                    current_token = eat(current_token, EOF);
                    break;
                default:
                    wrong_look_ahead(current_token, "parse_input");
            }

    }
    return ast;
}