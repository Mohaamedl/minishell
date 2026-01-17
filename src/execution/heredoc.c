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

#include "minishell.h"

void	read_heredoc_to_pipe(int *pipefd, char *terminator,
		int content_expands, t_shell *shell)
{
	char	*line;

	setup_signals_heredoc();
	line = readline("heredoc> ");
	while (line)
	{
		if (g_signal_received == SIGINT)
		{
			free(line);
			break ;
		}
		if (ft_strcmp(line, terminator) == 0)
		{
			free(line);
			break ;
		}
		line = process_heredoc_line(line, content_expands, shell);
		write_heredoc_line(pipefd[1], line);
		free(line);
		line = readline("heredoc> ");
	}
	rl_event_hook = NULL;
	setup_signals_interactive();
}

/**
 * @brief Process a single heredoc redirection.
 *
 * Reads heredoc input and writes to pipe. Returns -1 if interrupted by signal.
 *
 * @param pipefd The pipe file descriptors.
 * @param redir The heredoc redirection structure.
 * @param shell The shell state.
 * @return 0 on success, -1 if interrupted.
 */
static int	process_heredoc(int pipefd[2], t_redir *redir, t_shell *shell)
{
	read_heredoc_to_pipe(pipefd, redir->file,
		redir->file_name_is_expandable, shell);
	if (g_signal_received == SIGINT)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (0);
}

static int	init_heredoc_pipe(int pipefd[2], int *read_fd)
{
	if (pipe(pipefd) == -1)
		return (-1);
	*read_fd = pipefd[0];
	return (0);
}

static int	process_all_heredocs(t_redir *first_redir, int pipefd[2],
			t_shell *shell)
{
	t_redir	*curr_redir;

	curr_redir = first_redir;
	while (curr_redir != NULL)
	{
		if (curr_redir->type == HEREDOC)
		{
			if (process_heredoc(pipefd, curr_redir, shell) == -1)
				return (-1);
		}
		curr_redir = curr_redir->next;
	}
	return (0);
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
 * @return The file descriptor of the read end of the pipe containing all
 *         heredoc data, or `-1` if no heredocs or if interrupted by signal.
 */

int	handle_heredocs(t_redir *first_redir, t_shell *shell)
{
	int	pipefd[2];
	int	read_fd;

	if (!has_heredocs(first_redir))
		return (-1);
	if (init_heredoc_pipe(pipefd, &read_fd) == -1)
		return (-1);
	if (process_all_heredocs(first_redir, pipefd, shell) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	return (read_fd);
}
