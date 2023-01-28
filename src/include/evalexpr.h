#ifndef EVAL_EXPR_H
#define EVAL_EXPR_H

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

// Buffer size and ascii values of operators.
#define BUFFER_SIZE 2565
#define ASCII_MOD 37
#define ASCII_MUL 42
#define ASCII_PLUS 43
#define ASCII_MINUS 45
#define ASCII_DIV 47
#define ASCII_PARL 40
#define ASCII_PARR 41
#define ASCII_POW 94

// Stack.
struct stack
{
    int data;
    struct stack *next;
};

struct stack *stack_push(struct stack *s, int e);
struct stack *stack_pop(struct stack *s);
int stack_peek(struct stack *s);
void stack_destroy(struct stack *s);

// Queue.
struct list
{
    int data;
    char token;
    struct list *next;
};

struct fifo
{
    struct list *head;
    struct list *tail;
    size_t size;
};

struct fifo *fifo_init(void);
size_t fifo_size(struct fifo *fifo);
void fifo_push(struct fifo *fifo, int elt, char token);
int fifo_head(struct fifo *fifo);
void fifo_pop(struct fifo *fifo);
void fifo_clear(struct fifo *fifo);
void fifo_destroy(struct fifo *fifo);
void fifo_print(const struct fifo *fifo);

// Operations.
int add(int lhs, int rhs);
int sub(int lhs, int rhs);
int division(int lhs, int rhs);
int mod(int lhs, int rhs);
int expo(int lhs, int rhs);
int calculate(char operator, int lhs, int rhs);

// Evaluation.
int evalexpr_rpn(struct fifo *expr, int *err);
int evalexpr_infix(struct fifo *expr, int *err);
int evalexpr_lexer(char *str, int rpn, struct fifo **expr);

// Helper functions.
int is_operator(char c);
int valid(char c, int rpn);
int parse_number(char *str, size_t *i);
void skip_spaces(char *str, size_t *i);

#endif /* !EVAL_EXPR_H */
