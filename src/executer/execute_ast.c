/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:40:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/03 14:34:29 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../structs.h"


//pipe_indice = pipe a se escrever
void configure_stds(int *pipes, int pipe_indice, int numb_of_pipes)
{
	int read_fd;
	int write_fd;
	//fd[0] (Leitura)
	//fd[1] (Escrita)
	read_fd = (pipe_indice - 1) * 2; //é o read_fd do pipe anterior
	write_fd = (pipe_indice * 2) +  1; // e o write fd do pipe a se escrever(pipe_indice)
	if (pipe_indice > 0) //pipe indice descreve o pipe a se escrever, se por o primeiro pipe, e o primeiro cmd entao nao leio de nenhum pipe
		dup2(pipes[read_fd], STDIN_FILENO);

	if (pipe_indice < numb_of_pipes )
		dup2(pipes[write_fd], STDOUT_FILENO);
}



void handle_cmd(t_ast *curr_node, int *pipes, int pipe_indice, int numb_of_pipes ,t_shell *shell)
{
	int		heredoc_pipe_read_fd;
	int		pid;

	heredoc_pipe_read_fd = handle_heredocs(curr_node->cmd->redirs);
	pid = fork();
//	if (pid == -1)
//		return (perror("minishell: fork"), ERROR);

	if (pid == 0) // child
	{
		configure_stds(pipes, pipe_indice, numb_of_pipes);//nmb_of_pipes is important to check the case of the last and first cmd of pipeline
		// fechar todos os pipes no child          //(first cmd does not read from a pipe and last does not write to pipe)
		close_all_pipes(pipes, numb_of_pipes);
		// executar comando
		execute_in_child(curr_node, shell, heredoc_pipe_read_fd);
		_exit(ERROR);
	}
	if (heredoc_pipe_read_fd != -1)
		close(heredoc_pipe_read_fd);
}

void handle_subshell(t_ast *curr_node, int *pipes, int pipe_indice, int numb_of_pipes, t_shell *shell)
{
	int		pid;
	pid = fork();
//	if (pid == -1)
//		return (perror("minishell: fork"), ERROR);

	if (pid == 0) // child
	{
		configure_stds(pipes, pipe_indice, numb_of_pipes);//nmb_of_pipes is important to check the case of the last and first cmd of pipeline
		// fechar todos os pipes no child          //(first cmd does not read from a pipe and last does not write to pipe)
		close_all_pipes(pipes, numb_of_pipes);
		execute_ast(curr_node, shell);
		_exit(ERROR);
	}
}

//se encontro um && ou || numa pipeline e garantidamente um subshell, nao conto os pipes desse subshell
//porque se fosse um  && ou || divisor de pipelines teria sifdo apanhado em execute ast:
// a | b && c | d  vs  a |( b && c )| d
void traverse_and_execute(t_ast *node, int *pipes, int *pipe_indice, int num_pipes, t_shell *shell)//this runs always in a pipeline
{
    if (!node)
		return;

	if (node->type == PIPE) {
		// Primeiro vai à esquerda (pode haver outro PIPE ou um CMD)
		traverse_and_execute(node->left, pipes, pipe_indice, num_pipes, shell);
		//depois de executar o comando da esquerda do pipe, atualizo o pipe index, para o comando da direita do pipe escrever para o pipe da frente (pipe indice e sempre o pipe a escrever)
		(*pipe_indice)++;
		// Depois vai à direita
		traverse_and_execute(node->right, pipes, pipe_indice, num_pipes,shell);
	}
	else {
		if (node -> type == CMD)
			handle_cmd(node, pipes, *pipe_indice, num_pipes, shell);
		else if(node -> type == AND || node -> type == OR) //configurar os stds aqui, e tratar a subshell como um processo child, com os seus proprios stds configurados internamente mas que lem e escrevem nos stds do processo pai
			handle_subshell(node, pipes, *pipe_indice, num_pipes, shell); //esta subshell pode ter pipelines proprias dentro
	}
}


int calc_numb_pipes(t_ast *node)
{
	int count;
	count = 0;

	if (node ->type == CMD)
		return (count);
	if (node -> type == AND || node -> type == OR)
		return (count);
	if(node ->type == PIPE)
		count++;
	count = count + calc_numb_pipes(node -> left);
	count = count + calc_numb_pipes(node -> right);
	return (count);
}

int wait_for_pipeline(int num_cmds)
{
	int i;
	int status;
	int last_status;
	i = 0;
	last_status = 0;
	while (i < num_cmds)
	{
		// wait(NULL) espera por qualquer processo filho terminar
		// mas aqui usamos &status para capturar o valor de saída
		if (wait(&status) > 0)
		{
			// Usamos as macros POSIX para decifrar o status
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status); // Caso o comando morra por sinal (ex: Ctrl+C)
		}
		i++;
	}
	return (last_status);
}

//1- contar todos os pipes da pipeline : "a | (b | c || d | e) | f"  tem apenas 2 pipes.
//2 - percorrer a pipeline, se for um comando dou setup para ler do pipe atras e escrever no da frente.
//    se for uma expressao(and ou or chamo a funcao execute_ast() depois de setar os std fds),
//  tudo os que esta entre && ou || e uma pipeline ou um comando simples.
static int execute_pipeline(t_ast *node, t_shell *shell)
{
	int		*pipes;
	int		numb_of_pipes;
	int		pipe_indice;
	int		final_status;

	pipe_indice = 0;
	numb_of_pipes = calc_numb_pipes(node); //calcula o nmb of pipes desta pipeline.
	pipes = create_pipes(numb_of_pipes);
	//ao percorrer a tree envio o pipe e o indice do pipe;
	traverse_and_execute(node,pipes,&pipe_indice,numb_of_pipes,shell);
	// CRUCIAL: Fechar os pipes no PAI antes de esperar
	// Se o pai mantiver os pipes abertos, os filhos nunca recebem EOF
	close_all_pipes(pipes, numb_of_pipes);
	// Espera pelos filhos e limpa a memória
	final_status = wait_for_pipeline(numb_of_pipes + 1);
	free(pipes);
    return (final_status);
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
