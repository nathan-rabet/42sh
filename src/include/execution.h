#ifndef EXECUTION_H
#define EXECUTION_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "xalloc.h"

struct list_redir;
struct ast_pipe;

struct dup_item
{
    int fd_source;
    int fd_dest;
    int fd_source_dup;
    struct dup_item *next;
};

/**
 * @brief list of dup item
 */
struct list_dup
{
    struct dup_item *head; // head of the list
};

int exec_builtins(char **argv);
int exec_cmd(char **argv);
void exec_redir(struct list_redir *redir);
int exec_pipeline(struct ast_pipe *ast);

// Utils
bool is_builtin(char *name);
void return_and_free(char *msg, int exit_code);

// List dup
void push_top_dup(int fd_source, int fd_destination);
void close_all_list_dup();
void dup2_all_list();

#endif // EXECUTION_H
