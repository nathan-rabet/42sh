#ifndef LEXER_TESTS_COMMON_H
#define LEXER_TESTS_COMMON_H

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
            cr_assert_fail("Token %zu is incorrect: got type '%d', value '%s', "
                           "expected type '%d', value '%s'",
                           i, returned_tokens->type, returned_tokens->value,
                           expected_tokens[i].type, expected_tokens[i].value);
        }
        returned_tokens = returned_tokens->next;
    }
    xfree(returned_tokens);
}

#endif /* LEXER_TESTS_COMMON_H */
