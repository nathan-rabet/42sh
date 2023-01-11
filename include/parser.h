//
// Created by chalu on 1/11/2023.
//

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

void eat(token *current_token, token_t type);
void wrong_look_ahead(token *current_token, char *function);
token_t look_ahead(token *current_token);
struct ast *parser_input(token *current_token);
struct ast *parser_list(token *current_token);
struct ast *parser_and_or(token *current_token);
struct ast *parser_pipeline(token *current_token);
struct ast *parser_command(token *current_token);
struct ast *parser_simple_command(token *current_token);
struct ast *parser_element(token *current_token);


#endif //PARSER_H
