/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:40:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/28 16:13:21 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../structs.h"

/* Forward declarations */
static int	execute_pipe_node(t_ast *node, t_shell *shell);



/**
 * @brief Execute a pipe node (two-command pipeline)
 *
 * @param node The AST node with type PIPE
 * @param shell The shell state structure
 * @return Exit status of the last command in the pipeline
 */
static int	execute_pipe_node(t_ast *node, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pids[2];
	int		status;

	if (!node || !node->left || !node->right)
		return (ERROR);
	if (pipe(pipe_fds) == -1)
		return (perror("minishell: pipe"), ERROR);
	pids[0] = create_process();
	if (pids[0] == 0)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		execute_in_child(node->left, shell);
	}
	pids[1] = create_process();
	if (pids[1] == 0)
	{
		close(pipe_fds[1]);
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		execute_in_child(node->right, shell);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	status = wait_for_pipeline(pids, 2);
	return (status);
}

/**
 * @brief Execute an AST node recursively
 *
 * Main executor that traverses the AST and executes nodes based on type.
 * Handles:
 * - CMD nodes: Execute as simple commands
 * - PIPE nodes: Execute as pipelines
 * - Other operators (AND, OR): To be implemented for bonus
 *
 * @param node The AST node to execute
 * @param shell The shell state structure
 * @return Exit status of the execution
 */
int	execute_ast(t_ast *node, t_shell *shell)
{
	int	status;

	if (!node)
		return (SUCCESS);
	// Execute based on node type
	if (node->type == CMD)
	{
		status = execute_command_node(node, shell);
	}
	else if (node->type == PIPE)
	{
		status = execute_pipe_node(node, shell);
	}
	else if (node->type == AND)
	{
		// AND: Execute right only if left succeeds (exit 0)
		status = execute_ast(node->left, shell);
		if (status == SUCCESS)
			status = execute_ast(node->right, shell);
	}
	else if (node->type == OR)
	{
		// OR: Execute right only if left fails (exit != 0)
		status = execute_ast(node->left, shell);
		if (status != SUCCESS)
			status = execute_ast(node->right, shell);
	}
	else
	{
		// Unknown node type
		status = ERROR;
	}
	// Update shell's last exit status
	shell->last_exit_status = status;
	return (status);
}
