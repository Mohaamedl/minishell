/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:09 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:31:30 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clean up and free all shell resources
 *
 * Frees all allocated memory associated with the shell structure including
 * environment variables and readline history.
 *
 * @param shell Pointer to the shell structure to clean up
 */
void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env_list)
		free_env(shell->env_list);
	rl_clear_history();
	free(shell);
}
