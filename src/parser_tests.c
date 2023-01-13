#include "../include/parser.h"

void test1()
{
    printf("\n----------------TEST 1-----------------\n");
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

    struct ast *ast = parser_input(token);

    ast->vtable->pretty_print(ast);
}

void test2()
{
    printf("\n----------------TEST 2-----------------\n");

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


    struct token *tok4 = xmalloc(1, sizeof(struct token*));
    tok4->type = SEMI;
    tok4->value = ";";
    tok4->next = token;

    struct token *tok3 = xmalloc(1, sizeof(struct token*));
    tok3->type = WORD;
    tok3->value = "file";
    tok3->next = tok4;

    struct token *tok2 = xmalloc(1, sizeof(struct token*));
    tok2->type = WORD;
    tok2->value = "dir";
    tok2->next = tok3;

    struct token *tok = xmalloc(1, sizeof(struct token*));
    tok->type = WORD;
    tok->value = "cd";
    tok->next = tok2;

    struct ast *ast = parser_input(tok);

    ast->vtable->pretty_print(ast);
}


int main(void)
{
    test1();
    test2();
    return 0;
}
