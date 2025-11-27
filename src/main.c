/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/27 15:34:57 by framiran         ###   ########.fr       */
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

	while (shell.running)
	{
		if (interactive)
		{
			line = readline("minishell$ ");
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
			if (interactive)
				printf("exit\n");  // Ctrl+D pressed
			break ;
		}

		// Empty line check
		if (!*line)
		{
			free(line);
			continue ;
		}

		if (interactive)
			add_history(line);

		// Tokenize input
		head = tokenize(line);
		if (!head)
		{
			fprintf(stderr, "Error: Unclosed quotes\n");
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
		print_tokens(head);
		// Build and execute AST
		first_node = build_cmds_and_ops_list(head);
		print_nodes(first_node);
		end_node = get_last_node(first_node);
		root_node = build_tree(first_node, end_node);
		build_sub_trees(&root_node);
		// print_tree(root_node);
		print_tree_visual(root_node);
		// Execute AST
		execute_ast(root_node, &shell);

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
