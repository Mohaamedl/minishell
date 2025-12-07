/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:40:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 17:43:33 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../structs.h"

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
	char	**envp;
	char	**args;
	int		status;
	int		saved_std_fds[2];
	if (!node || !node->cmd || !node->cmd->cmd_name)
		return (ERROR);
	// Expand variables in arguments and redirections
	expand_cmd_args(node->cmd->args, shell);
	expand_redirection_files(node->cmd->redirs, shell);
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
		envp = env_list_to_array(shell->env_list);
		pid_t pid = fork();
		if (pid == 0)
		{ //no processo filho:
			apply_redirections(node);
			if(!cmd_name_is_redir(node->cmd->cmd_name))//se o cmd_name for uma redirecao aplico so a redirecao e nao corro nada ex: > outfile (cria o ficheiro outfile sem nada)
				execute_external_cmd(args,envp); //chama o excve e se nao funcionar da exit com um status code
			exit(0); //so para testar as redirecoes depois sera para remover
		}
		else //processo pai
		{
			printf("Entou no pai, o comando foi executado\n");
			free(envp); //ja nao preciso mais disto
			waitpid(pid, &status, 0); //nao preciso de resetar os fds no pai porque alterei os no processo filho e esse processo vai morrer com o execve
			//vou buscar o status que o filho retornou com waitpid(pid, &status, 0);
			return(WEXITSTATUS(status)); //aqui faremos uma manipulacao deste resultado para retornar SUCCESS, ERROR, etc
		}
	}
	// Free args array (not the strings, they belong to the AST)
	free(args);
	return (status);
}

/**
 * @brief Execute a pipe node
 *
 * Executes a pipeline by executing left and right sides.
 * Note: This is a simplified version. Full pipe implementation
 * with fork/exec will be added later.
 *
 * @param node The AST node with type PIPE
 * @param shell The shell state structure
 * @return Exit status of the last command in the pipeline
 */
static int	execute_pipe_node(t_ast *node, t_shell *shell)
{
	int	status;

	if (!node)
		return (ERROR);
	// For now, execute sequentially (not a real pipe)
	// TODO: Implement proper pipeline with fork, pipe, dup2
	if (node->left)
		execute_ast(node->left, shell);
	if (node->right)
		status = execute_ast(node->right, shell);
	else
		status = ERROR;
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
		// TODO: Implement AND logic (bonus)
		// Execute left, if success then execute right
		status = ERROR;
		fprintf(stderr, "minishell: AND operator not yet implemented\n");
	}
	else if (node->type == OR)
	{
		// TODO: Implement OR logic (bonus)
		// Execute left, if failure then execute right
		status = ERROR;
		fprintf(stderr, "minishell: OR operator not yet implemented\n");
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
