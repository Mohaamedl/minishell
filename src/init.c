/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:20 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/14 15:57:21 by framiran         ###   ########.fr       */
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
void	increment_shlvl(t_env **env)
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
