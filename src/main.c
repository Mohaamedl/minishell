/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:31:30 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*line;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
	{
		print_error("minishell", NULL, "initialization failed");
		return (ERROR);
	}
	while (shell->running)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			char **args;
			
			add_history(line);
			args = simple_parse(line);
			if (args)
			{
				execute_command(args, shell);
				free_array(args);
			}
		}
		free(line);
	}
	cleanup_shell(shell);
	return (SUCCESS);
}
