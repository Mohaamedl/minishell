/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/14 13:15:39 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Simple command parser for testing built-ins (Phase 3)
 *
 * This is a TEMPORARY parser used only for testing built-in commands
 * before the full lexer/parser from Phase 2 is integrated.
 *
 * Splits input line by spaces into argc/argv format.
 * Does NOT handle quotes, escapes, redirections, or pipes.
 *
 * @param line Raw input line from readline
 * @return NULL-terminated array of arguments, or NULL on error
 *
 * @note
 * - This will be REPLACED by the real parser in Phase 2
 * - Only for built-in testing purposes
 * - Memory must be freed with free_array()
 */
char	**simple_parse(char *line)
{
	char	**args;

	if (!line || !*line)
		return (NULL);
	args = ft_split(line, ' ');
	return (args);
}

/**
 * @brief Execute a parsed command
 *
 * Checks if command is a built-in and executes it.
 * For Phase 3 testing, external commands are not supported.
 *
 * @param args NULL-terminated array of arguments
 * @param shell Shell state structure
 * @return Exit status of the command
 */
int	execute_command(char **args, t_shell *shell)
{
	int	status;

	if (!args || !args[0])
		return (SUCCESS);
	if (is_builtin(args[0]))
	{
		status = execute_builtin(args, shell);
		return (status);
	}
	print_error(args[0], NULL,
		"command not found (external commands not yet implemented)");
	return (CMD_NOT_FOUND);
}
