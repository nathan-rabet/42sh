#include "global.h"

static global_sh global = { 0 };

global_sh *get_global_sh(void)
{
    return &global;
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
