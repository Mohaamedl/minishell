/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:33:21 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

/**
 * @brief Print all environment variables in export format
 * 
 * Prints variables in sorted order with 'declare -x' prefix.
 * Format: declare -x NAME="value"
 * 
 * @param env Environment linked list
 */
static void	print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->key && current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else if (current->key)
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

/**
 * @brief Parse and validate export argument
 * 
 * Splits the argument into key and value at the '=' sign.
 * Validates that the key is a valid identifier.
 * 
 * @param arg The export argument (NAME=value or NAME)
 * @param key Output parameter for the key
 * @param value Output parameter for the value
 * @return 0 on success, 1 on error
 */
static int	parse_export_arg(const char *arg, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key_len = equal_sign - arg;
		*key = ft_substr(arg, 0, key_len);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (*value == NULL && equal_sign))
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Process a single export argument
 * 
 * Validates the variable name and adds/updates it in the environment.
 * 
 * @param arg The export argument
 * @param env Pointer to environment list
 * @return 0 on success, 1 on error
 */
static int	process_export_arg(const char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		ret;

	if (parse_export_arg(arg, &key, &value) == ERROR)
		return (ERROR);
	if (!is_valid_identifier(key))
	{
		print_error("export", arg, "not a valid identifier");
		free(key);
		free(value);
		return (ERROR);
	}
	if (value)
		ret = set_env_value(env, key, value);
	else
		ret = SUCCESS;
	free(key);
	free(value);
	return (ret);
}

/**
 * @brief Implement export built-in command
 * 
 * Sets or displays environment variables.
 * Without arguments: displays all variables in export format.
 * With arguments: sets variables in the environment.
 * 
 * @param args NULL-terminated array of arguments
 * @param env Pointer to environment linked list
 * @return 0 on success, 1 if any export failed
 * 
 * @details
 * Behavior:
 * - export VAR=value     → Set VAR to value
 * - export A=1 B=2       → Set multiple variables
 * - export               → Print all variables (sorted)
 * - export 123=bad       → Error: not a valid identifier
 * - export VAR=          → Set VAR to empty string
 * - export VAR           → Declare VAR without value
 * 
 * Valid identifiers:
 * - Must start with letter or underscore
 * - Can contain letters, digits, and underscores
 * - Examples: VAR, _VAR, VAR123, VAR_NAME
 * 
 * Invalid identifiers:
 * - Cannot start with digit: 123VAR
 * - Cannot contain special chars: VA-R, VA$R
 * 
 * @note
 * - Continues processing even if one export fails
 * - Returns 1 if any export failed
 * - Variables without '=' are declared but not set
 * 
 * @example
 * $ export MY_VAR=hello
 * $ echo $MY_VAR
 * hello
 * $ export
 * declare -x MY_VAR="hello"
 * declare -x PATH="/usr/bin:/bin"
 */
int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_export_list(*env);
		return (SUCCESS);
	}
	ret = SUCCESS;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], env) == ERROR)
			ret = ERROR;
		i++;
	}
	return (ret);
}
