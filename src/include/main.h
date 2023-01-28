#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "execution.h"
#include "lexer.h"
#include "parser.h"

int exec_final(struct token *input_tokens);
int exec_stdin_noargs();
int exec_stdin();
int exec_file(char *argv);
int exec_argument(char *input_string);

#endif /* MAIN_H */
