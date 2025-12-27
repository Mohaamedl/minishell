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
static int	execute_pipeline(t_ast *node, t_shell *shell);




static int execute_pipeline(t_ast *node, t_shell *shell)
{
	t_ast	*cmds;
	int		num_cmds;
	int		*pipes;
	pid_t	*pids;
	int		i;
	int		status;
	int		last_status;
	int		heredoc_pipe_read_fd:
	t_ast	*curr_node;

	cmds = get_cmds_from_pipeline(node, &num_cmds);
	pipes = create_pipes(num_cmds);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
		return (ERROR);
	i = 0;
	while (i < num_cmds)
	{
		curr_node = get_node_by_index(cmds,i);
		heredoc_pipe_read_fd = handle_heredocs(curr_node->cmd->redirs);
		pids[i] = fork();
		if (pids[i] == -1)
			return (perror("minishell: fork"), ERROR);

		if (pids[i] == 0) // child
		{
			// configuro o stdin para ler do pipe que esta atras
			if (i > 0) //se nao for o primeiro cmd, le do pipe atras
				dup2(pipes[(i - 1) * 2], STDIN_FILENO);

			// stdout
			if (i < num_cmds - 1) //escreve para o pipe a frente
				dup2(pipes[i * 2 + 1], STDOUT_FILENO);

			// fechar todos os pipes no child
			close_all_pipes(pipes, (num_cmds - 1));

			// executar comando
			execute_in_child(curr_node, shell, heredoc_pipe_read_fd);
			_exit(ERROR);
		}
		i++;
	}

	// 4️⃣ Fechar pipes no pai
	close_all_pipes(pipes, (num_cmds - 1));

	// 5️⃣ Esperar filhos
	last_status = 0;
	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else
				last_status = ERROR;
		}
		i++;
	}
	free(pipes);
	free(pids);
	return (last_status);
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
		status = execute_pipeline(node, shell);
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
