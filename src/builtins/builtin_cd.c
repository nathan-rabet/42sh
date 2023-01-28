#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "xalloc.h"

// Checks if given string starts with dot or dot-dot.
int has_dot_prefix(char *str)
{
    return !strncmp(".", str, 1) || !strncmp("..", str, 2);
}

// Gets PWD. Size of current buffer is returned as well.
char *get_current_dir(size_t *buffer_size)
{
    char *buffer = xcalloc(*buffer_size, sizeof(char));
    char *path = NULL;
    while ((path = getcwd(buffer, *buffer_size)) == NULL
           && errno == ENAMETOOLONG)
    {
        *buffer_size *= 2;
        buffer = xrealloc(buffer, *buffer_size, sizeof(char));
    }

    size_t pwd_len = strlen(path);
    if (path[pwd_len - 1] != '/')
    {
        // TODO(robertnant): might fail if len is
        // exactly buffer_size with NULL byte included.
        path[pwd_len] = '/';
        pwd_len++;
    }

    path[pwd_len] = '\0';

    return path;
}

// Creates full target path by concatinating PWD to current path.
char *create_full_path(const char *curpath)
{
    // The path can be longer than PATH_MAX since it will canonicalized.
    size_t buffer_size = PATH_MAX;
    char *full_path = get_current_dir(&buffer_size);

    // Other error found.
    if (full_path == NULL)
        return NULL;

    size_t pwd_len = strlen(full_path);
    size_t curpath_len = strlen(curpath);

    if (curpath_len + pwd_len + 1 > buffer_size)
    {
        full_path = realloc(full_path, curpath_len + pwd_len + 1);
    }
    memcpy(full_path + pwd_len, curpath, curpath_len);
    full_path[curpath_len + pwd_len] = '\0';

    return full_path;
}

/**
 * Canonicalizes given path and returns it.
 * Returns error code of 1 if path is too long.
 */
char *canonicalize_path(const char *path, int *err_code)
{
    // Error occured during full path creation.
    if (path == NULL)
        return NULL;

    char *curpath = xcalloc(strlen(path) + 1, sizeof(char));
    char *destination = curpath;

    // Add slash for absolute path if any.
    if (path[0] == '/')
    {
        destination[0] = '/';
        destination++;
    }

    while (path[0] != '\0')
    {
        if (*path == '/')
        {
            // Skip leading or trailing backslashes and skip extra backslashes.
            while (*path == '/')
            {
                path++;
            }
        }
        else if (*path == '.' && (*(path + 1) == '\0' || *(path + 1) == '/'))
        {
            // Handle case for dot.
            path += *(path + 1) == '\0' ? 1 : 2;
            continue;
        }
        else if (strncmp("..", path, 2)
                 && (*(path + 2) == '\0' || *(path + 2) == '/'))
        {
            // Handle case for dot dot (move back to previous directory).
            path += *(path + 1) == '\0' ? 2 : 3;

            /**
             * Moving back to previous directory is only done if destination
             * pointer has been incremented.
             */
            while (destination > curpath + 1)
            {
                if (*destination != '/')
                    destination--;
                else
                    break;
            }
            continue;
        }
        else
        {
            // Copy current formatted part of path to destination.
            while (*path && *path != '/')
            {
                *destination++ = *path++;
            }

            // Add separating backslash if found.
            if (*path != '\0')
            {
                *destination = '/';
                destination++;
                path++;
            }
        }
    }

    // Remove final '/'.
    if (destination > curpath + 1 && *(destination - 1) == '/')
        *(destination - 1) = '\0';

    // If final path is too long, return NULL.
    if (strlen(curpath) >= PATH_MAX)
        *err_code = 1;

    return curpath;
}

void chdir_and_check_failure(char *path, char *pwd)
{
    if (chdir(path) == -1)
    {
        xfree(path);
        err(1, "cd: Failed to move to given directory");
    }

    // Set PWD and OLDPWD environment variables.
    setenv("OLDPWD", pwd, 1);
    setenv("PWD", path, 1);
}

void my_cd(char **argv)
{
    char *directory = argv[1];
    char *curpath = NULL;

    size_t pwd_buffer_size = PATH_MAX;
    char *pwd = get_current_dir(&pwd_buffer_size);

    // Get home environment.
    char *home = getenv("HOME");

    if (directory == NULL)
    {
        if (home == NULL)
        {
            // Set curpath to "/" as default behaviour.
            directory = xcalloc(2, sizeof(char));
            directory[0] = '/';
        }
        else
        {
            directory = home;
        }
    }
    if (directory[0] == '/' || has_dot_prefix(directory))
    {
        curpath = directory;

        // Concatenate PWD to path (if not absolute path) and canonicalize.
        int err_code = 0;
        if (curpath[0] != '/')
        {
            curpath = create_full_path(directory);
        }

        curpath = canonicalize_path(curpath, &err_code);
        // Check if curpath is too long.
        size_t pwd_len = strlen(pwd);
        if (err_code == 1 && strncmp(pwd, curpath, pwd_len) == 0)
        {
            // Turn into relative path.
            // TODO(robertnant): check for errors.
            chdir_and_check_failure(curpath + pwd_len, pwd);
        }
        else if (err_code == 0)
        {
            chdir_and_check_failure(curpath, pwd);
        }
        xfree(curpath);
    }
    else
    {
        /**
         * Normally the directory should be checked to see if exists.
         * However it is not required by subject.
         */
        if (directory[0] != '/')
        {
            curpath = create_full_path(directory);
        }
        chdir_and_check_failure(curpath, pwd);
        xfree(curpath);
    }
}
