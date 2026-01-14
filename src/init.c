/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:20 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:31:30 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Increment the SHLVL environment variable
 *
 * Gets the current SHLVL value, increments it by 1, and updates the
 * environment. If SHLVL doesn't exist, it starts from 0.
 *
 * @param env Pointer to the environment list
 */
static void	increment_shlvl(t_env **env)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = get_env_value(*env, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		set_env_value(env, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

/**
 * @brief Initialize the shell structure
 *
 * Allocates and initializes a new shell structure with environment variables,
 * exit status, running state, and signal handlers. Also increments SHLVL.
 *
 * @param envp Environment variables from main
 * @return Pointer to initialized shell structure, or NULL on error
 */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_env(envp);
	if (!shell->env_list)
	{
		free(shell);
		return (NULL);
	}
	shell->last_exit_status = 0;
	shell->running = 1;
	increment_shlvl(&shell->env_list);
	setup_signals_interactive();
	return (shell);
}
