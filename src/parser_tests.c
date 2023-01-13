#include "../include/parser.h"

int main(void)
{
    xalloc_init();
    //token list = malloc(3 * sizeof (token));
    //list = malloc(sizeof(token *));
    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = NEWLINE;
    token4->value = "\n";
    token4->next = NULL;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = WORD;
    token3->value = "/exe";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = WORD;
    token2->value = "/bin";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "ls";
    token->next = token2;

    struct ast *ast = parser_list(token);

    ast->vtable->pretty_print(ast);
    return 0;
}
