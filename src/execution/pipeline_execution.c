/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution.c                               :+:      :+:    :+:   */
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
 * @brief Spawns a child process to execute a single command within a pipeline.
 *
 * This function manages the lifecycle of a command node:
 * 1. Handles heredocs to obtain a reading FD before forking.
 * 2. Forks a child process:
 * - Child: Connects to pipes (via configure_stds), cleans up the pipe array,
 *   and executes the command logic.
 * - Parent: Closes the heredoc FD immediately to prevent resource leaks.
 * The child process configures its standard streams, closes all pipes,
 * and executes the command. The parent closes the heredoc FD if opened.
 *
 * @param curr_node The AST node containing command and redirection data.
 * @param ctx Pipeline context containing pipes, indices, and shell state.
 */
static void	handle_cmd(t_ast *curr_node, t_pipe_ctx *ctx)
{
	int	heredoc_pipe_read_fd;
	int	pid;

	heredoc_pipe_read_fd = handle_heredocs(curr_node->cmd->redirs, ctx->shell);
	pid = fork();
	if (pid == 0)
	{
		configure_stds(ctx->pipes, ctx->pipe_indice, ctx->numb_of_pipes);
		close_all_pipes(ctx->pipes, ctx->numb_of_pipes);
		execute_in_child(curr_node, ctx->shell, heredoc_pipe_read_fd);
		_exit(ERROR);
	}
	if (heredoc_pipe_read_fd != -1)
		close(heredoc_pipe_read_fd);
}

/**
 * @brief Spawns a child process to execute a subshell within a pipeline.
 *
 * Treats a logic block (AND/OR) as a single stage in the pipeline:
 * 1. Forks a child process.
 * 2. Inside the child:
 * - Connects the subshell's standard streams to the parent's pipeline.
 * - Closes parent pipe FDs to ensure EOF propagation.
 * - Re-enters the main AST executor (execute_ast) to handle internal logic.
 * This allows complex commands like `a | (b && c) | d` to function correctly.
 *
 * @param curr_node The AST node representing the subshell (AND/OR).
 * @param ctx Pipeline context containing pipes, indices, and shell state.
 */
static void	handle_subshell(t_ast *curr_node, t_pipe_ctx *ctx)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		configure_stds(ctx->pipes, ctx->pipe_indice, ctx->numb_of_pipes);
		close_all_pipes(ctx->pipes, ctx->numb_of_pipes);
		execute_ast(curr_node, ctx->shell);
		_exit(ERROR);
	}
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
static void	traverse_and_execute(t_ast *node, t_pipe_ctx *ctx)
{
	if (!node)
		return ;
	if (node->type == PIPE)
	{
		traverse_and_execute(node->left, ctx);
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

/**
 * @brief Orchestrates the execution of a multi-stage pipeline.
 *
 * 1. Calculates total pipes needed (excluding nested subshell pipes).
 * 2. Allocates and initializes the pipe array.
 * 3. Traverses the AST to spawn all sibling processes in parallel.
 * 4. Closes all parent pipe FDs to trigger EOF in children.
 * 5. Waits for all children and cleans up memory.
 * CRUCIAL: Parent must close pipes before waiting, otherwise children
 * will never receive EOF.
 *
 * @param node The current PIPE node of the AST.
 * @param shell The shell state structure.
 * @return int The final exit status of the pipeline's last command.
 */
int	execute_pipeline(t_ast *node, t_shell *shell)
{
	t_pipe_ctx	ctx;
	int			final_status;

	ctx.pipe_indice = 0;
	ctx.numb_of_pipes = calc_numb_pipes(node);
	ctx.pipes = create_pipes(ctx.numb_of_pipes);
	ctx.shell = shell;
	traverse_and_execute(node, &ctx);
	close_all_pipes(ctx.pipes, ctx.numb_of_pipes);
	final_status = wait_for_pipeline(ctx.numb_of_pipes + 1);
	free(ctx.pipes);
	return (final_status);
}
