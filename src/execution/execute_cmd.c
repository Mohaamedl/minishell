/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:13:22 by framiran          #+#    #+#             */
/*   Updated: 2026/01/03 14:41:19 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a builtin command.
 *
 * Applies redirections and executes the builtin. Handles heredoc redirections.
 */
static int	execute_builtin_command(char **args, t_shell *shell,
				t_ast *node, int heredoc_pipe_read_fd)
{
	int	status;

	status = apply_redirections(node, heredoc_pipe_read_fd);
	if (status == SUCCESS)
		status = execute_builtin(args, shell);
	return (status);
}

/**
 * @brief Execute an external command in a new process.
 *
 * Forks and executes an external command with redirections. If the command is
 * only a redirection, it exits with success. Parent waits and returns status.
 *
 * @return Exit status of child or ERROR if fork failed.
 */
static int	handle_external_command(char **args, t_shell *shell, t_ast *node,
			int heredoc_pipe_read_fd)
{
	pid_t	pid;
	int		status;

	pid = create_process();
	if (pid == -1)
	{
		free(args);
		return (ERROR);
	}
	if (pid == 0)
	{
		apply_redirections(node, heredoc_pipe_read_fd);
		if (!cmd_name_is_redir(node->cmd->cmd_name))
			status = execute_external_cmd(args, shell);
		else
			status = SUCCESS;
		_exit(status);
	}
	return (wait_for_process(pid));
}

/**
 * @brief Execute an AST node in a child process as part of a pipeline.
 *
 * Called after fork(), this function executes command or non-command
 * AST nodes inside a pipeline and terminates the process with _exit().
 *
 * @param node      AST node to execute.
 * @param shell     Shell context (copied in child process).
 * @param pipe_fd   Pipe file descriptor (read or write end).
 * @param pipe_type Pipe direction indicator.
 */
void	execute_in_child(t_ast *node, t_shell *shell, int heredoc_pipe_read_fd)
{
	char	**args;
	int		status;

	if (!node)
		_exit(ERROR);
	if (node->type == CMD)
	{
		if (!node->cmd)
			_exit(ERROR);
		args = prepare_cmd_for_execution(node->cmd, shell);
		if (!args)
			_exit(ERROR);
		if (is_builtin(args[0]))
			_exit(execute_builtin_command(args, shell, node,
					heredoc_pipe_read_fd));
		apply_redirections(node, heredoc_pipe_read_fd);
		if (!cmd_name_is_redir(node->cmd->cmd_name))
			status = execute_external_cmd(args, shell);
		else
			status = SUCCESS;
		free(args);
		_exit(status);
	}
	else
		_exit(execute_ast(node, shell));
}

/**
 * @brief Execute a command AST node in the main shell process.
 *
 * Handles argument preparation, heredocs, redirections, and executes
 * builtins or external commands outside of pipelines.
 *
 * @param node  AST node of type CMD.
 * @param shell Shell context structure.
 *
 * @return Exit status of the executed command or ERROR on failure.
 */
static int	run_cmd_with_mode(char **args, t_ast *node, t_shell *shell,
			int heredoc_fd)
{
	if (is_builtin(args[0]))
		return (execute_builtin_command(args, shell, node, heredoc_fd));
	return (handle_external_command(args, shell, node, heredoc_fd));
}

int	execute_command_node(t_ast *node, t_shell *shell)
{
	char	**args;
	int		status;
	int		saved_std_fds[2];
	int		heredoc_pipe_read_fd;

	heredoc_pipe_read_fd = -1;
	if (!node || !node->cmd || !node->cmd->cmd_name)
		return (ERROR);
	args = prepare_cmd_for_execution(node->cmd, shell);
	save_std_fds(saved_std_fds);
	if (!args)
		return (ERROR);
	heredoc_pipe_read_fd = handle_heredocs(node->cmd->redirs, shell);
	if (heredoc_pipe_read_fd == -1 && g_signal_received == SIGINT)
		return (130);
	status = run_cmd_with_mode(args, node, shell, heredoc_pipe_read_fd);
	restore_std_fds(saved_std_fds[0], saved_std_fds[1]);
	free(args);
	if (heredoc_pipe_read_fd != -1)
		close(heredoc_pipe_read_fd);
	return (status);
}
