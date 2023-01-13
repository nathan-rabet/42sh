#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "xalloc.h"
#include <string.h>

struct token_list
{
    struct token *current_token;
};


void eat(struct token_list *tokens, token_t type);
void wrong_look_ahead(struct token_list *tokens, char *function);
token_t look_ahead(struct token_list *tokens);
struct ast *parser_input(struct token *tokens);
struct ast *parser_and_or(struct token_list *tokens);
struct ast *parser_list(struct token_list *tokens);
struct ast *parser_pipeline(struct token_list *tokens);
struct ast *parser_command(struct token_list *tokens);
struct ast *parser_simple_command(struct token_list *tokens);
struct ast *parser_element(struct token_list *tokens, char *first);


#endif //PARSER_H
