/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_traversal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 12:00:00 by framiran          #+#    #+#             */
/*   Updated: 2026/01/15 12:00:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern void	close_fd_safe(int fd);
extern void	register_child_process(pid_t pid, t_pipe_ctx *ctx);

static pid_t	fork_and_execute_cmd(t_ast *node, t_pipe_ctx *ctx, int fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		configure_stds(ctx->pipes, ctx->pipe_indice, ctx->numb_of_pipes);
		close_all_pipes(ctx->pipes, ctx->numb_of_pipes);
		execute_in_child(node, ctx->shell, fd);
		_exit(ERROR);
	}
	return (pid);
}

/**
 * @brief Spawns a child process to execute a single command in a pipeline.
 *
 * Manages the lifecycle of a command node:
 * 1. Sets up heredoc signal handlers (for Ctrl+C during input).
 * 2. Handles heredocs to obtain reading FD.
 * 3. Checks if Ctrl+C was pressed; skips execution if so.
 * 4. Restores normal signal handlers.
 * 5. Forks a child process.
 *
 * @param curr_node The AST node containing command and redirection data.
 * @param ctx Pipeline context containing pipes, indices, and shell state.
 */
static void	handle_cmd(t_ast *curr_node, t_pipe_ctx *ctx)
{
	int		heredoc_fd;
	pid_t	pid;

	setup_signals_heredoc();
	heredoc_fd = handle_heredocs(curr_node->cmd->redirs, ctx->shell);
	if (g_signal_received == SIGINT)
	{
		close_fd_safe(heredoc_fd);
		return ;
	}
	setup_signals_executing();
	pid = fork_and_execute_cmd(curr_node, ctx, heredoc_fd);
	close_fd_safe(heredoc_fd);
	register_child_process(pid, ctx);
}

static pid_t	fork_and_execute_subshell(t_ast *node, t_pipe_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		configure_stds(ctx->pipes, ctx->pipe_indice, ctx->numb_of_pipes);
		close_all_pipes(ctx->pipes, ctx->numb_of_pipes);
		execute_ast(node, ctx->shell);
		_exit(ERROR);
	}
	return (pid);
}

/**
 * @brief Spawns a child process to execute a subshell in a pipeline.
 *
 * Treats a logic block (AND/OR) as a single stage in the pipeline:
 * 1. Forks a child process.
 * 2. Inside the child:
 * - Connects the subshell's standard streams to the parent's pipeline.
 * - Closes parent pipe FDs to ensure EOF propagation.
 * - Re-enters the main AST executor (execute_ast).
 *
 * @param curr_node The AST node representing the subshell (AND/OR).
 * @param ctx Pipeline context containing pipes, indices, and shell state.
 */
static void	handle_subshell(t_ast *curr_node, t_pipe_ctx *ctx)
{
	pid_t	pid;

	pid = fork_and_execute_subshell(curr_node, ctx);
	register_child_process(pid, ctx);
}

/**
 * @brief Linearly traverses the AST to spawn pipeline stages.
 *
 * Flattens the pipe hierarchy by visiting all nodes at the current level.
 * - For PIPE nodes: Recursively visits left/right branches and increments
 *   the pipe index to move data forward through the array.
 * - For CMD/Subshells: Acts as a leaf. Spawns a process (via handle_cmd
 *   or handle_subshell) that uses the current pipe_indice for its FDs.
 * Subshells (AND/OR) are treated as single units, preserving their
 * internal logic while maintaining their position in the parent pipeline.
 * After executing left side, pipe_indice is incremented for right side.
 *
 * @param node The current AST node.
 * @param ctx Pipeline context containing pipes, indices, and shell state.
 */
void	traverse_and_execute(t_ast *node, t_pipe_ctx *ctx)
{
	if (!node)
		return ;
	if (node->type == PIPE)
	{
		traverse_and_execute(node->left, ctx);
		if (g_signal_received == SIGINT)
			return ;
		ctx->pipe_indice++;
		traverse_and_execute(node->right, ctx);
	}
	else
	{
		if (node->type == CMD)
			handle_cmd(node, ctx);
		else if (node->type == AND || node->type == OR)
			handle_subshell(node, ctx);
	}
}
