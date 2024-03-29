#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>

#include "ast.h"
#include "lexer.h"
#include "xalloc.h"

struct token_list
{
    struct token *current_token;
};

// Get token
void eat(struct token_list *tokens, token_t type);
void wrong_look_ahead(struct token_list *tokens, char *function);
token_t look_ahead(struct token_list *tokens);

// Parser grammar
struct ast *parser_input(struct token_list *tokens);
struct ast *parser_and_or(struct token_list *tokens);
struct ast *parser_list(struct token_list *tokens);
struct ast *parser_pipeline(struct token_list *tokens);
struct ast *parser_command(struct token_list *tokens);
struct ast *parser_shell_command(struct token_list *tokens);
struct ast *parser_simple_command(struct token_list *tokens);
char **parser_element(struct token_list *tokens, char **words);
struct ast *parser_compound_list(struct token_list *tokens);
struct ast *parser_funcdec(struct token_list *tokens);
struct list_redir *parser_redirection(struct token_list *tokens,
                                      struct list_redir *next);

// Parser rule
struct ast *parser_rule_if(struct token_list *tokens);
struct ast *parser_else_clause(struct token_list *tokens);
struct ast *parser_rule_elif(struct token_list *tokens);
struct ast *parser_rule_until(struct token_list *tokens);
struct ast *parser_rule_while(struct token_list *tokens);
struct ast *parser_rule_for(struct token_list *tokens);
struct ast *parser_rule_case(struct token_list *tokens);
struct ast *parser_case_clause(struct token_list *tokens);
struct list_case_item *parser_case_item(struct token_list *tokens);

// Utils
bool is_redirection(enum token_types type);
bool is_shell_command(struct token *token);
bool is_prefix(struct token_list *token);
bool is_name_before_equal(char *name);
bool is_assignment_word(char *name);
bool is_funcdec(struct token_list *tokens);
char *IONumbertype(enum ast_redir_type type);
void parser_grammar_return_error_2(struct token *token);
enum ast_redir_type redirection_type(enum token_types type);

#endif // PARSER_H
