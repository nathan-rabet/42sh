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
        cr_assert_eq(returned_tokens->type, expected_tokens[i].type,
                     "Got token type '%d', expected '%d'",
                     returned_tokens->type, expected_tokens[i].type);
        cr_assert_str_eq(returned_tokens->value, expected_tokens[i].value,
                         "Got token value '%s', expected '%s'",
                         returned_tokens->value, expected_tokens[i].value);
        returned_tokens = returned_tokens->next;
    }

    xfree(returned_tokens);
    xalloc_deinit();
}

// rule 1
Test(lexer, rule_2_3_1_empty_input)
{
    const char *cmd = "";

    const token *expected_tokens = NULL;

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_1_simple_words)
{
    const char *cmd = "echo test eeee";

    const token expected_tokens[] = {
        { .type = NAME, .value = "echo" },
        { .type = NAME, .value = "test" },
        { .type = NAME, .value = "eeee" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_1_spaces_before_word)
{
    const char *cmd = "     test";

    const token expected_tokens[] = {
        { .type = NAME, .value = "test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_1_spaces_between_words)
{
    const char *cmd = "     test     echo  ";

    const token expected_tokens[] = {
        { .type = NAME, .value = "test" },
        { .type = NAME, .value = "echo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}
// Rule 2
Test(lexer, rule_2_3_2_great_operator)
{
    const char *cmd = "a > b";

    const token expected_tokens[] = {
        { .type = NAME, .value = "a" },
        { .type = GREAT, .value = ">" },
        { .type = NAME, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_2_another_pipe_operator)
{
    const char *cmd = "a | b";

    const token expected_tokens[] = {
        { .type = NAME, .value = "a" },
        { .type = PIPE, .value = "|" },
        { .type = NAME, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_2_double_operator_dgreat)
{
    const char *cmd = "a >> b";

    const token expected_tokens[] = {
        { .type = NAME, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = NAME, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_2_or_if_operator)
{
    const char *cmd = "a && b";

    const token expected_tokens[] = {
        { .type = NAME, .value = "a" },
        { .type = AND_IF, .value = "&&" },
        { .type = NAME, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

// rule 3
Test(lexer, rule_2_3_3_double_operator_without_spaces)
{
    const char *cmd = "a>>b";

    const token expected_tokens[] = {
        { .type = NAME, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = NAME, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer, rule_2_3_2__S_quoted_operator)
{
    const char *cmd = "echo 'a > b'";

    const token expected_tokens[] = {
        { .type = NAME, .value = "echo" },
        { .type = WORD, .value = "'a > b'" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}
