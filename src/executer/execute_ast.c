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
static char	**convert_args_to_array(t_cmd *cmd);
static int	execute_command_node(t_ast *node, t_shell *shell);
static int	execute_pipe_node(t_ast *node, t_shell *shell);
int			cmd_name_is_redir(char *cmd_name);

/**
 * @brief Convert argument linked list to NULL-terminated array
 *
 * Converts the t_arg linked list to an array format needed for execution.
 * First element is the command name, followed by arguments.
 *
 * @param cmd The command structure containing name and args
 * @return NULL-terminated array of strings, NULL on error
 */
static char	**convert_args_to_array(t_cmd *cmd)
{
	char	**args;
	t_arg	*current;
	int		count;
	int		i;

	if (!cmd || !cmd->cmd_name)
		return (NULL);
	// Count arguments (cmd->args includes cmd_name as first element)
	count = 0;
	current = cmd->args;
	while (current)
	{
		count++;
		current = current->next;
	}
	// Allocate array
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	// Fill array directly from args list (which includes cmd_name)
	i = 0;
	current = cmd->args;
	while (current)
	{
		args[i++] = current->value;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void	save_std_fds(int *saved_std_fds)
{
	saved_std_fds[0] = dup(STDIN_FILENO);
	saved_std_fds[1] = dup(STDOUT_FILENO);
}
void restore_std_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}


/**
 * @brief Execute a simple command node
 *
 * Executes a command by checking if it's a builtin or external command.
 * Builtins are executed in the current process, external commands would
 * require fork/exec (to be implemented).
 *
 * Variable expansion is performed before command execution.
 *
 * @param node The AST node containing the command
 * @param shell The shell state structure
 * @return Exit status of the command
 */
static int	execute_command_node(t_ast *node, t_shell *shell)
{
	char	**args;
	int		status;
	int		saved_std_fds[2];
	t_arg	*expanded_args;

	if (!node || !node->cmd || !node->cmd->cmd_name)
		return (ERROR);
	// Expand variables in arguments and redirections
	expand_cmd_args(node->cmd->args, shell);
	expand_redirection_files(node->cmd->redirs, shell);
	// Expand wildcards in arguments
	expanded_args = expand_wildcards_in_args(node->cmd->args);
	if (expanded_args)
	{
		free_args_list(node->cmd->args);
		node->cmd->args = expanded_args;
		// Update cmd_name to first arg after wildcard expansion
		if (expanded_args->value)
			node->cmd->cmd_name = expanded_args->value;
	}
	args = convert_args_to_array(node->cmd);
	if (!args)
		return (ERROR);
	status = SUCCESS;
	if (is_builtin(args[0]))
	{
		save_std_fds(saved_std_fds);
		status = apply_redirections(node);//altera me os fds, depois de executar a funcao builtin devo restautar os fds para os originais, no caso de funcoes externas nao preciso de restaurar porque estao num fork
		//so executa se apply redirections correu bem entao tenho que retornar o status dentro dessa funcao e ja definir o status code
		if (status == SUCCESS)
			status = execute_builtin(args, shell);
		restore_std_fds(saved_std_fds[0], saved_std_fds[1]);
	}
	else
	{
		pid_t pid = create_process();

		if (pid == -1)
		{
			free(args);
			return (ERROR);
		}
		if (pid == 0)
		{
			// in child process
			int exit_code;
			int redir_status;
			
			redir_status = apply_redirections(node);
			if (redir_status == ERROR)
				_exit(1); // exit immediately if redirections fail
			if (!cmd_name_is_redir(node->cmd->cmd_name))
				exit_code = execute_external_cmd(args, shell);
			else
				exit_code = SUCCESS;
			_exit(exit_code);
		}
		else
		{
			// Parent process
			status = wait_for_process(pid);
		}
	}
	// Free args array (not the strings, they belong to the AST)
	free(args);
	return (status);
}
int cmd_name_is_redir(char *cmd_name)
{
	if (ft_strcmp(cmd_name, ">>") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "<<") == 0)
		return 1;
	if (ft_strcmp(cmd_name, ">") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "<") == 0)
		return 1;
	return 0;
}

/**
 * @brief Execute a single command in a pipeline
 *
 * @param node Command node to execute
 * @param shell Shell state
 */
static void	execute_in_child(t_ast *node, t_shell *shell)
{
	char	**args;
	t_arg	*expanded_args;

	if (!node)
		_exit(ERROR);
	if (node->type == CMD)
	{
		if (!node->cmd)
			_exit(ERROR);
		expand_cmd_args(node->cmd->args, shell);
		expand_redirection_files(node->cmd->redirs, shell);
		// Expand wildcards in arguments
		expanded_args = expand_wildcards_in_args(node->cmd->args);
		if (expanded_args)
		{
			free_args_list(node->cmd->args);
			node->cmd->args = expanded_args;
			if (expanded_args->value)
				node->cmd->cmd_name = expanded_args->value;
		}
		args = convert_args_to_array(node->cmd);
		if (!args)
			_exit(ERROR);
		if (apply_redirections(node) == ERROR)
			_exit(1); // exit immediately if redirections fail
		if (is_builtin(args[0]))
			_exit(execute_builtin(args, shell));
		else if (!cmd_name_is_redir(node->cmd->cmd_name))
		{
			int exit_code = execute_external_cmd(args, shell);
			free(args);
			_exit(exit_code);
		}
		free(args);
		_exit(SUCCESS);
	}
	else
	{
		_exit(execute_ast(node, shell));
	}
}

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
