#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_case_init(char *name, struct list_case_item **case_item,
                          size_t nb_item)
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
    case_ast->nb_item = nb_item;
    case_ast->case_item = case_item;
    return &case_ast->base;
}

struct list_case_item *list_case_item_init(char **word, size_t nb_word,
                                           struct ast *to_execute)
{
    struct list_case_item *item = xmalloc(1, sizeof(struct list_case_item));
    item->word = word;
    item->nb_word = nb_word;
    item->to_execute = to_execute;
    return item;
}

int case_run(struct ast *ast)
{
    assert(ast && ast->type == AST_CASE);
    struct ast_case *case_ast = (struct ast_case *)ast;
    int status = 0;
    for (size_t i = 0; i < case_ast->nb_item; i++)
    {
        for (size_t j = 0; j < case_ast->case_item[i]->nb_word; j++)
        {
            if (strcmp(case_ast->name, case_ast->case_item[i]->word[j]) == 0)
                status = case_ast->case_item[i]->to_execute->vtable->run(
                    case_ast->case_item[i]->to_execute);
        }
    }

    return status;
}

void case_free(struct ast *ast)
{
    assert(ast && ast->type == AST_CASE);
    struct ast_case *case_ast = (struct ast_case *)ast;
    xfree(case_ast->name);
    xfree(ast);
}

void case_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_CASE);
    struct ast_case *case_ast = (struct ast_case *)ast;
    printf("CASE (%s) ", case_ast->name);
    printf("in \n");
    for (size_t i = 0; i < case_ast->nb_item; i++)
    {
        printf("ITEM : (");
        for (size_t j = 0; j < case_ast->case_item[i]->nb_word; j++)
            printf(" %s | ", case_ast->case_item[i]->word[j]);
        printf(") EXECUTE : ");
        case_ast->case_item[i]->to_execute->vtable->pretty_print(
            case_ast->case_item[i]->to_execute);
        printf(";;\n");
    }
    printf("ESAC\n");
}
