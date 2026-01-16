/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/05 13:57:47 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*head;
	t_ast		*first_node;
	t_ast		*end_node;
	t_ast		*root_node;
	t_shell		shell;
	int			interactive;

	(void)argc;
	(void)argv;

	// Initialize shell
	shell.env_list = init_env(envp);
	shell.last_exit_status = 0;
	shell.running = 1;
	shell.is_interactive = isatty(STDIN_FILENO);

	// Check if stdin is a terminal (interactive mode)
	interactive = shell.is_interactive;

	// Setup signal handlers for interactive mode
	if (interactive)
		setup_signals_interactive();

	while (shell.running)
	{
		if (interactive)
		{
			line = readline("minishell$ ");
			if (g_signal_received == SIGINT)
			{
				shell.last_exit_status = EXIT_SIGINT;
				g_signal_received = SUCCESS;
			}
		}
		else
		{
			// Non-interactive mode: use getline instead of readline
			char	*buffer;
			size_t	bufsize;
			ssize_t	len;

			buffer = NULL;
			bufsize = 0;
			len = getline(&buffer, &bufsize, stdin);
			if (len == -1)
			{
				free(buffer);
				line = NULL;
			}
			else
			{
				// Remove trailing newline
				if (buffer[len - 1] == '\n')
					buffer[len - 1] = '\0';
				line = buffer;
			}
		}
		if (!line)
		{
			// Ctrl+D (EOF) handling
			if (interactive)
			{
				printf("exit\n");
				shell.running = 0;
			}
			break ;
		}

		// Empty line check - skip whitespace-only lines
		if (is_empty_or_whitespace(line))
		{
			free(line);
			continue ;
		}

		// Add to history (only for interactive mode and non-empty lines)
		if (interactive)
			add_history(line);

		// Tokenize input
		head = tokenize(line);
		print_tokens(head);
		if (!head)
		{
			fprintf(stderr, "syntax error: Unclosed quotes\n");
			shell.last_exit_status = SYNTAX_ERROR;
			free(line);
			continue;
		}

		// Validate token list (from KAN-73)
		if (validate_token_list(head) != 1)
		{
			shell.last_exit_status = SYNTAX_ERROR;
			free_tokens(head);
			free(line);
			continue;
		}
		//print_tokens(head);
		// Build and execute AST
		first_node = build_cmds_and_ops_list(head);
		//print_nodes(first_node);
		end_node = get_last_node(first_node);
		root_node = build_tree(first_node, end_node);
		build_sub_trees(&root_node);
		// print_tree(root_node);
		//print_tree_visual(root_node);
		//printf("numb of pipes: %i\n", calc_numb_pipes(root_node));
		// Setup signal handling for command execution
		if (interactive)
			setup_signals_executing();

		// Execute AST
		execute_ast(root_node, &shell);

		// Restore interactive signal handling
		if (interactive)
			setup_signals_interactive();

		// Optional: Print tree for debugging (can be removed/commented)


		// Cleanup
		free_tree(root_node);
		free_tokens(head);
		free(line);
	}

	// Cleanup shell
	free_env(shell.env_list);
	return (shell.last_exit_status);
}
