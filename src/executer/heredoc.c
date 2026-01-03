/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:15:37 by framiran          #+#    #+#             */
/*   Updated: 2025/12/15 18:29:55 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <minishell.h>

int has_heredocs(t_redir *first_redir)
{
	t_redir *curr_redir;

	curr_redir = first_redir;
	while (curr_redir != NULL)
	{
		if(curr_redir -> type == HEREDOC)
			return (1);
		curr_redir = curr_redir -> next;
	}
	return (0);
}

void	read_heredoc_to_pipe(int *pipefd, char *terminator)
{
	char	*line;
	
	setup_signals_heredoc();
	line = readline("heredoc> ");
	while (line)
	{
		if (g_signal_received == SIGINT)
		{
			free(line);
			rl_event_hook = NULL;
			setup_signals_interactive();
			return ;
		}
		if (ft_strcmp(line, terminator) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
		line = readline("heredoc> ");
	}
	rl_event_hook = NULL;
	setup_signals_interactive();
	if (!line && g_signal_received == SIGINT)
		return ;
}

/**
 * @brief Handle all heredocs and write them to the same pipe.
 *
 * Checks for heredocs in the redirection list, creates a pipe, and writes
 * the heredoc input to the same pipe for all heredocs found. This is similar
 * to how Bash handles heredocs: it writes all heredoc content into a single
 * pipe and provides the read end of the pipe for use in command execution.
 *
 * If Ctrl+C (SIGINT) is pressed during heredoc input, the function will:
 * - Close the pipe
 * - Return -1 to signal interruption
 * - The caller should check g_signal_received and set exit status to 130
 *
 * @return The file descriptor of the read end of the pipe containing all heredoc data,
 *         or `-1` if no heredocs are present or if interrupted by signal.
 */

int handle_heredocs(t_redir *first_redir)
{
	t_redir *curr_redir;
	int heredoc_pipe_read_fd;
	int pipefd[2];

	heredoc_pipe_read_fd = -1;
	curr_redir = first_redir;
	if(has_heredocs(first_redir))
	{
		pipe(pipefd);
		heredoc_pipe_read_fd =  pipefd[0];
		while (curr_redir != NULL)
		{
			if(curr_redir -> type == HEREDOC)
			{
				read_heredoc_to_pipe(pipefd, curr_redir->file);
				if (g_signal_received == SIGINT)
				{
					close(pipefd[0]);
					close(pipefd[1]);
					return (-1);
				}
			}
			curr_redir = curr_redir -> next;
		}
		close(pipefd[1]); //close writing side after all heredocs wrote to the same pipe
	}
	return (heredoc_pipe_read_fd);
}
