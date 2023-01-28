#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "builtins.h"
#include "xalloc.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static void _test_aliases(const char *expected, FILE *(*func)(void))
{
    FILE *actual = func();

    char *actual_str = xcalloc(strlen(expected) + 1, sizeof(char));

    fread(actual_str, sizeof(char), strlen(expected) + 1, actual);

    cr_assert_str_eq(actual_str, expected);

    xfree(actual_str);
    fclose(actual);
}

Test(aliases_builtins, alias_only, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", NULL };

    builtin_alias(cmd + 1);

    const char *expected = "";
    _test_aliases(expected, cr_get_redirected_stdout);

    xalloc_deinit();
}

// Test(aliases_builtins, alias_only_with_alias, .init = cr_redirect_stdout)
// {
//     xalloc_init();

//     char *cmd[] = { "alias", "foo=bar", NULL };
//     char *cmd1[] = { "alias", NULL };

//     builtin_alias(cmd + 1);
//     builtin_alias(cmd1 + 1);

//     const char *expected = "foo='bar'\n";
//     _test_aliases(expected, cr_get_redirected_stdout);

//     xalloc_deinit();
// }

// Test(aliases_builtins, alias_only_with_aliases, .init = cr_redirect_stdout)
// {
//     xalloc_init();

//     char *cmd[] = { "alias", "foo=bar", NULL };
//     char *cmd1[] = { "alias", "foo1=bar1", NULL };
//     char *cmd2[] = { "alias", NULL };

//     builtin_alias(cmd + 1);
//     builtin_alias(cmd1 + 1);
//     builtin_alias(cmd2 + 1);

//     const char *expected = "foo='bar'\n"
//                            "foo1='bar1'\n";

//     _test_aliases(expected, cr_get_redirected_stdout);

//     xalloc_deinit();
// }

Test(aliases_builtins, incorrect_equal_space, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo", "=bar", NULL };
    char *cmd1[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_alias(cmd1 + 1);

    const char *expected = "42sh: alias: foo : not found\n"
                           "42sh: alias: =bar : not found\n";

    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, incorrect_equal_space_2, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo", "=", "bar", NULL };
    char *cmd1[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_alias(cmd1 + 1);

    const char *expected = "42sh: alias: foo : not found\n"
                           "42sh: alias: = : not found\n"
                           "42sh: alias: bar : not found\n";

    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_only, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "unalias", NULL };

    builtin_unalias(cmd + 1);

    const char *expected = "unalias: usage: unalias [-a] name [name ...]\n";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_only_with_alias, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo=bar", NULL };
    char *cmd1[] = { "unalias", "foo", NULL };
    char *cmd2[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_unalias(cmd1 + 1);
    builtin_alias(cmd2 + 1);

    const char *expected = "";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_only_with_aliases, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo=bar", NULL };
    char *cmd1[] = { "alias", "foo1=bar1", NULL };
    char *cmd2[] = { "unalias", "foo", "foo1", NULL };
    char *cmd3[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_alias(cmd1 + 1);
    builtin_unalias(cmd2 + 1);
    builtin_alias(cmd3 + 1);

    const char *expected = "";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_all, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo=bar", NULL };
    char *cmd1[] = { "alias", "foo1=bar1", NULL };
    char *cmd2[] = { "unalias", "-a", NULL };
    char *cmd3[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_alias(cmd1 + 1);
    builtin_unalias(cmd2 + 1);
    builtin_alias(cmd3 + 1);

    const char *expected = "";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_all_nothing, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "unalias", "-a", NULL };

    builtin_unalias(cmd + 1);

    const char *expected = "";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}

Test(aliases_builtins, unalias_non_existing, .init = redirect_all_std)
{
    xalloc_init();

    char *cmd[] = { "unalias", "foo", NULL };

    builtin_unalias(cmd + 1);

    const char *expected = "42sh: unalias: foo : not found\n";
    _test_aliases(expected, cr_get_redirected_stderr);

    xalloc_deinit();
}
