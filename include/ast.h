#ifndef AST_H
#define AST_H

enum ast_type {
    AST_CMD,
    AST_IF,
    AST_LIST,
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

struct ast_if {
    struct ast base;
    struct ast *condition;
    struct ast *if_body;
    struct ast *else_body;
}

#endif //AST_H
