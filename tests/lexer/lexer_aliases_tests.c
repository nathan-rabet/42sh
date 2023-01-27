#include <criterion/criterion.h>

#include "alias.h"
#include "lexer.h"
#include "lexer_tests_common.h"
#include "xalloc.h"

Test(lexer_aliases, alias_simple)
{
    xalloc_init();
    add_alias("foo", "bar");

    const char *cmd = "foo";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_simple_2)
{
    xalloc_init();
    add_alias("foo", "bar");

    const char *cmd = "foo bar";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "bar" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_simple_3)
{
    xalloc_init();
    add_alias("foo", "bar");

    const char *cmd = "foo bar baz";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_complex)
{
    xalloc_init();
    xalloc_init();

    add_alias("foo", "bar baz");

    const char *cmd = "foo bar baz";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_complex_1)
{
    xalloc_init();
    add_alias("foo", "bar baz");

    const char *cmd = "foo bar baz";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_complex_2)
{
    xalloc_init();
    add_alias("foo", "bar foo");

    const char *cmd = "foo bar baz";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "foo" },
        { .type = WORD, .value = "bar" },
        { .type = WORD, .value = "baz" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));
}

Test(lexer_aliases, not_alias)
{
    xalloc_init();
    add_alias("foo", "bar");

    const char *cmd = "echo foo";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "foo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, not_alias_2)
{
    xalloc_init();
    add_alias("foo", "bar");

    const char *cmd = "echo foo bar";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "echo" },
        { .type = WORD, .value = "foo" },
        { .type = WORD, .value = "bar" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_recursive)
{
    xalloc_init();
    add_alias("foo", "bar ; foo");

    const char *cmd = "foo";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = SEMI, .value = ";" },
        { .type = WORD, .value = "foo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_recursive_1)
{
    xalloc_init();
    add_alias("foo", "bar ; foo");

    const char *cmd = "foo; foo";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" }, { .type = SEMI, .value = ";" },
        { .type = WORD, .value = "foo" }, { .type = SEMI, .value = ";" },
        { .type = WORD, .value = "bar" }, { .type = AND_IF, .value = "&&" },
        { .type = WORD, .value = "foo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}

Test(lexer_aliases, alias_recursive_3)
{
    xalloc_init();
    add_alias("foo", "bar || foo");

    const char *cmd = "foo";

    const token *expected_tokens = (const token[]){
        { .type = WORD, .value = "bar" },
        { .type = OR_IF, .value = "||" },
        { .type = WORD, .value = "foo" },
    };

    _test_tokens(cmd, expected_tokens, sizeof(expected_tokens) / sizeof(token));

    xalloc_deinit();
}
