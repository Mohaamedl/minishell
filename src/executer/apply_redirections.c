/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:22:44 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:23:06 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	//REDIR_IN,    // <
	//REDIR_OUT,   // >
	//APPEND,      // >>
	//HEREDOC,      // <<


int	apply_heredoc(t_redir *redir)
{
	int pipefd[2];
	char *line;
	int result;

	pipe(pipefd);
	setup_signals_heredoc();
	line = readline("heredoc> ");
	while (line && ft_strcmp(line, redir->file) != 0)
	{
		// Check if Ctrl+C was pressed
		if (g_signal_received == SIGINT)
		{
			free(line);
			close(pipefd[0]);
			close(pipefd[1]);
			write(STDOUT, "\n", 1);
			setup_signals_executing();
			return (ERROR);
		}
		// Aqui você processa a linha do heredoc, por exemplo:
		write(pipefd[1], line, strlen(line)); //escreve para o pipe
		write(pipefd[1], "\n", 1); //readline nao adiciona o \n
		free(line); // libera a linha anterior
		line = readline("heredoc> "); // lê a próxima linha
	}
	// Check if loop ended due to NULL (Ctrl+D or Ctrl+C)
	if (!line)
	{
		// If Ctrl+C was pressed
		if (g_signal_received == SIGINT)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			write(STDOUT, "\n", 1);
			setup_signals_executing();
			return (ERROR);
		}
		// If Ctrl+D (EOF), we should complete the heredoc with what we have
		// This is the standard bash behavior - heredoc completes on EOF
		close(pipefd[1]);
		result = dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		setup_signals_executing();
		if (result == -1)
			return (ERROR);
		return (SUCCESS);
	}
	free(line); // libera a última linha (que é igual ao delimitador)
	close(pipefd[1]);//fecha o lado de escrita do pipe
	result = dup2(pipefd[0], STDIN_FILENO); //prepara o stin para ler do pipe, para um eventual comando como o cat ler do heredoc
	close(pipefd[0]);
	setup_signals_executing();
	if (result == -1)
		return (ERROR);
	return (SUCCESS);
}
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
int	apply_redirections(t_ast *cmd_node)
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
			status = apply_heredoc(curr_redir);
			if(status == ERROR)
				return status;
		}
		curr_redir = curr_redir -> next;
	}
	return (SUCCESS);
}
