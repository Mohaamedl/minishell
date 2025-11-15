/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:33:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implement env built-in command
 * 
 * Prints all environment variables in NAME=value format.
 * Each variable is printed on a separate line.
 * 
 * @param env Pointer to the environment linked list
 * @return Always returns 0
 * 
 * @details
 * Behavior:
 * - Prints all environment variables
 * - Format: NAME=value\n
 * - No options or arguments supported
 * - Returns 0 even if environment is empty
 * 
 * @note
 * - Iterates through the environment linked list
 * - Does not sort the output (matches bash behavior)
 * - Handles NULL environment gracefully
 * 
 * @example
 * $ env
 * USER=john
 * HOME=/home/john
 * PATH=/usr/bin:/bin
 * $ echo $?
 * 0
 */
int	builtin_env(t_env *env)
{
	t_env	*current;

	if (!env)
		return (SUCCESS);
	current = env;
	while (current)
	{
		if (current->key && current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (SUCCESS);
}
