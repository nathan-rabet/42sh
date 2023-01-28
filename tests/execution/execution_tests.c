#include "../../src/include/execution.h"
#include "../../src/include/lexer.h"
#include "../../src/include/parser.h"

void test_exec_cmd()
{
    char **argv = xmalloc(2, sizeof(char *));
    argv[0] = "echo";
    argv[1] = "2";
    int *error = 0;
    exec_cmd(argv);
}

void test_exec_redir()
{
    printf("\n----------------TEST 1-----------------\n");
    printf("\n---------------- REDIRECTION STDOUT-----------------\n\n");

    struct token *token6 = xmalloc(1, sizeof(struct token));
    token6->type = NEWLINE;
    token6->value = "\n";
    token6->next = NULL;

    struct token *token4 = xmalloc(1, sizeof(struct token));
    token4->type = WORD;
    token4->value = "file.txt";
    token4->next = token6;

    struct token *token3 = xmalloc(1, sizeof(struct token));
    token3->type = GREAT;
    token3->value = ">";
    token3->next = token4;

    struct token *token2 = xmalloc(1, sizeof(struct token));
    token2->type = WORD;
    token2->value = "tofile";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token));
    token->type = WORD;
    token->value = "echo";
    token->next = token2;

    struct ast *ast = parser_input(token);
    ast->vtable->pretty_print(ast);
    ast->vtable->run(ast);
}

void test_exec_input()
{
    printf("\n----------------TEST 2-----------------\n");
    printf("\n---------------- REDIRECTION STDIN-----------------\n\n");

    struct token *token6 = xmalloc(1, sizeof(struct token));
    token6->type = NEWLINE;
    token6->value = "\n";
    token6->next = NULL;

    struct token *token3 = xmalloc(1, sizeof(struct token));
    token3->type = WORD;
    token3->value = "file.txt";
    token3->next = token6;

    struct token *token2 = xmalloc(1, sizeof(struct token));
    token2->type = LESS;
    token2->value = "<";
    token2->next = token3;

    struct token *token = xmalloc(1, sizeof(struct token));
    token->type = WORD;
    token->value = "cat";
    token->next = token2;

    struct ast *ast = parser_input(token);
    ast->vtable->pretty_print(ast);
    ast->vtable->run(ast);
}

int test(void)
{
    xalloc_init();
    test_exec_cmd();
    test_exec_redir();
    // test_exec_input();
    xfree_all();
    return 0;
}