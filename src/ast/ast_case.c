#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_case_init(char *name, struct ast *case_clause)
{
    static struct ast_vtable vtable = {
            .run = &case_run,
            .free = &case_free,
            .pretty_print = &case_pretty_print,
    };
    struct ast_case *case_ast = xmalloc(1, sizeof(struct ast_case));
    case_ast->base.type = AST_CASE;
    case_ast->base.vtable = &vtable;
    case_ast->name = name;
    case_ast->case_clause = case_clause;
    return &case_ast->base;
}

bool case_run(struct ast *ast) {
    assert(ast && ast->type == AST_CASE);
    //struct ast_case *case_ast = (struct ast_case *)ast;
    //TODO(clara)
    return false;
}

void case_free(struct ast *ast)
{
    assert(ast && ast->type == AST_CASE);
    struct ast_case *case_ast = (struct ast_case*) ast;
    xfree(case_ast->name);
    case_ast->case_clause->vtable->free(case_ast->case_clause);
    xfree(ast);
}

void case_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_CASE);
    struct ast_case *case_ast = (struct ast_case*) ast;
    printf("case: %s ", case_ast->name);
    printf("in : ");
    case_ast->case_clause->vtable->pretty_print(case_ast->case_clause);
    printf("esac\n");
}