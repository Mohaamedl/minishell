/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:24:20 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/03 14:07:12 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close all pipe file descriptors
 *
 * CRITICAL: Must close all pipes to prevent FD leaks.
 * This function should be called:
 * 1. In parent process after forking all children
 * 2. In each child process after setting up its pipes
 * 3. On error during pipe creation
 *
 * @param pipes Array of pipe file descriptors
 * @param num_pipes Number of pipes (n-1 for n commands)
 */
void	close_all_pipes(int *pipes, int num_pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < num_pipes * 2)
	{
		close(pipes[i]);
		i++;
	}
}
