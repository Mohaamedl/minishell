/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:32:54 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/26 13:00:26 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if argument is a valid -n option
 *
 * The echo command accepts multiple -n options. This function validates
 * that the argument starts with '-n' and contains only 'n' characters.
 *
 * @param arg The argument to check
 * @return true if valid -n option, false otherwise
 *
 * @example
 * is_n_option("-n")     → true
 * is_n_option("-nnn")   → true
 * is_n_option("-na")    → false
 */
static bool	is_n_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Implement echo built-in command
 *
 * Prints arguments to standard output separated by spaces.
 * Supports the -n option which suppresses the trailing newline.
 * Multiple -n options are processed (matches bash behavior).
 *
 * @param args NULL-terminated array of arguments (args[0] is "echo")
 * @return Always returns 0 (echo never fails)
 *
 * @details
 * Behavior matches bash echo:
 * - echo hello world          → "hello world\n"
 * - echo -n test              → "test"
 * - echo -n -n test           → "test"
 * - echo -nnn test            → "test"
 * - echo ""                   → "\n"
 *
 * @note
 * - args[0] is the command name "echo"
 * - Processes all leading -n options
 * - Prints space between arguments
 * - Returns 0 even with no arguments
 */
int	builtin_echo(char **args)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		//printf("%s", args[i]);
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
		//	printf(" ");
			write(STDOUT_FILENO," ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO,"\n", 1);;
	return (SUCCESS);
}
