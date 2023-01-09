#include <string.h>

#include "lexer.h"
#include "xalloc.h"

#define BUFFER_SIZE 1024

char *IFS = " \t\n";

static void find_next_char(const char *str, char fond_char)
{
    size_t i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == fond_char)
            return i;
        i++;
    }
    return -1;
}

token *get_next_tokens(const char *input, size_t str_len, size_t *tokens_len)
{
    lexer lexer = {
        .str = input, .str_i = 0, .last_tokens = { 0 }, .last_tokens_i = 0
    };

    size_t len = strlen(input);

    token *t = NULL;

    while (lexer.str_i < len)
    {
        char c = input[i++];

        if (c == '\n' && input[i + 1] == '\0') // abcd...\n
        {
            t = xmalloc(1, sizeof(token));

            t->type = NEWLINE;
            tokens_len = 1;
            return t;
        }

        if (strchr(IFS, c) // abc def\nghi -> [abc, def, ghi]
            continue;

        else if (c == '\'')
        {
            buffer_i = 0;
            buffer[buffer_i++] = c;
            while (i < len)
            {
                c = input[i++];
                if (c == '\'')
                {
                    buffer[buffer_i++] = c;
                    break;
                }
                buffer[buffer_i++] = c;
            }
            buffer[buffer_i] = '\0';
            return (token){ .type = WORD, .value = strdup(buffer) };
        }
        else if (c == ';')
        {
            return (token){ .type = SEMICOLON };
        }
        else
        {
            buffer_i = 0;
            buffer[buffer_i++] = c;
            while (i < len)
            {
                c = input[i++];
                if (c == ' ' || c == '\t' || c == '\n' || c == ';')
                {
                    --i;
                    break;
                }
                buffer[buffer_i++] = c;
            }
            buffer[buffer_i] = '\0';
            if (strcmp(buffer, "if") == 0)
            {
                return (token){ .type = IF };
            }
            else if (strcmp(buffer, "then") == 0)
            {
                return (token){ .type = THEN };
            }
            else if (strcmp(buffer, "elif") == 0)
            {
                return (token){ .type = ELIF };
            }
            else if (strcmp(buffer, "else") == 0)
            {
                return (token){ .type = ELSE };
            }
            else if (strcmp(buffer, "fi") == 0)
            {
                return (token){ .type = FI };
            }
            else
            {
                return (token){ .type = WORD, .value = strdup(buffer) };
            }
        }
    }
    i = 0;
    return (token){ .type = EOF, .value = NULL };
}
