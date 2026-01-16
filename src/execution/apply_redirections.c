/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:22:44 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:38 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Apply input redirection (<)
 * 
 * @param redir Redirection structure with file to read from
 * @return SUCCESS on success, ERROR on failure
 */
int	apply_redir_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
		return (ERROR);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		printf("minishell: dup2 failed: %s\n", strerror(errno));
		close(fd);
		return (ERROR);
	}
	if (close(fd) == -1)
	{
		printf("minishell: close failed: %s\n", strerror(errno));
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Apply append redirection (>>)
 * 
 * @param redir Redirection structure with file to append to
 * @return SUCCESS on success, ERROR on failure
 */
int	apply_redir_app(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		printf("minishell: dup2 failed: %s\n", strerror(errno));
		close(fd);
		return (ERROR);
	}
	if (close(fd) == -1)
	{
		printf("minishell: close failed: %s\n", strerror(errno));
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Apply output redirection (>)
 * 
 * @param redir Redirection structure with file to write to
 * @return SUCCESS on success, ERROR on failure
 */
int	apply_redir_out(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		printf("minishell: dup2 failed: %s\n", strerror(errno));
		close(fd);
		return (ERROR);
	}
	if (close(fd) == -1)
	{
		printf("minishell: close failed: %s\n", strerror(errno));
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Apply single redirection based on type
 * 
 * @param redir Redirection to apply
 * @param heredoc_pipe_read_fd File descriptor for heredoc input
 * @return SUCCESS on success, ERROR on failure
 */
static int	apply_single_redir(t_redir *redir, int heredoc_pipe_read_fd)
{
	int	status;

	if (redir->type == REDIR_OUT)
		status = apply_redir_out(redir);
	else if (redir->type == APPEND)
		status = apply_redir_app(redir);
	else if (redir->type == REDIR_IN)
		status = apply_redir_in(redir);
	else if (redir->type == HEREDOC)
	{
		dup2(heredoc_pipe_read_fd, STDIN_FILENO);
		close(heredoc_pipe_read_fd);
		return (SUCCESS);
	}
	else
		return (SUCCESS);
	return (status);
}

/**
 * @brief Apply all redirections for a command
 * 
 * @param cmd_node AST node containing command with redirections
 * @param heredoc_pipe_read_fd File descriptor for heredoc input
 * @return SUCCESS on success, ERROR on failure
 */
int	apply_redirections(t_ast *cmd_node, int heredoc_pipe_read_fd)
{
	int		status;
	t_redir	*curr_redir;

	curr_redir = cmd_node->cmd->redirs;
	while (curr_redir)
	{
		status = apply_single_redir(curr_redir, heredoc_pipe_read_fd);
		if (status == ERROR)
			return (status);
		curr_redir = curr_redir->next;
	}
	return (SUCCESS);
}
