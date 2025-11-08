/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:33:30 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

/**
 * @brief Implement unset built-in command
 * 
 * Removes environment variables from the environment list.
 * Supports removing multiple variables in one command.
 * 
 * @param args NULL-terminated array of arguments (args[0] is "unset")
 * @param env Pointer to environment linked list
 * @return 0 on success, 1 if any unset had invalid identifier
 * 
 * @details
 * Behavior:
 * - unset VAR            → Remove VAR from environment
 * - unset A B C          → Remove multiple variables
 * - unset NONEXISTENT    → No error (silently ignore)
 * - unset 123VAR         → Error: not a valid identifier
 * 
 * Valid identifiers (same rules as export):
 * - Must start with letter or underscore
 * - Can contain letters, digits, and underscores
 * - Examples: VAR, _VAR, VAR123
 * 
 * @note
 * - Continues processing even if one variable is invalid
 * - Returns 0 even if variable doesn't exist
 * - Returns 1 only if identifier is invalid
 * - Can unset critical variables like PATH
 * 
 * @example
 * $ export MY_VAR=test
 * $ echo $MY_VAR
 * test
 * $ unset MY_VAR
 * $ echo $MY_VAR
 * 
 * $ unset VAR1 VAR2 VAR3
 * $ echo $?
 * 0
 */
int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	ret;

	if (!args[1])
		return (SUCCESS);
	ret = SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_error("unset", args[i], "not a valid identifier");
			ret = ERROR;
		}
		else
		{
			unset_env_value(env, args[i]);
		}
		i++;
	}
	return (ret);
}
