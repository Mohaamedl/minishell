/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:25:48 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/03 12:53:50 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Wait for a child process and get its exit status
 *
 * Waits for the specified child process to terminate and extracts
 * the exit status. Handles both normal exits and signal terminations.
 *
 * Exit status codes:
 * - Normal exit: 0-255 (value from exit())
 * - Signal termination: 128 + signal_number
 *
 * @param pid Process ID to wait for
 * @return int Exit status of the child process
 */
int	wait_for_process(pid_t pid)
{
	int	status;
	int	exit_code;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		return (ERROR);
	}
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	else
		exit_code = ERROR;
	return (exit_code);
}

/**
 * @brief Wait for all processes in a pipeline
 *
 * Waits for all child processes in a pipeline and collects their
 * exit statuses. Returns the exit status of the last command in
 * the pipeline (bash behavior).
 *
 * @param pids Array of process IDs
 * @param count Number of processes
 * @return int Exit status of the last command
 */
//int	wait_for_pipeline(pid_t *pids, int count)
//{
//	int	i;
//	int	status;
//	int	last_status;
//
//	i = 0;
//	last_status = 0;
//	while (i < count)
//	{
//		status = wait_for_process(pids[i]);
//		if (i == count - 1)
//			last_status = status;
//		i++;
//	}
//	return (last_status);
//}
