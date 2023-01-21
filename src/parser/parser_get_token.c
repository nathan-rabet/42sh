#include "../include/parser.h"

void eat(struct token_list *tokens, token_t type)
{
     assert(tokens->current_token != NULL);
     if (tokens->current_token->type == type)
         tokens->current_token = tokens->current_token->next;
     else
         parser_grammar_return_error_2(tokens->current_token);
         //fprintf(stderr,"Read token '%i', expected token : %i\n",
                // tokens->current_token->type, type); // EXIT failure

}

token_t look_ahead(struct token_list *tokens)
{
    assert(tokens->current_token != NULL);
    return tokens->current_token->type;
}

void wrong_look_ahead(struct token_list *tokens, char *function)
{
    fprintf(stderr, "No rule match %i, can't generate look ahead in function %s\n",
            tokens->current_token->type, function);
    exit(2);
}