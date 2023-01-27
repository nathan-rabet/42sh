#ifndef BUILTINS_H
#define BUILTINS_H

/**
 * @brief `alias` builtin command
 *
 * @param name The name of the alias
 * @param value The value of the alias
 */
void builtin_alias(const char *name, const char *value);

/**
 * @brief `unalias` builtin command
 *
 * @param name The name of the alias to remove
 */
void builtin_unalias(const char *name);

#endif /* BUILTINS_H */
