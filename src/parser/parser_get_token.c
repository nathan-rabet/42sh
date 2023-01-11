#include "../../include/parser.h"

void eat(token *current_token, token_t type)
{
     assert(current_token != NULL);
     if (current_token->type == type)
         current_token[0] = current_token[1]; // Pass to the next token TODO(clara)
     else
         fprintf(stderr,"Read token '%i', expected token : %i",
                 current_token->type, type); // EXIT failure
}

token_t look_ahead(token *current_token)
{
    assert(current_token != NULL);
    return current_token[1].type;
}

void wrong_look_ahead(token *current_token, char *function)
{
    fprintf(stderr, "No rule match %i, can't generate look ahead in function %s",
            current_token->type, function);
}