#include <criterion/criterion.h>

#include "lexer.h"
#include "lexer_tests_common.h"
#include "xalloc.h"

// Rule 2.3.1: If the end of input is recognized, the current token (if
//  any) shall be delimited.
Test(lexer_scl, empty_input)
{
    xalloc_init();
    const char *cmd = "";

    const token *expected_tokens = NULL;

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_words)
{
    xalloc_init();
    const char *cmd = "echo test eeee";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "test" },
        { .type = WORD, .value = "eeee" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.2 : If the previous character was used as part of an
// operator and the current character is not quoted and can be used with
// the previous characters to form an operator, it shall be used as part
// of that (operator) token.
Test(lexer_scl, great_operator)
{
    xalloc_init();
    const char *cmd = "a > b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = GREAT, .value = ">" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, pipe_operator)
{
    xalloc_init();
    const char *cmd = "a | b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, double_operator_dgreat)
{
    xalloc_init();
    const char *cmd = "a >> b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, or_if_operator)
{
    xalloc_init();
    const char *cmd = "a && b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.3: If the previous character was used as part of an operator
// and the current character cannot be used with the previous characters
// to form an operator, the operator containing the previous character
// shall be delimited.
Test(lexer_scl, double_operator_without_spaces)
{
    xalloc_init();
    const char *cmd = "a>>b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, double_operator_with_spaces)
{
    xalloc_init();
    const char *cmd = "a >> b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, two_operators_concat)
{
    xalloc_init();
    const char *cmd = "a >>&& b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DGREAT, .value = ">>" },
        { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, three_operators_concat)
{
    xalloc_init();
    const char *cmd = "a >>&&| b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },    { .type = DGREAT, .value = ">>" },
        { .type = AND_IF, .value = "&&" }, { .type = PIPE, .value = "|" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, invalid_newline)
{
    xalloc_init();
    const char *cmd = "a >>b\nbb";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },  { .type = DGREAT, .value = ">>" },
        { .type = WORD, .value = "b" },  { .type = NEWLINE, .value = "\n" },
        { .type = WORD, .value = "bb" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, DLESSDASH)
{
    xalloc_init();
    const char *cmd = "a <<- b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DLESSDASH, .value = "<<-" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, DLESSDASH_sticked)
{
    xalloc_init();
    const char *cmd = "a<<-b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = DLESSDASH, .value = "<<-" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, concatened_operators)
{
    xalloc_init();
    const char *cmd = "a &&& b";

    const token expected_tokens[] = {
        { .type = WORD, .value = "a" },
        { .type = AND_IF, .value = "&&" },
        { .type = AMPERSAND, .value = "&" },
        { .type = WORD, .value = "b" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.4: If the current character is <backslash>, single-quote, or
// double-quote and it is not quoted, it shall affect quoting for subsequent
// characters up to the end of the quoted text. The rules for quoting are as
// described in Quoting . During token recognition no substitutions shall be
// actually performed, and the result token shall contain exactly the characters
// that appear in the input (except for <newline> joining), unmodified,
// including any embedded or enclosing quotes or substitution operators, between
// the <quotation-mark> and the end of the quoted text. The token shall not be
// delimited by the end of the quoted field.
Test(lexer_scl, quoted_operator)
{
    xalloc_init();
    const char *cmd = "echo 'a > b'";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "'a > b'" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_word)
{
    xalloc_init();
    const char *cmd = "'echo'";

    const token expected_tokens[] = {
        { .type = WORD, .value = "'echo'" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_word_with_spaces)
{
    xalloc_init();
    const char *cmd = "'echo'   ";

    const token expected_tokens[] = {
        { .type = WORD, .value = "'echo'" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, two_dquotes)
{
    xalloc_init();
    const char *cmd = "\"echo\"\"echo\"";

    const token expected_tokens[] = {
        { .type = WORD, .value = "\"echo\"" },
        { .type = WORD, .value = "\"echo\"" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, two_quotes_different)
{
    xalloc_init();
    const char *cmd = "\"echo\"'echo'";

    const token expected_tokens[] = {
        { .type = WORD, .value = "\"echo\"" },
        { .type = WORD, .value = "'echo'" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_backslash)
{
    xalloc_init();
    const char *cmd = "echo \\a";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, newline_joining)
{
    xalloc_init();
    const char *cmd = "echo \\\n a";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, two_newline_joining)
{
    xalloc_init();
    const char *cmd = "echo \\\n\\\n a";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, three_newline_joining)
{
    xalloc_init();
    const char *cmd = "echo \\\n\\\n\\\n a";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.5: If the current character is an unquoted '$' or '`', the
// shell shall identify the start of any candidates for parameter
// expansion (Parameter Expansion), command substitution (Command
// Substitution), or arithmetic expansion (Arithmetic Expansion) from
// their introductory unquoted character sequences: '$' or "${", "$(" or
// '`', and "$((", respectively. The shell shall read sufficient input
// to determine the end of the unit to be expanded (as explained in the
// cited sections). While processing the characters, if instances of
// expansions or quoting are found nested within the substitution, the
// shell shall recursively process them in the manner specified for the
// construct that is found. The characters found from the beginning of
// the substitution to its end, allowing for any recursion necessary to
// recognize embedded constructs, shall be included unmodified in the
// result token, including any embedded or enclosing substitution
// operators or quotes. The token shall not be delimited by the end of
// the substitution.
Test(lexer_scl, simple_command_substitution)
{
    xalloc_init();
    const char *cmd = "echo $(a)";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$(a)" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_command_substitution_with_spaces)
{
    xalloc_init();
    const char *cmd = "echo $(  a  )";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$(  a  )" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_command_backquote)
{
    xalloc_init();
    const char *cmd = "echo `a`";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "`a`" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_command_backquote_with_spaces)
{
    xalloc_init();
    const char *cmd = "echo `  a  `";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "`  a  `" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_arithmetic_expansion)
{
    xalloc_init();
    const char *cmd = "echo $((a))";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$((a))" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_arithmetic_expansion_with_spaces)
{
    xalloc_init();
    const char *cmd = "echo $((  a  ))";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$((  a  ))" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_parameter_expansion_braces)
{
    xalloc_init();
    const char *cmd = "echo ${a}";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "${a}" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_parameter_expansion_braces_with_spaces)
{
    xalloc_init();
    const char *cmd = "echo ${  a  }";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "${  a  }" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_parameter_expansion)
{
    xalloc_init();
    const char *cmd = "echo $a";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_parameter_expansion_with_spaces)
{
    xalloc_init();
    const char *cmd = "echo $a    ";

    const token expected_tokens[] = {
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "$a" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.6: If the current character is not quoted and can be used as
// the first character of a new operator, the current token (if any)
// shall be delimited. The current character shall be used as the
// beginning of the next (operator) token.

Test(lexer_scl, simple_command_with_spaces)
{
    xalloc_init();
    const char *cmd = "test;";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
        { .type = SEMI, .value = ";" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, simple_command_with_spaces_and_newline)
{
    xalloc_init();
    const char *cmd = "test&&";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
        { .type = AND_IF, .value = "&&" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.7: If the current character is an unquoted <blank>, any
// token containing the previous character is delimited and the current
// character shall be discarded.
Test(lexer_scl, spaces_before_word)
{
    xalloc_init();
    const char *cmd = "     test";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, spaces_between_words)
{
    xalloc_init();
    const char *cmd = "     test     echo  ";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
        { .type = WORD, .value = "echo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, spaces_and_tab)
{
    xalloc_init();
    const char *cmd = "test    \t \t echo";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
        { .type = WORD, .value = "echo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.8: If the previous character was part of a word, the current
// character shall be appended to that word.
Test(lexer_scl, reserved_if)
{
    xalloc_init();
    const char *cmd = "if";

    const token expected_tokens[] = {
        { .type = IF, .value = "if" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_if_clause)
{
    xalloc_init();
    const char *cmd = "if test; then echo; fi";

    const token expected_tokens[] = {
        { .type = IF, .value = "if" },     { .type = WORD, .value = "test" },
        { .type = SEMI, .value = ";" },    { .type = THEN, .value = "then" },
        { .type = WORD, .value = "echo" }, { .type = SEMI, .value = ";" },
        { .type = FI, .value = "fi" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_lbrace)
{
    xalloc_init();
    const char *cmd = "{";

    const token expected_tokens[] = {
        { .type = LBRACE, .value = "{" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_sticked)
{
    xalloc_init();
    const char *cmd = "if{";

    const token expected_tokens[] = {
        { .type = WORD, .value = "if{" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_sticked_2)
{
    xalloc_init();
    const char *cmd = "ifthenelsefi";

    const token expected_tokens[] = {
        { .type = WORD, .value = "ifthenelsefi" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_sticked_3)
{
    xalloc_init();
    const char *cmd = "!test";

    const token expected_tokens[] = {
        { .type = WORD, .value = "!test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_bang)
{
    xalloc_init();
    const char *cmd = "! test";

    const token expected_tokens[] = {
        { .type = BANG, .value = "!" },
        { .type = WORD, .value = "test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, reserved_with_separator)
{
    xalloc_init();
    const char *cmd = "if;then;fi";

    const token expected_tokens[] = {
        { .type = IF, .value = "if" },     { .type = SEMI, .value = ";" },
        { .type = THEN, .value = "then" }, { .type = SEMI, .value = ";" },
        { .type = FI, .value = "fi" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.9: If the current character is a '#', it and all subsequent
// characters up to, but excluding, the next <newline> shall be
// discarded as a comment. The <newline> that ends the line is not
// considered part of the comment.

Test(lexer_scl, comment)
{
    xalloc_init();
    const char *cmd = "test # comment";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, alone_comment)
{
    xalloc_init();
    const char *cmd = "#";

    const token *expected_tokens = NULL;

    _test_tokens(cmd, expected_tokens, 0);
}

Test(lexer_scl, only_comment)
{
    xalloc_init();
    const char *cmd = "# comment";

    const token *expected_tokens = NULL;

    _test_tokens(cmd, expected_tokens, 0);
}

Test(lexer_scl, comment_with_newline)
{
    xalloc_init();
    const char *cmd = "test # comment\n";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_scl, comment_with_newline_2)
{
    xalloc_init();
    const char *cmd = "test # comment\ntest2";

    const token expected_tokens[] = {
        { .type = WORD, .value = "test" },
        { .type = WORD, .value = "test2" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

// Rule 2.3.10: The current character is used as the start of a new
// word.

// ! FIXME: I have no f*cking idea how to test this rule.
