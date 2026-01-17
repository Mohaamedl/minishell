/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/13 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
 * @brief Check if redirection list contains heredocs
 * 
 * @param first_redir First redirection in list
 * @return 1 if heredocs present, 0 otherwise
 */
int	has_heredocs(t_redir *first_redir)
{
	t_redir	*curr_redir;

	curr_redir = first_redir;
	while (curr_redir != NULL)
	{
		if (curr_redir->type == HEREDOC)
			return (1);
		curr_redir = curr_redir->next;
	}
	return (0);
}

/**
 * @brief Process heredoc line with optional variable expansion
 * 
 * @param line Input line
 * @param expand Whether to expand variables
 * @param shell Shell state for expansion
 * @return Processed line (may be original or expanded)
 */
char	*process_heredoc_line(char *line, int expand, t_shell *shell)
{
	char	*tmp;
	char	*result;

	if (!expand)
		return (line);
	tmp = line;
	result = expand_variables(line, shell);
	if (result != NULL)
		free(tmp);
	else
		result = tmp;
	return (result);
}

/**
 * @brief Write heredoc line to pipe with newline
 * 
 * @param fd File descriptor to write to
 * @param line Line to write
 */
void	write_heredoc_line(int fd, char *line)
{
	write(fd, line, strlen(line));
	write(fd, "\n", 1);
}
