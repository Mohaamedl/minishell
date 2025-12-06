/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a new child process using fork()
 *
 * This function is a wrapper around fork() that handles errors gracefully.
 * It creates a new process by duplicating the calling process.
 *
 * Return value:
 * - In parent process: returns child's PID (> 0)
 * - In child process: returns 0
 * - On error: returns -1 and prints error message
 *
 * @return pid_t - Process ID (parent gets child PID, child gets 0, -1 on error)
 */
pid_t	create_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (-1);
	}
	return (pid);
}
