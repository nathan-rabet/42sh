// GRAMMAR
// input =
//list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../../include/parser.h"

struct ast *parser_input(token *current_token)
{
    struct ast *ast = NULL;
    switch (current_token->type)
    {
        case NEWLINE :
            eat(current_token, NEWLINE);
            break;
        case EOF :
            eat(current_token, EOF);
            break;
        default:
            ast = parse_list(current_token);
            switch (current_token->type) {
                case NEWLINE :
                    eat(current_token, NEWLINE);
                    break;
                case EOF :
                    eat(current_token, EOF);
                    break;
                default:
                    wrong_look_ahead(current_token, "parse_input");
            }

    }
}