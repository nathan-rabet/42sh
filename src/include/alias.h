#ifndef ALIAS_H
#define ALIAS_H

/**
 * @brief Add an alias to the alias list
 *
 * @note If the alias already exists, it will be overwritten
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
const char *get_alias(const char *name);

/**
 * @brief Remove an alias from the alias list
 *
 * @note If the alias does not exist, nothing will happen
 *
 * @param name The name of the alias to delete
 */
void remove_alias(const char *name);

#endif /* ALIAS_H */
