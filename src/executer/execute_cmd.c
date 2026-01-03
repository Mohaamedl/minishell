/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:13:22 by framiran          #+#    #+#             */
/*   Updated: 2025/12/15 18:36:36 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <minishell.h>

/**
 * @brief Execute a builtin command.
 *
 * Applies any necessary redirections and then attempts to execute the builtin
 * command. If the redirections are applied successfully, the builtin command is
 * executed. The function handles cases where the redirection involves heredocs.
 */
int execute_builtin_command(char **args, t_shell *shell, t_ast *node, int heredoc_pipe_read_fd)
{
	int status;
	status = apply_redirections(node, heredoc_pipe_read_fd);  // heredoc_pipe_read_fd pode ser -1 ou um fd válido
	if (status == SUCCESS)
		status = execute_builtin(args, shell);
	return status;
}


/**
 * @brief Execute an external command in a new process.
 *
 * Creates a new process to execute an external command. Redirections are applied
 * before execution, and the function handles both command execution and redirection.
 * It waits for the child process to finish and returns its exit status.
 *
 * If the command is just a redirection (without execution), it does nothing and
 * returns success.
 *
 * @return The exit status of the executed command or `ERROR` if the process creation failed.
 */
int handle_external_command(char **args, t_shell *shell, t_ast *node, int heredoc_pipe_read_fd)
{
	pid_t pid = create_process();
	if (pid == -1)
	{
		free(args);
		return ERROR;
	}
	if (pid == 0)
	{
		int exit_code;
		apply_redirections(node, heredoc_pipe_read_fd);
		if (!cmd_name_is_redir(node->cmd->cmd_name))
			exit_code = execute_external_cmd(args, shell);
		else
			exit_code = SUCCESS;  // Caso seja apenas um redirecionamento, não executa nada
		_exit(exit_code);
	}
	else
		return wait_for_process(pid);  // Espera pelo processo filho e retorna seu status
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
void	execute_in_child(t_ast *node, t_shell *shell) //pipe fd can be the writing or reading end of the pipe(that comes from the "|" fork())
{
	char	**args;
	int		heredoc_pipe_read_fd;

	heredoc_pipe_read_fd = -1;
	if (!node)
		_exit(ERROR);
	if (node->type == CMD)
	{
		if (!node->cmd)
			_exit(ERROR);
		args = prepare_cmd_for_execution(node->cmd, shell);
		if (!args)
			_exit(ERROR);
		heredoc_pipe_read_fd = handle_heredocs(node->cmd->redirs);//-1 if there is no heredocs or interrupted
		if (heredoc_pipe_read_fd == -1 && g_signal_received == SIGINT)
			_exit(130); // Exit with 130 (128 + SIGINT) when heredoc interrupted
		if (is_builtin(args[0]))
			_exit(execute_builtin_command(args, shell, node, heredoc_pipe_read_fd));//execute_builtin() applies redirections
		apply_redirections(node,heredoc_pipe_read_fd);
		if (!cmd_name_is_redir(node->cmd->cmd_name))
		{
			int exit_code = execute_external_cmd(args, shell);
			free(args);
			_exit(exit_code);
		}
		free(args);
		_exit(SUCCESS);
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
	heredoc_pipe_read_fd = handle_heredocs(node->cmd->redirs);//-1 if invalid or interrupted
	if (heredoc_pipe_read_fd == -1 && g_signal_received == SIGINT)
		return (130); // Return 130 (128 + SIGINT) when heredoc interrupted
	if (is_builtin(args[0]))
		status = execute_builtin_command(args, shell, node, heredoc_pipe_read_fd);
	else
		status = handle_external_command(args, shell, node, heredoc_pipe_read_fd);
	restore_std_fds(saved_std_fds[0], saved_std_fds[1]);
	free(args);
	if (heredoc_pipe_read_fd != -1)
		close(heredoc_pipe_read_fd);
	return (status);
}

