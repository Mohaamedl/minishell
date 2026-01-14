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

/**
 * @brief Handle readline input for interactive mode
 *
 * Reads a line from stdin using readline and updates exit status if
 * SIGINT was received.
 *
 * @param shell Pointer to the shell structure
 * @return Allocated string with the input line, or NULL on EOF
 */
static char	*handle_interactive_input(t_shell *shell)
{
	char	*line;

	line = readline("minishell$ ");
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_status = EXIT_SIGINT;
		g_signal_received = SUCCESS;
	}
	return (line);
}

/**
 * @brief Handle getline input for non-interactive mode
 *
 * Reads a line from stdin using getline and removes the trailing newline.
 *
 * @return Allocated string with the input line, or NULL on EOF/error
 */
static char	*handle_noninteractive_input(void)
{
	char	*buffer;
	size_t	bufsize;
	ssize_t	len;

	buffer = NULL;
	bufsize = 0;
	len = getline(&buffer, &bufsize, stdin);
	if (len == -1)
	{
		free(buffer);
		return (NULL);
	}
	if (buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';
	return (buffer);
}

/**
 * @brief Handle EOF condition
 *
 * Prints "exit" message for interactive mode and sets shell running state
 * to false.
 *
 * @param shell Pointer to the shell structure
 * @param interactive Flag indicating if in interactive mode
 */
static void	handle_eof(t_shell *shell, int interactive)
{
	if (interactive)
	{
		printf("exit\n");
		shell->running = 0;
	}
}

/**
 * @brief Process and validate tokenized input
 *
 * Tokenizes the input line and validates the token list. Updates the exit
 * status on errors.
 *
 * @param line Input line to tokenize
 * @param shell Pointer to the shell structure
 * @return Pointer to the token list, or NULL on error
 */
static t_token	*process_tokens(char *line, t_shell *shell)
{
	t_token	*head;

	head = tokenize(line);
	if (!head)
	{
		fprintf(stderr, "syntax error: Unclosed quotes\n");
		shell->last_exit_status = SYNTAX_ERROR;
		return (NULL);
	}
	if (validate_token_list(head) != 1)
	{
		shell->last_exit_status = SYNTAX_ERROR;
		free_tokens(head);
		return (NULL);
	}
	return (head);
}

/**
 * @brief Build and execute the Abstract Syntax Tree
 *
 * Builds the AST from the token list, executes it, and performs cleanup.
 * Handles signal setup for command execution in interactive mode.
 *
 * @param head Head of the token list
 * @param shell Pointer to the shell structure
 * @param interactive Flag indicating if in interactive mode
 */
static void	execute_tokens(t_token *head, t_shell *shell, int interactive)
{
	t_ast	*first_node;
	t_ast	*end_node;
	t_ast	*root_node;

	first_node = build_cmds_and_ops_list(head);
	end_node = get_last_node(first_node);
	root_node = build_tree(first_node, end_node);
	build_sub_trees(&root_node);
	if (interactive)
		setup_signals_executing();
	execute_ast(root_node, shell);
	if (interactive)
		setup_signals_interactive();
	free_tree(root_node);
}

/**
 * @brief Process a single command line
 *
 * Handles input reading, tokenization, AST building, and execution for a
 * single command line. Manages memory cleanup at each step.
 *
 * @param shell Pointer to the shell structure
 * @param interactive Flag indicating if in interactive mode
 * @return 1 to continue the loop, 0 to break
 */
static int	process_line(t_shell *shell, int interactive)
{
	char	*line;
	t_token	*head;

	if (interactive)
		line = handle_interactive_input(shell);
	else
		line = handle_noninteractive_input();
	if (!line)
	{
		handle_eof(shell, interactive);
		return (0);
	}
	if (is_empty_or_whitespace(line))
	{
		free(line);
		return (1);
	}
	if (interactive)
		add_history(line);
	head = process_tokens(line, shell);
	if (!head)
	{
		free(line);
		return (1);
	}
	execute_tokens(head, shell, interactive);
	free_tokens(head);
	free(line);
	return (1);
}

/**
 * @brief Main entry point for the minishell program
 *
 * Initializes the shell environment, sets up signal handlers, and enters
 * the main read-eval-print loop. Handles both interactive and non-interactive
 * modes. Cleans up resources before exiting.
 *
 * @param argc Argument count (unused)
 * @param argv Argument vector (unused)
 * @param envp Environment variables
 * @return Exit status of the last command
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		interactive;

	(void)argc;
	(void)argv;
	shell.env_list = init_env(envp);
	shell.last_exit_status = 0;
	shell.running = 1;
	shell.is_interactive = isatty(STDIN_FILENO);
	interactive = shell.is_interactive;
	if (interactive)
		setup_signals_interactive();
	while (shell.running)
	{
		if (!process_line(&shell, interactive))
			break ;
	}
	free_env(shell.env_list);
	return (shell.last_exit_status);
}
