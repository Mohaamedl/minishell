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


void configure_stds(int pipe_indice)
{
	if (pipe_indice > 0) 
		dup2(pipes[(i - 1) * 2], STDIN_FILENO);

	if (pipe_indice < num_cmds - 1) //escreve para o pipe a frente
		dup2(pipes[i * 2 + 1], STDOUT_FILENO);
}



void handle_cmd(t_ast *curr_node, int *pipe, int pipe_indice)
{
	int		heredoc_pipe_read_fd;
	int		pid;

	heredoc_pipe_read_fd = handle_heredocs(curr_node->cmd->redirs);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), ERROR);

	if (pid == 0) // child
	{
		configure_stds(pipe_indice, numb _of_pipes);//nmb_of_pipes is important to check the case of the last and first cmd of pipeline
		// fechar todos os pipes no child          //(first cmd does not read from a pipe and last does not write to pipe)
		close_all_pipes(pipes, (num_cmds - 1));
		// executar comando
		execute_in_child(curr_node, shell, heredoc_pipe_read_fd);
		_exit(ERROR);
	}
}

void handle_subshell()
{
	int		pid;
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), ERROR);

	if (pid == 0) // child
	{
		configure_stds(pipe_indice, numb _of_pipes);//nmb_of_pipes is important to check the case of the last and first cmd of pipeline
		// fechar todos os pipes no child          //(first cmd does not read from a pipe and last does not write to pipe)
		close_all_pipes(pipes, (num_cmds - 1));
		// executar comando
		execute_ast();
		_exit(ERROR);
	}
}	


void traverse_and_execute(t_ast *node, int *pipes, int *i, int num_pipes)//this runs always in a pipeline
{
    if (!node) return;

    if (node->type == PIPE) {
        // Primeiro vai à esquerda (pode haver outro PIPE ou um CMD)
        traverse_and_execute(node->left, pipes, i, num_pipes);
        // Depois vai à direita
        traverse_and_execute(node->right, pipes, i, num_pipes);
    } 
    else {
		if (node -> type == CMD)
        	handle_cmd(node, pipes, *i, num_pipes);
		else if(node -> type == AND || node -> type == OR)
			handle_subshell(node, pipes, *i, num_pipes); //esta subshell pode ter pipelines proprias dentro
        (*i)++; // Avança para o próximo comando no pipeline
    }
}	

//1- contar todos os pipes da pipeline : "a | (b | c || d | e) | f"  tem apenas 2 pipes. 
//2 - percorrer a pipeline, se for um comando dou setup para ler do pipe atras e escrever no da frente.
//    se for uma expressao(and ou or chamo a funcao execute_ast() depois de setar os std fds),
//  tudo os que esta entre && ou || e uma pipeline ou um comando simples. 
static int execute_pipeline(t_ast *node, t_shell *shell)
{
	int		*pipes;
	int		numb_of_pipes;
	int		status;
	int		last_status;
	t_ast	*curr_node;

	numb_of_pipes = calc_nmb_pipes(node); //node em principio e um pipe, calcula o nmb of pipes desta pipeline.
	pipes = create_pipes(numb_of_pipes);
	//ao percorrer a tree envio o pipe e o indice do pipe;
	transverse_tree_and_execute();
	//fechar pipes
	//retornar status 
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
