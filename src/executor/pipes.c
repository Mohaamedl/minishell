/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocate memory for pipe array
 *
 * @param num_pipes Number of pipes to create
 * @return int* Allocated array, NULL on error
 */
static int	*allocate_pipes(int num_pipes)
{
	int	*pipes;

	pipes = malloc(sizeof(int) * num_pipes * 2);
	if (!pipes)
		perror("minishell: malloc");
	return (pipes);
}

/**
 * @brief Create pipes for a pipeline
 *
 * Creates (n-1) pipes for n commands in a pipeline.
 *
 * @param num_commands Number of commands in the pipeline
 * @return int* Array of pipe file descriptors (caller must free), NULL on error
 */
int	*create_pipes(int num_commands)
{
	int	*pipes;
	int	num_pipes;
	int	i;

	if (num_commands < 2)
		return (NULL);
	num_pipes = num_commands - 1;
	pipes = allocate_pipes(num_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_pipes)
	{
		if (pipe(&pipes[i * 2]) == -1)
		{
			perror("minishell: pipe");
			close_all_pipes(pipes, i);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

/**
 * @brief Redirect stdin from previous pipe
 *
 * @param pipes Array of pipe file descriptors
 * @param index Index of current command
 */
static void	setup_input_pipe(int *pipes, int index)
{
	if (index > 0)
	{
		if (dup2(pipes[(index - 1) * 2], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * @brief Redirect stdout to next pipe
 *
 * @param pipes Array of pipe file descriptors
 * @param index Index of current command
 * @param total Total number of commands in pipeline
 */
static void	setup_output_pipe(int *pipes, int index, int total)
{
	if (index < total - 1)
	{
		if (dup2(pipes[index * 2 + 1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * @brief Setup pipe file descriptors for a command in a pipeline
 *
 * For command at index i in a pipeline of total commands:
 * - If not first command: redirect stdin from previous pipe
 * - If not last command: redirect stdout to next pipe
 * - Close all pipe file descriptors (both used and unused)
 *
 * @param pipes Array of pipe file descriptors
 * @param index Index of current command (0-based)
 * @param total Total number of commands in pipeline
 */
void	setup_pipe_fds(int *pipes, int index, int total)
{
	if (!pipes)
		return ;
	setup_input_pipe(pipes, index);
	setup_output_pipe(pipes, index, total);
	close_all_pipes(pipes, total - 1);
}
