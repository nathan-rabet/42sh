#include "../include/global.h"

static global_sh global = {
        .variable = NULL,
};

global_sh *get_global_sh(void)
{
    return &global;
}

void push_variable(char *name, char *value)
{
    struct variable *tmp = xmalloc(1, sizeof (variable));
    if (global.variable == NULL)
        tmp->next = NULL;
    else
        tmp->next = global.variable;
    global.variable = tmp;
}

char *get_variable(char *name)
{
    struct variable *tmp = global.variable;
    while (tmp)
    {
        if (strcmp(tmp->name, name) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

void remove_variable(char *name)
{
    if (global.variable == NULL)
        return;
    struct variable *tmp = global.variable;
    if (global.variable && strcmp(global.variable->name, name) == 0)
    {
        global.variable = global.variable->next;
        xfree(tmp);
    }
    if (global.variable->next == NULL && strcmp(global.variable->name, name) != 0)
        return;
    while (tmp->next && strcmp(tmp->next->name, name) != 0)
        tmp = tmp->next;
    if (strcmp(tmp->next->name, name) == 0)
        return;
    struct variable *tmp2 = tmp->next;
    tmp->next = tmp->next->next;
    xfree(tmp2);
}

void push_loop_stack(struct ast *ast)
{
    (void)ast;
    // TODO
}

struct ast *pop_loop_stack(void)
{
    // TODO
    return NULL;
}
