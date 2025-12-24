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

	//REDIR_IN,    // <
	//REDIR_OUT,   // >
	//APPEND,      // >>
	//HEREDOC,      // <<



int apply_redir_in(t_redir *redir)
{
	int fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n",redir->file, strerror(errno));
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

int apply_redir_app(t_redir *redir)
{
	int fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n",redir->file, strerror(errno));
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

int apply_redir_out(t_redir *redir)
{
	int fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n",redir->file, strerror(errno));
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
int	apply_redirections(t_ast *cmd_node, int heredoc_pipe_read_fd)
{
	int	status;

	t_redir *curr_redir = cmd_node -> cmd ->redirs;
	while(curr_redir)
	{
		if(curr_redir ->type == REDIR_OUT)
		{
			status = apply_redir_out(curr_redir);
			if(status == ERROR)
				return status;
		}
		if(curr_redir ->type == APPEND)
		{
			status = apply_redir_app(curr_redir);
			if(status == ERROR)
				return status;
		}
		if(curr_redir ->type == REDIR_IN)
		{
			status = apply_redir_in(curr_redir);
			if(status == ERROR)
				return status;
		}
		if(curr_redir ->type == HEREDOC)
		{
			dup2(heredoc_pipe_read_fd, STDIN_FILENO);
			close(heredoc_pipe_read_fd);
		}
		curr_redir = curr_redir -> next;
	}
	return (SUCCESS);
}
