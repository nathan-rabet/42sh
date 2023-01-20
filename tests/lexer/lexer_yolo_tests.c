#include <criterion/criterion.h>

#include "lexer.h"
#include "xalloc.h"

/**
 * @brief Test the lexer's token recognition
 *
 * @param input The input string
 * @param expected_tokens A contiguous array of tokens
 * @param nb_tokens The number of tokens in the array
 */
static inline void _test_tokens(const char *input, const token *expected_tokens,
                                size_t nb_tokens)
{
    xalloc_init();

    token *returned_tokens = get_tokens(input, strlen(input));

    if (!expected_tokens)
        cr_assert_null(returned_tokens, "Got tokens, expected none");

    for (size_t i = 0; i < nb_tokens; i++)
    {
        bool is_value_correct =
            strcmp(returned_tokens->value, expected_tokens[i].value) == 0;
        bool is_type_correct = returned_tokens->type == expected_tokens[i].type;

        if (!is_value_correct || !is_type_correct)
        {
            cr_assert_fail("Token %zu is incorrect: got type %d, value %s, "
                           "expected type %d, value %s",
                           i, returned_tokens->type, returned_tokens->value,
                           expected_tokens[i].type, expected_tokens[i].value);
        }
        returned_tokens = returned_tokens->next;
    }
    xfree(returned_tokens);
    xalloc_deinit();
}

Test(lexer_yolo, chatgpt_1)
{
    const char *cmd = "echo hello world";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "hello" },
        { .type = WORD, .value = "world" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, chatgpt_2)
{
    const char *cmd = "cat file1.txt file2.txt > output.txt";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "file1.txt" },
        { .type = WORD, .value = "file2.txt" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "output.txt" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, chatgpt_3)
{
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
}

Test(lexer_yolo, complex_lvl_1)
{
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
}

Test(lexer_yolo, complex_lvl_2)
{
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
}

Test(lexer_yolo, complex_lvl_3)
{
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
}

Test(lexer_yolo, complex_lvl_4)
{
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
}

Test(lexer_yolo, lot_of_symbols)
{
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
}

Test(lexer_yolo, only_concatenated_operators)
{
    const char *cmd = "&&|||&|";

    const token expected_tokens[] = {
        { .type = AND_IF, .value = "&&" }, { .type = OR_IF, .value = "||" },
        { .type = PIPE, .value = "|" },    { .type = AMPERSAND, .value = "&" },
        { .type = PIPE, .value = "|" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, only_concatenated_operators_with_spaces)
{
    const char *cmd = "&& || | &";

    const token expected_tokens[] = {
        { .type = AND_IF, .value = "&&" },
        { .type = OR_IF, .value = "||" },
        { .type = PIPE, .value = "|" },
        { .type = AMPERSAND, .value = "&" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, only_concatenated_operators_with_spaces_and_words)
{
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
}

Test(lexer_yolo, backslash_n_nightmare)
{
    const char *cmd = "cat '\ntest\n' && \\\necho newline";

    const token expected_tokens[] = {
        { .type = WORD, .value = "cat" },
        { .type = WORD, .value = "'\ntest\n'" },
        { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "newline" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, no_braces_and_bang)
{
    const char *cmd = "echo {a,b}c !d";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "{a,b}c" },
        { .type = WORD, .value = "!d" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_yolo, yes_braces_and_bang)
{
    const char *cmd = "echo { a, b } c ! d";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" }, { .type = LBRACE, .value = "{" },
        { .type = WORD, .value = "a," },   { .type = WORD, .value = "b" },
        { .type = RBRACE, .value = "}" },  { .type = WORD, .value = "c" },
        { .type = BANG, .value = "!" },    { .type = WORD, .value = "d" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}
