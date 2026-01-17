/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:40:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/03 17:22:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../structs.h"


/**
 * @brief Redirects STDIN/STDOUT to the appropriate pipes in the pipeline.
 *
 * Connects the process to the "tubing" created by the parent:
 * - If not the first command (pipe_indice > 0): Reads from the previous pipe's read end.
 * - If not the last command (pipe_indice < numb_of_pipes): Writes to the current pipe's write end.
 *
 * @param pipes The array containing all pipe file descriptors.
 * @param pipe_indice The current command's position (0-indexed).
 * @param numb_of_pipes Total number of pipes in the current pipeline level.
 */
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


/**
 * @brief Spawns a child process to execute a single command within a pipeline.
 *
 * This function manages the lifecycle of a command node:
 * 1. Sets up heredoc signal handlers (for Ctrl+C during input).
 * 2. Handles heredocs to obtain reading FD.
 * 3. Checks if Ctrl+C was pressed; skips execution if so.
 * 4. Restores normal signal handlers.
 * 5. Forks a child process:
 * - Child: Connects to pipes, executes.
 * - Parent: Closes the heredoc FD immediately.
 *
 * @param curr_node The AST node containing command and redirection data.
 * @param pipes Array of pipeline file descriptors.
 * @param pipe_indice The command's position in the pipeline.
 * @param numb_of_pipes Total pipes in the current pipeline.
 * @param shell Pointer to the shell state.
 */
void handle_cmd(t_ast *curr_node, int *pipes, int pipe_indice, int numb_of_pipes,
		 t_shell *shell, int *spawned)
{
	int		heredoc_pipe_read_fd;
	int		pid;

	setup_signals_heredoc();
	heredoc_pipe_read_fd = handle_heredocs(curr_node->cmd->redirs, shell);
	if (g_signal_received == SIGINT)
	{
		if (heredoc_pipe_read_fd != -1)
			close(heredoc_pipe_read_fd);
		return ;
	}
	setup_signals_executing();
	pid = fork();
//	if (pid == -1)
//		return (perror("minishell: fork"), ERROR);

	if (pid == 0) // child
	{
		configure_stds(pipes, pipe_indice, numb_of_pipes);
		close_all_pipes(pipes, numb_of_pipes);
		execute_in_child(curr_node, shell, heredoc_pipe_read_fd);
		_exit(ERROR);
	}
	if (heredoc_pipe_read_fd != -1)
		close(heredoc_pipe_read_fd);
	(*spawned)++;
}
/**
 * @brief Spawns a child process to execute a subshell within a pipeline.
 * * Treats a logic block (AND/OR) as a single stage in the pipeline:
 * 1. Forks a child process.
 * 2. Inside the child:
 * - Connects the subshell's standard streams to the parent's pipeline.
 * - Closes parent pipe FDs to ensure EOF propagation.
 * - Re-enters the main AST executor (execute_ast) to handle internal logic.
 * * This allows complex commands like `a | (b && c) | d` to function correctly.
 * * @param curr_node The AST node representing the subshell (AND/OR).
 * @param pipes Array of pipeline file descriptors.
 * @param pipe_indice The subshell's position in the pipeline.
 * @param numb_of_pipes Total pipes in the current pipeline.
 */
void handle_subshell(t_ast *curr_node, int *pipes, int pipe_indice,
	int numb_of_pipes, t_shell *shell, int *spawned)
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
	(*spawned)++;
}

/**
 * @brief Linearly traverses the AST to spawn pipeline stages.
 *
 * Flattens the pipe hierarchy by visiting all nodes at the current level.
 * - For PIPE nodes: Recursively visits left/right branches and increments
 * the pipe index to move data forward through the array.
 * - For CMD/Subshells: Acts as a leaf. Spawns a process (via handle_cmd
 * or handle_subshell) that uses the current pipe_indice for its FDs.
 *
 * Note: Subshells (AND/OR) are treated as single units, preserving their
 * internal logic while maintaining their position in the parent pipeline.
 */
