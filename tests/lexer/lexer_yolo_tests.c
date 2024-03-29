#include <criterion/criterion.h>

#include "lexer.h"
#include "lexer_tests_common.h"
#include "xalloc.h"

Test(lexer_yolo, chatgpt_1)
{
    xalloc_init();
    const char *cmd = "echo hello world";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "hello" },
        { .type = WORD, .value = "world" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, chatgpt_2)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, chatgpt_3)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, complex_lvl_1)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1 | wc -l";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "wc" },
        { .type = WORD, .value = "-l" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, complex_lvl_2)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1 | wc -l | "
                      "grep -v \"hello world\"";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "wc" },
        { .type = WORD, .value = "-l" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hello world\"" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, complex_lvl_3)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1 | wc -l | "
                      "grep -v \"hello world\" | sed -e \"s/hello/hi/g\"";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "wc" },
        { .type = WORD, .value = "-l" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hello world\"" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "sed" },
        { .type = WORD, .value = "-e" },
        { .type = WORD, .value = "\"s/hello/hi/g\"" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, complex_lvl_4)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1 | wc -l | "
                      "grep -v \"hello world\" | sed -e \"s/hello/hi/g\" | "
                      "grep -v \"hi world\"";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "wc" },
        { .type = WORD, .value = "-l" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hello world\"" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "sed" },
        { .type = WORD, .value = "-e" },
        { .type = WORD, .value = "\"s/hello/hi/g\"" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hi world\"" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, lot_of_symbols)
{
    xalloc_init();
    const char *cmd = "cat file1.txt file2.txt > output.txt 2>&1 && wc -l || "
                      "grep -v \"hello world\" | sed -e \"s/hello/hi/g\" ; "
                      "grep -v \"hi world\" &";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
        { .type = WORD, .value = "2" },
        { .type = GREATAND, .value = ">&" },
        { .type = WORD, .value = "1" },
        { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "wc" },
        { .type = WORD, .value = "-l" },
        { .type = OR_IF, .value = "||" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hello world\"" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "sed" },
        { .type = WORD, .value = "-e" },
        { .type = WORD, .value = "\"s/hello/hi/g\"" },
        { .type = SEMI, .value = ";" },
        { .type = WORD, .value = "grep" },
        { .type = WORD, .value = "-v" },
        { .type = WORD, .value = "\"hi world\"" },
        { .type = AMPERSAND, .value = "&" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, only_concatenated_operators)
{
    xalloc_init();
    const char *cmd = "&&|||&|";

    const token expected_tokens[] = {
        { .type = AND_IF, .value = "&&" }, { .type = OR_IF, .value = "||" },
        { .type = PIPE, .value = "|" },    { .type = AMPERSAND, .value = "&" },
        { .type = PIPE, .value = "|" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, only_concatenated_operators_with_spaces)
{
    xalloc_init();
    const char *cmd = "&& || | &";

    const token expected_tokens[] = {
        { .type = AND_IF, .value = "&&" },
        { .type = OR_IF, .value = "||" },
        { .type = PIPE, .value = "|" },
        { .type = AMPERSAND, .value = "&" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, only_concatenated_operators_with_spaces_and_words)
{
    xalloc_init();
    const char *cmd = "&& || | & cat file1.txt file2.txt";

    const token expected_tokens[] = {
        { .type = AND_IF, .value = "&&" },
        { .type = OR_IF, .value = "||" },
        { .type = PIPE, .value = "|" },
        { .type = AMPERSAND, .value = "&" },
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, backslash_n_nightmare)
{
    xalloc_init();
    const char *cmd = "cat '\ntest\n' && \\\necho newline";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "'\ntest\n'" },
        { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "newline" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, no_braces_and_bang)
{
    xalloc_init();
    const char *cmd = "echo {a,b}c !d";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "{a,b}c" },
        { .type = WORD, .value = "!d" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_yolo, yes_braces_and_bang)
{
    xalloc_init();
    const char *cmd = "echo { a, b } c ! d";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" }, { .type = LBRACE, .value = "{" },
        { .type = WORD, .value = "a," },   { .type = WORD, .value = "b" },
        { .type = RBRACE, .value = "}" },  { .type = WORD, .value = "c" },
        { .type = BANG, .value = "!" },    { .type = WORD, .value = "d" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}
