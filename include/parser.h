//
// Created by chalu on 1/11/2023.
//

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "xalloc.h"
#include <string.h>

struct token *eat(struct token *current_token, token_t type);
void wrong_look_ahead(struct token *current_token, char *function);
token_t look_ahead(struct token *current_token);
struct ast *parser_input(struct token *current_token);
struct ast *parser_and_or(struct token *current_token);
struct ast *parser_list(struct token *current_token);
struct ast *parser_pipeline(struct token *current_token);
struct ast *parser_command(struct token *current_token);
struct ast *parser_simple_command(struct token *current_token);
struct ast *parser_element(struct token *current_token, char *first);


#endif //PARSER_H
