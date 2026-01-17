/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 12:35:00 by framiran          #+#    #+#             */
/*   Updated: 2026/01/14 12:35:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Increment the SHLVL environment variable
 *
 * Gets the current SHLVL value, increments it by 1, and updates the
 * environment. If SHLVL doesn't exist, it starts from 0.
 *
 * @param env Pointer to the environment list
 */
static void	increment_shlvl(t_env **env)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = get_env_value(*env, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		set_env_value(env, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

/**
 * @brief Initialize the shell structure (non-allocating version)
 *
 * Initializes an already allocated shell structure with environment variables,
 * exit status, running state, is_interactive flag, and signal handlers.
 * Also increments SHLVL.
 *
 * @param shell Pointer to the shell structure to initialize
 * @param envp Environment variables from main
 */
void	init_shell_struct(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->last_exit_status = 0;
	shell->running = 1;
	shell->is_interactive = isatty(STDIN_FILENO);
	if (shell->env_list)
		increment_shlvl(&shell->env_list);
	if (shell->is_interactive)
		setup_signals_interactive();
}

/**
 * @brief Read a line from stdin in non-interactive mode
 *
 * Reads input from stdin one character at a time until newline or EOF.
 * Used when the shell is run in a pipeline or from a script.
 *
 * @return The line read, or NULL on EOF or error
 */
char	*get_next_line_non_interactive(void)
{
	char	*line;
	char	*tmp;
	char	buffer[2];
	ssize_t	bytes_read;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	buffer[1] = '\0';
	bytes_read = read(STDIN_FILENO, buffer, 1);
	while (bytes_read > 0 && buffer[0] != '\n')
	{
		tmp = line;
		line = ft_strjoin(line, buffer);
		free(tmp);
		if (!line)
			return (NULL);
		bytes_read = read(STDIN_FILENO, buffer, 1);
	}
	if (bytes_read <= 0 && ft_strlen(line) == 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * @brief Check if a string contains only whitespace
 *
 * @param str String to check
 * @return 1 if only whitespace/empty, 0 otherwise
 */
int	is_empty_or_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\r')
			return (0);
		str++;
	}
	return (1);
}

/**
 * @brief Get the last node of the AST linked list
 *
 * Traverses the AST linked list through the right pointers to find the last
 * node. This is needed for the build_tree() function.
 *
 * @param head Head of the AST linked list
 * @return Pointer to the last node in the list, or NULL if list is empty
 */
t_ast	*get_last_node(t_ast *head)
{
	t_ast	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->right == NULL)
			return (tmp);
		tmp = tmp->right;
	}
	return (tmp);
}