void traverse_and_execute(t_ast *node, int *pipes, int *pipe_indice,
	int num_pipes, t_shell *shell, int *spawned)//this runs always in a pipeline
{
    if (!node)
		return;
	if (g_signal_received == SIGINT)
		return;

	if (node->type == PIPE) {
		// Primeiro vai à esquerda (pode haver outro PIPE ou um CMD)
		traverse_and_execute(node->left, pipes, pipe_indice, num_pipes, shell, spawned);
		//depois de executar o comando da esquerda do pipe, atualizo o pipe index, para o comando da direita do pipe escrever para o pipe da frente (pipe indice e sempre o pipe a escrever)
		(*pipe_indice)++;
		// Depois vai à direita
		traverse_and_execute(node->right, pipes, pipe_indice, num_pipes, shell, spawned);
	}
	else {
		if (node -> type == CMD)
			handle_cmd(node, pipes, *pipe_indice, num_pipes, shell, spawned);
		else if(node -> type == AND || node -> type == OR) //configurar os stds aqui, e tratar a subshell como um processo child, com os seus proprios stds configurados internamente mas que lem e escrevem nos stds do processo pai
			handle_subshell(node, pipes, *pipe_indice, num_pipes, shell, spawned); //esta subshell pode ter pipelines proprias dentro
	}
}

/**
 * @brief Counts the total number of pipes in the current pipeline sequence.
 *
 * This function determines the size of the pipe array needed for a single
 * pipeline. It stops counting at CMD nodes or logical operators (AND/OR),
 * effectively flattening the current pipeline while treating subshells
 * as single, isolated units.
 *
 * @param node The current AST node being evaluated.
 * @return int The number of PIPE nodes found in the current branch.
 */
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
/**
 * @brief Synchronizes the shell by waiting for all pipeline processes.
 *
 * Iterates through the number of spawned commands, collecting their exit statuses.
 * It uses POSIX macros to decode the status:
 * - WIFEXITED: Captures normal exit codes.
 * - WIFSIGNALED: Handles processes killed by signals (e.g., Ctrl+C),
 * offsetting by 128 per standard shell behavior.
 * * @param num_cmds Total processes to wait for (numb_of_pipes + 1).
 * @return int The exit status of the last process collected.
 */
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
/**
 * @brief Orchestrates the execution of a multi-stage pipeline.
 *
 * 1. Calculates total pipes needed (excluding nested subshell pipes).
 * 2. Allocates and initializes the pipe array.
 * 3. Traverses the AST to spawn all sibling processes in parallel.
 * 4. Closes all parent pipe FDs to trigger EOF in children.
 * 5. Waits for all children and cleans up memory.
 *
 * @param node The current PIPE node of the AST.
 * @param shell The shell state structure.
 * @return int The final exit status of the pipeline's last command.
 */
static int execute_pipeline(t_ast *node, t_shell *shell)
{
	int		*pipes;
	int		numb_of_pipes;
	int		pipe_indice;
	int		spawned;
	int		final_status;

	pipe_indice = 0;
	spawned = 0;
	numb_of_pipes = calc_numb_pipes(node); //calcula o nmb of pipes desta pipeline.
	pipes = create_pipes(numb_of_pipes);
	//ao percorrer a tree envio o pipe e o indice do pipe;
	traverse_and_execute(node, pipes, &pipe_indice, numb_of_pipes, shell, &spawned);
	// CRUCIAL: Fechar os pipes no PAI antes de esperar
	// Se o pai mantiver os pipes abertos, os filhos nunca recebem EOF
	close_all_pipes(pipes, numb_of_pipes);
	// Espera pelos filhos e limpa a memória
	final_status = wait_for_pipeline(spawned);
	if (g_signal_received == SIGINT)
		final_status = 130;
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
