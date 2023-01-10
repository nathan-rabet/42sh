#ifndef AST_H
#define AST_H

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

enum ast_type {
    AST_CMD,
    AST_IF,
    AST_LIST,
    AST_REDIR,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_WHILE,
    AST_UNTIL,
    AST_FOR,
    AST_FUNC,
    AST_CASE,
};
struct ast_vtable;

struct ast {
    enum ast_type type;
    struct ast_vtable *vtable;
};

struct ast_vtable {
    void (*run)(struct ast *ast);
    void (*free)(struct ast *ast);
    void (*pretty_print)(struct ast *ast);
};

struct ast_cmd {
    struct ast base;
    char **words; // NULL terminated char* list
};

struct ast *ast_cmd_init(char **word);
void cmd_run(struct ast *ast);
void cmd_free(struct ast *ast);
void cmd_pretty_print(struct ast *ast);

struct ast_if {
    struct ast base;
    struct ast *condition;
    struct ast *then_body;
    struct ast *else_body;
};

struct ast *ast_if_init(struct ast *condition,
                        struct ast *then_body, struct ast *else_body);
void if_run(struct ast *ast);
void if_free(struct ast *ast);
void if_pretty_print(struct ast *ast);

struct ast_list {
    struct ast base;
    size_t nb_children;
    struct ast **children;
};

struct ast *ast_list_init(size_t nb_children, struct ast **children);
void list_run(struct ast *ast);
void list_free(struct ast *ast);
void list_pretty_print(struct ast *ast);

enum ast_redir_type {
    /// @brief Tokens redirection
        DLESS, // <<
        DGREAT, // >>
        LESSAND, // <&
        GREATAND, // >&
        LESSGREAT, // <>
        DLESSDASH, // <<-

    /// @brief Token clobber
        CLOBBER, // >|
};

struct ast_redir {
    struct ast base;
    enum ast_redir_type type;
    char *IONumber;
    char *target;
};

struct ast *ast_redir_init(enum ast_redir_type type,char *IONumber, char *target);
bool redir_run(struct ast *ast);
void redir_free(struct ast *ast);
void redir_pretty_print(struct ast *ast);

#endif //AST_H
