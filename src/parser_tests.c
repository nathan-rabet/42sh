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

void test3()
{
    printf("\n----------------TEST 3-----------------\n");

    xalloc_init();
    //token list = malloc(3 * sizeof (token));
    //list = malloc(sizeof(token *));
    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = NEWLINE;
    token5->value = "\n";
    token5->next = NULL;

    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = FI;
    token4->value = "\n";
    token4->next = token5;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = WORD;
    token3->value = "/exe";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = ELSE;
    token2->value = "/bin";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "toto";
    token->next = token2;


    struct token *tok4 = xmalloc(1, sizeof(struct token*));
    tok4->type = WORD;
    tok4->value = "echo";
    tok4->next = token;

    struct token *tok3 = xmalloc(1, sizeof(struct token*));
    tok3->type = THEN;
    tok3->value = "file";
    tok3->next = tok4;

    struct token *tok2 = xmalloc(1, sizeof(struct token*));
    tok2->type = WORD;
    tok2->value = "dir";
    tok2->next = tok3;

    struct token *tok = xmalloc(1, sizeof(struct token*));
    tok->type = IF;
    tok->value = "cd";
    tok->next = tok2;

    struct ast *ast = parser_input(tok);

    ast->vtable->pretty_print(ast);
}

void test4()
{
    printf("\n----------------TEST 4-----------------\n");

    xalloc_init();
    //token list = malloc(3 * sizeof (token));
    //list = malloc(sizeof(token *));

    struct token *token12 = xmalloc(1, sizeof(struct token*));
    token12->type = NEWLINE;
    token12->value = "\n";
    token12->next = NULL;

    struct token *token11 = xmalloc(1, sizeof(struct token*));
    token11->type = FI;
    token11->value = "\n";
    token11->next = token12;

    struct token *token10 = xmalloc(1, sizeof(struct token*));
    token10->type = WORD;
    token10->value = "le zen2";
    token10->next = token11;

    struct token *token9 = xmalloc(1, sizeof(struct token*));
    token9->type = THEN;
    token9->value = "\n";
    token9->next = token10;

    struct token *token8 = xmalloc(1, sizeof(struct token*));
    token8->type = WORD;
    token8->value = "le elif ";
    token8->next = token9;

    struct token *token7 = xmalloc(1, sizeof(struct token*));
    token7->type = ELIF;
    token7->value = "\n";
    token7->next = token8;

    struct token *token6= xmalloc(1, sizeof(struct token*));
    token6->type = WORD;
    token6->value = "thenif euh";
    token6->next = token7;

    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = THEN;
    token5->value = "\n";
    token5->next = token6;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = WORD;
    token3->value = "/exe";
    token3->next = token5;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = ELIF;
    token2->value = "/bin";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "toto";
    token->next = token2;


    struct token *tok4 = xmalloc(1, sizeof(struct token*));
    tok4->type = WORD;
    tok4->value = "echo";
    tok4->next = token;

    struct token *tok3 = xmalloc(1, sizeof(struct token*));
    tok3->type = THEN;
    tok3->value = "file";
    tok3->next = tok4;

    struct token *tok2 = xmalloc(1, sizeof(struct token*));
    tok2->type = WORD;
    tok2->value = "dir";
    tok2->next = tok3;

    struct token *tok = xmalloc(1, sizeof(struct token*));
    tok->type = IF;
    tok->value = "cd";
    tok->next = tok2;

    struct ast *ast = parser_input(tok);

    ast->vtable->pretty_print(ast);
}



int main(void)
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
