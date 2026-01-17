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

extern void	traverse_and_execute(t_ast *node, t_pipe_ctx *ctx);

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
	int			final_status;
	int			num_cmds;
	t_pipe_ctx	ctx;

	ctx.pipe_indice = 0;
	ctx.spawned = 0;
	ctx.numb_of_pipes = calc_numb_pipes(node);
	num_cmds = ctx.numb_of_pipes + 1;
	ctx.pipes = create_pipes(num_cmds);
	ctx.pids = malloc(sizeof(pid_t) * num_cmds);
	ctx.shell = shell;
	traverse_and_execute(node, &ctx);
	close_all_pipes(ctx.pipes, ctx.numb_of_pipes);
	if (g_signal_received == SIGINT)
		final_status = 130;
	else
		final_status = wait_for_pipeline(ctx.pids, ctx.spawned);
	free(ctx.pipes);
	free(ctx.pids);
	return (final_status);
}
