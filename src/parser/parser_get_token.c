#include "../../include/parser.h"

void eat(struct token_list *tokens, token_t type)
{
     assert(tokens->current_token != NULL);
     if (tokens->current_token->type == type)
         tokens->current_token = tokens->current_token->next;// = tokens->current_token[1]; // Pass to the next token TODO(clara)
     else
         fprintf(stderr,"Read token '%i', expected token : %i",
                 tokens->current_token->type, type); // EXIT failure
}

token_t look_ahead(struct token_list *tokens)
{
    assert(tokens->current_token != NULL);
    return tokens->current_token[1].type;
}

void wrong_look_ahead(struct token_list *tokens, char *function)
{
    fprintf(stderr, "No rule match %i, can't generate look ahead in function %s\n",
            tokens->current_token->type, function);
}