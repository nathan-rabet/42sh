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
    AST_PIPE,
};
struct ast_vtable;

struct ast {
    enum ast_type type;
    struct ast_vtable *vtable;
};

struct ast_vtable {
    bool (*run)(struct ast *ast);
    void (*free)(struct ast *ast);
    void (*pretty_print)(struct ast *ast);
};

struct ast_cmd {
    struct ast base;
    char **words; // NULL terminated char* list
};

struct ast *ast_cmd_init(char **word);
bool cmd_run(struct ast *ast);
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
bool if_run(struct ast *ast);
void if_free(struct ast *ast);
void if_pretty_print(struct ast *ast);

struct ast_list {
    struct ast base;
    size_t nb_children;
    struct ast **children;
};

struct ast *ast_list_init(size_t nb_children, struct ast **children);
bool list_run(struct ast *ast);
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

struct ast_and {
    struct ast base;
    struct ast *ast_left;
    struct ast *ast_right;

};

struct ast *ast_and_init(struct ast *ast_left, struct ast *ast_right);
bool and_run(struct ast *ast);
void and_free(struct ast *ast);
void and_pretty_print(struct ast *ast);

struct ast_or {
    struct ast base;
    struct ast *ast_left;
    struct ast *ast_right;

};

struct ast *ast_or_init(struct ast *ast_left, struct ast *ast_right);
bool or_run(struct ast *ast);
void or_free(struct ast *ast);
void or_pretty_print(struct ast *ast);

struct ast_not {
    struct ast base;
    struct ast *ast_not;

};

struct ast *ast_not_init(struct ast *ast_not);
bool not_run(struct ast *ast);
void not_free(struct ast *ast);
void not_pretty_print(struct ast *ast);

struct ast_while {
    struct ast base;
    struct ast *condition;
    struct ast *to_execute;

};

struct ast *ast_while_init(struct ast *condition, struct ast *to_execute);
bool while_run(struct ast *ast);
void while_free(struct ast *ast);
void while_pretty_print(struct ast *ast);

struct ast_until {
    struct ast base;
    struct ast *condition;
    struct ast *to_execute;

};

struct ast *ast_until_init(struct ast *condition, struct ast *to_execute);
bool until_run(struct ast *ast);
void until_free(struct ast *ast);
void until_pretty_print(struct ast *ast);

struct ast_for {
    struct ast base;
    struct ast *word;
    struct ast *to_execute;
};

struct ast *ast_for_init(char *name, struct ast *word, struct ast *to_execute);
bool for_run(struct ast *ast);
void for_free(struct ast *ast);
void for_pretty_print(struct ast *ast);

#endif //AST_H
