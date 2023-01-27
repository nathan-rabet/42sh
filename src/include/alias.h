#ifndef ALIAS_H
#define ALIAS_H

#include <stdbool.h>

/**
 * @brief Add an alias to the alias list
 *
 * @note If the alias already exists, it will be overwritten.
 * If the value is NULL or empty, the alias will be removed (if it exists)
 *
 * @param name The name of the alias
 * @param value The value of the alias
 */
void add_alias(const char *name, const char *value);

/**
 * @brief Get the value of an alias
 *
 * @param name The name of the alias
 *
 * @return The value of the alias, or NULL if it does not exist
 */
const char *get_alias_value(const char *name);

/**
 * @brief Remove an alias from the alias list
 *
 * @note If the alias does not exist, nothing will happen
 *
 * @param name The name of the alias to delete
 */
void remove_alias(const char *name);

/**
 * @brief Check if an alias as already been used
 *
 * @param name The name of the alias
 *
 * @return True if the alias has been used, false otherwise
 */
bool is_alias_used(const char *name);

/**
 * @brief Mark an alias as used
 *
 * @param name The name of the alias
 */
void use_alias(const char *name);

/**
 * @brief Mark all aliases as unused
 *
 */
void unuse_all_aliases(void);

#endif /* ALIAS_H */
