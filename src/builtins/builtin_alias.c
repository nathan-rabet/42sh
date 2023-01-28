#include "alias.h"
#include "builtins.h"
#include "global.h"
#include "xalloc.h"

void add_alias(const char *name, const char *value)
{
    if (value == NULL || strlen(value) == 0)
    {
        remove_alias(name);
        return;
    }

    global_sh *global = get_global_sh();

    alias *new_alias = xcalloc(1, sizeof(alias));
    new_alias->name = xstrdup(name);
    new_alias->value = xstrdup(value);

    if (global->alias_ll == NULL)
    {
        global->alias_ll = new_alias;
    }
    else
    {
        alias *current = global->alias_ll;
        while (current->next != NULL)
        {
            if (strcmp(current->name, name) == 0)
            {
                xfree(current->value);
                current->value = new_alias->value;
                xfree(new_alias->name);
                xfree(new_alias);
                return;
            }
            current = current->next;
        }
        current->next = new_alias;
    }
}

const char *get_alias_value(const char *name)
{
    global_sh *global = get_global_sh();

    alias *current = global->alias_ll;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

bool remove_alias(const char *name)
{
    global_sh *global = get_global_sh();

    if (global->alias_ll != NULL)
    {
        alias *previous = NULL;
        alias *current = global->alias_ll;
        while (current != NULL)
        {
            if (strcmp(current->name, name) == 0)
            {
                if (previous == NULL)
                    global->alias_ll = current->next;
                else
                    previous->next = current->next;

                xfree(current->name);
                xfree(current->value);
                xfree(current);
                return true;
            }
            previous = current;
            current = current->next;
        }
    }

    return false;
}

bool is_alias_used(const char *name)
{
    global_sh *global = get_global_sh();

    alias *current = global->alias_ll;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current->is_used;
        current = current->next;
    }
    return false;
}

void use_alias(const char *name)
{
    global_sh *global = get_global_sh();

    alias *current = global->alias_ll;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            current->is_used = true;
            return;
        }
        current = current->next;
    }
}

void unuse_all_aliases(void)
{
    global_sh *global = get_global_sh();

    alias *current = global->alias_ll;
    while (current != NULL)
    {
        current->is_used = false;
        current = current->next;
    }
}

void builtin_alias(char **argv)
{
    if (*argv == NULL)
    {
        // Print all aliases
        for (alias *current = get_global_sh()->alias_ll; current != NULL;
             current = current->next)
            printf("%s='%s'\n", current->name, current->value);
    }
    else
    {
        for (; *argv != NULL; ++argv)
        {
            // Check if argv[0] is form "name=value"
            const char *equal = strchr(*argv, '=');
            if (equal != NULL && equal[1] != '\0' && &equal[-1] >= *argv)
            {
                // argv[0] is form "name=value"
                size_t name_len = equal - *argv;
                char *name = xmalloc(name_len + 1, sizeof(char));
                strncpy(name, *argv, name_len);
                name[name_len] = '\0';
                add_alias(name, equal + 1);
                xfree(name);
            }
            else
            {
                // argv[0] is form "name"
                const char *value = get_alias_value(*argv);
                if (value == NULL)
                    fprintf(stderr, "42sh: alias: %s : not found\n", *argv);
                else
                    printf("%s='%s'\n", *argv, value);
            }
        }
    }
}

void builtin_unalias(char **argv)
{
    if (*argv == NULL)
    {
        fprintf(stderr, "unalias: usage: unalias [-a] name [name ...]\n");
    }
    else
    {
        if (strcmp(*argv, "-a") == 0)
        {
            // Remove all aliases
            while (get_global_sh()->alias_ll != NULL)
                remove_alias(get_global_sh()->alias_ll->name);
        }
        else
        {
            // Remove arguments aliases
            for (; *argv != NULL; ++argv)
                if (!remove_alias(*argv))
                    fprintf(stderr, "42sh: unalias: %s : not found\n", *argv);
        }
    }
}
