#include "alias.h"
#include "global.h"
#include "xalloc.h"

void add_alias(const char *name, const char *value)
{
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

const char *get_alias(const char *name)
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

void remove_alias(const char *name)
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
                return;
            }
            previous = current;
            current = current->next;
        }
    }
}

void builtin_alias(const char *name, const char *value)
{
    add_alias(name, value);
}

void builtin_unalias(const char *name)
{
    remove_alias(name);
}
