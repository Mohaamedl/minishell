/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 12:00:00 by framiran          #+#    #+#             */
/*   Updated: 2026/01/15 12:00:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../structs.h"

/**
 * @brief Redirects STDIN/STDOUT to the appropriate pipes in the pipeline.
 *
 * Connects the process to the "tubing" created by the parent:
 * - If not the first command (pipe_indice > 0): Reads from previous pipe.
 * - If not the last command (pipe_indice < numb_of_pipes): Writes to current.
 * fd[0] is for reading, fd[1] is for writing.
 * read_fd points to the read end of the previous pipe.
 * write_fd points to the write end of the current pipe.
 *
 * @param pipes The array containing all pipe file descriptors.
 * @param pipe_indice The current command's position (0-indexed).
 * @param numb_of_pipes Total number of pipes in the current pipeline level.
 */
void	configure_stds(int *pipes, int pipe_indice, int numb_of_pipes)
{
	int	read_fd;
	int	write_fd;

	read_fd = (pipe_indice - 1) * 2;
	write_fd = (pipe_indice * 2) + 1;
	if (pipe_indice > 0)
		dup2(pipes[read_fd], STDIN_FILENO);
	if (pipe_indice < numb_of_pipes)
		dup2(pipes[write_fd], STDOUT_FILENO);
}

/**
 * @brief Counts the total number of pipes in the current pipeline sequence.
 *
 * This function determines the size of the pipe array needed for a single
 * pipeline. It stops counting at CMD nodes or logical operators (AND/OR),
 * effectively flattening the current pipeline while treating subshells
 * as single, isolated units.
 *
 * @param node The current AST node being evaluated.
 * @return int The number of PIPE nodes found in the current branch.
 */
int	calc_numb_pipes(t_ast *node)
{
	int	count;

	count = 0;
	if (node->type == CMD)
		return (count);
	if (node->type == AND || node->type == OR)
		return (count);
	if (node->type == PIPE)
		count++;
	count = count + calc_numb_pipes(node->left);
	count = count + calc_numb_pipes(node->right);
	return (count);
}

/**
 * @brief Safely closes a file descriptor if valid.
 *
 * @param fd The file descriptor to close.
 */
void	close_fd_safe(int fd)
{
	if (fd != -1)
		close(fd);
}

/**
 * @brief Registers a spawned child process in the pipeline context.
 *
 * Updates the process ID array and increments the spawned counter.
 *
 * @param pid The process ID of the spawned child.
 * @param ctx Pipeline context to update.
 */
void	register_child_process(pid_t pid, t_pipe_ctx *ctx)
{
	if (pid > 0)
	{
		ctx->pids[ctx->pipe_indice] = pid;
		ctx->spawned++;
	}
}
