#include <stdlib.h>

#include "evalexpr.h"

// Insertion of element.
struct stack *stack_push(struct stack *s, int e)
{
    struct stack *elm = xmalloc(1, sizeof(struct stack));

    // Error check.
    if (elm == NULL)
        return s;

    elm->data = e;
    elm->next = s;

    return elm;
}

// Removal of top of stack.
struct stack *stack_pop(struct stack *s)
{
    // Check NULL parameter.
    if (s == NULL)
        return NULL;

    struct stack *next = s->next;

    xfree(s);

    s = NULL;

    return next;
}

// Getting value at top of stack.
int stack_peek(struct stack *s)
{
    return s->data;
}

// Stack destroy.
void stack_destroy(struct stack *s)
{
    while (s != NULL)
    {
        s = stack_pop(s);
    }
}
