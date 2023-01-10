#include <criterion/criterion.h>

#include "lexer.h"
#include "xalloc.h"

// rule 1
Test(lexer, rule_2_3_1_empty_input)
{
    const char *cmd = "";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);

    cr_assert_eq(tokens_len, 1);
    cr_assert_str_eq(tokens[0].value, "");

    cr_assert_eq(tokens[0].type, WORD);
}

Test(lexer, rule_2_3_1_simple_words)
{
    const char *cmd = "echo test eeee";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);

    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "echo");
    cr_assert_str_eq(tokens[1].value, "test");
    cr_assert_str_eq(tokens[2].value, "eeee");

    cr_assert_eq(tokens[2].type, WORD);
}

Test(lexer, rule_2_3_1_spaces_before_word)
{
    const char *cmd = "     test";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);

    cr_assert_eq(tokens_len, 1);
    cr_assert_str_eq(tokens[0].value, "test");

    cr_assert_eq(tokens[0].type, WORD);
}

Test(lexer, rule_2_3_1_spaces_between_words)
{
    const char *cmd = "     test     echo  ";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 1);
    cr_assert_str_eq(tokens[0].value, "test");
    cr_assert_str_eq(tokens[1].value, "echo");

    cr_assert_eq(tokens[1].type, WORD);
}
// Rule 2
Test(lexer, rule_2_3_2_great_operator)
{
    const char *cmd = "a > b";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "a");
    cr_assert_str_eq(tokens[1].value, ">");
    cr_assert_str_eq(tokens[2].value, "b");

    cr_assert_str_eq(tokens[1].type, GREAT);
    cr_assert_eq(tokens[2].type, WORD);
}

Test(lexer, rule_2_3_2_another_pipe_operator)
{
    const char *cmd = "a | b";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "a");
    cr_assert_str_eq(tokens[1].value, "|");
    cr_assert_str_eq(tokens[2].value, "b");

    cr_assert_str_eq(tokens[1].type, PIPE);

    cr_assert_eq(tokens[2].type, WORD);
}

Test(lexer, rule_2_3_2_double_operator_dgreat)
{
    const char *cmd = "a >> b";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "a");
    cr_assert_str_eq(tokens[1].value, ">>");
    cr_assert_str_eq(tokens[2].value, "b");

    cr_assert_str_eq(tokens[1].type, DGREAT);

    cr_assert_eq(tokens[2].type, WORD);
}

Test(lexer, rule_2_3_2_or_if_operator)
{
    const char *cmd = "a && b";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "a");
    cr_assert_str_eq(tokens[1].value, "&&");
    cr_assert_str_eq(tokens[2].value, "b");
    cr_assert_str_eq(tokens[1].type, AND_IF);

    cr_assert_eq(tokens[2].type, WORD);
}

// rule 3
Test(lexer, rule_2_3_3_double_operator_without_spaces)
{
    const char *cmd = "a>>b";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "a");
    cr_assert_str_eq(tokens[1].value, ">>");
    cr_assert_str_eq(tokens[2].value, "b");

    cr_assert_str_eq(tokens[1].type, DGREAT);

    cr_assert_eq(tokens[2].type, WORD);
}

Test(lexer, rule_2_3_2__S_quoted_operator)
{
    const char *cmd = "echo 'a > b'";

    size_t tokens_len = 0;
    token *tokens = get_tokens(cmd, &tokens_len);
    cr_assert_eq(tokens_len, 3);
    cr_assert_str_eq(tokens[0].value, "echo");
    cr_assert_str_eq(tokens[1].value, "a > b");
    cr_assert_str_eq(tokens[2].value, "b");

    cr_assert_eq(tokens[2].type, WORD);
}
