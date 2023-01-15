#include "../include/parser.h"

void test1()
{
    printf("\n----------------TEST 1-----------------\n");
    printf("\n---------------- COMMAND -----------------\n\n");

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
    printf("\n---------------- SIMPLE LIST -----------------\n\n");


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
    printf("\n---------------- SIMPLE IF -----------------\n\n");


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
    printf("\n---------------- IF ELIF IMBRIQUE -----------------\n\n");


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

void test5()
{
    printf("\n----------------TEST 5-----------------\n");
    printf("\n---------------- IF AND LIST -----------------\n\n");


    xalloc_init();
    //token list = malloc(3 * sizeof (token));
    //list = malloc(sizeof(token *));

    struct token *token12 = xmalloc(1, sizeof(struct token*));
    token12->type = NEWLINE;
    token12->value = "\n";
    token12->next = NULL;

    struct token *token10 = xmalloc(1, sizeof(struct token*));
    token10->type = FI;
    token10->value = "fi";
    token10->next = token12;

    struct token *token9 = xmalloc(1, sizeof(struct token*));
    token9->type = SEMI;
    token9->value = ";";
    token9->next = token10;

    struct token *token8 = xmalloc(1, sizeof(struct token*));
    token8->type = WORD;
    token8->value = "b";
    token8->next = token9;

    struct token *token7 = xmalloc(1, sizeof(struct token*));
    token7->type = WORD;
    token7->value = "echo";
    token7->next = token8;

    struct token *token6= xmalloc(1, sizeof(struct token*));
    token6->type = NEWLINE;
    token6->value = "\n";
    token6->next = token7;

    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = WORD;
    token5->value = "a";
    token5->next = token6;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = WORD;
    token3->value = "echo";
    token3->next = token5;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = THEN;
    token2->value = "then";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = SEMI;
    token->value = ";";
    token->next = token2;


    struct token *tok4 = xmalloc(1, sizeof(struct token*));
    tok4->type = WORD;
    tok4->value = "true";
    tok4->next = token;

    struct token *tok3 = xmalloc(1, sizeof(struct token*));
    tok3->type = SEMI;
    tok3->value = ";";
    tok3->next = tok4;

    struct token *tok2 = xmalloc(1, sizeof(struct token*));
    tok2->type = WORD;
    tok2->value = "false";
    tok2->next = tok3;

    struct token *tok = xmalloc(1, sizeof(struct token*));
    tok->type = IF;
    tok->value = "if";
    tok->next = tok2;

    struct ast *ast = parser_input(tok);

    ast->vtable->pretty_print(ast);
}

void test6()
{
    printf("\n----------------TEST 6-----------------\n");
    printf("\n---------------- REDIRECTION -----------------\n\n");

    xalloc_init();

    struct token *token6 = xmalloc(1, sizeof(struct token*));
    token6->type = NEWLINE;
    token6->value = "\n";
    token6->next = NULL;

    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = WORD;
    token4->value = "file.txt";
    token4->next = token6;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = GREAT;
    token3->value = ">";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = WORD;
    token2->value = "tofile";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "echo";
    token->next = token2;

    struct ast *ast = parser_input(token);

    ast->vtable->pretty_print(ast);
}

void test7()
{
    printf("\n----------------TEST 7-----------------\n");
    printf("\n---------------- REDIRECTION AND IF -----------------\n\n");

    xalloc_init();

    struct token *tokenend = xmalloc(1, sizeof(struct token*));
    tokenend->type = NEWLINE;
    tokenend->value = "\n";
    tokenend->next = NULL;

    struct token *token11 = xmalloc(1, sizeof(struct token*));
    token11->type = FI;
    token11->value = "fi";
    token11->next = tokenend;

    struct token *token10 = xmalloc(1, sizeof(struct token*));
    token10->type = SEMI;
    token10->value = ";";
    token10->next = token11;

    struct token *token9 = xmalloc(1, sizeof(struct token*));
    token9->type = WORD;
    token9->value = "example.txt";
    token9->next = token10;

    struct token *token8 = xmalloc(1, sizeof(struct token*));
    token8->type = GREAT;
    token8->value = ">";
    token8->next = token9;

    struct token *token7 = xmalloc(1, sizeof(struct token*));
    token7->type = WORD;
    token7->value = "foobar";
    token7->next = token8;

    struct token *token6 = xmalloc(1, sizeof(struct token*));
    token6->type = WORD;
    token6->value = "echo";
    token6->next = token7;

    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = THEN;
    token5->value = "then";
    token5->next = token6;

    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = SEMI;
    token4->value = ";";
    token4->next = token5;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = WORD;
    token3->value = "ok";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = WORD;
    token2->value = "echo";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = IF;
    token->value = "if";
    token->next = token2;

    struct ast *ast = parser_input(token);

    ast->vtable->pretty_print(ast);
}

void test8()
{
    printf("\n----------------TEST 8-----------------\n");
    printf("\n---------------- Multiple REDIRECTION -----------------\n\n");

    xalloc_init();

    struct token *tokenend = xmalloc(1, sizeof(struct token*));
    tokenend->type = NEWLINE;
    tokenend->value = "\n";
    tokenend->next = NULL;

    struct token *token6 = xmalloc(1, sizeof(struct token*));
    token6->type = WORD;
    token6->value = "file2.txt";
    token6->next = tokenend;

    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = GREAT;
    token5->value = ">";
    token5->next = token6;


    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = WORD;
    token4->value = "file.txt";
    token4->next = token5;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = GREAT;
    token3->value = ">";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = WORD;
    token2->value = "tofile";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "echo";
    token->next = token2;

    struct ast *ast = parser_input(token);

    ast->vtable->pretty_print(ast);
}

void test9()
{
    printf("\n----------------TEST 9-----------------\n");
    printf("\n---------------- SIMPLE PIPELINE -----------------\n\n");

    xalloc_init();

    struct token *tokenend = xmalloc(1, sizeof(struct token*));
    tokenend->type = NEWLINE;
    tokenend->value = "\n";
    tokenend->next = NULL;

    struct token *token5 = xmalloc(1, sizeof(struct token*));
    token5->type = WORD;
    token5->value = "a";
    token5->next = tokenend;


    struct token *token4 = xmalloc(1, sizeof(struct token*));
    token4->type = WORD;
    token4->value = "echo";
    token4->next = token5;

    struct token *token3 = xmalloc(1, sizeof(struct token*));
    token3->type = PIPE;
    token3->value = "|";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token*));
    token2->type = WORD;
    token2->value = "tofile";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token*));
    token->type = WORD;
    token->value = "echo";
    token->next = token2;

    struct ast *ast = parser_input(token);

    ast->vtable->pretty_print(ast);
}


int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    return 0;
}
