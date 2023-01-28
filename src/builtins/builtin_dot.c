#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "main.h"

#define BUFFER_SIZE 4096

/**
 * @brief execute commands in the current environment
 * If file does not contain a <slash>, the shell shall use the search path
 * specified by PATH to find the directory containing file.
 *
 * @param arg takes a filename or a filename and an option
 */
int builtin_dot(char **argv)
{
    int status = 0;
    char *filename = argv[0];
    if (strchr(filename, '/') != NULL)
    {
        // if filepath already contains '/', no need to search in PATH
        status = exec_file(filename);
        xfree_all();
        return status;
    }
    char *path = getenv("PATH");
    if (path == NULL)
    {
        fprintf(stderr, "Error: PATH environment variable not set\n");
        exit(1);
    }
    char *token = strtok(path, ":");
    while (token != NULL)
    {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", token, filename);
        if (access(file_path, F_OK) != -1)
        {
            return exec_file(file_path);
        }
        token = strtok(NULL, ":");
    }
    fprintf(stderr, "%s: No such file or directory\n", *argv);
    exit(1);
}