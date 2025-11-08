/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:33:13 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/**
 * @brief Check if a string represents a numeric value
 * 
 * Validates that a string contains only digits, optionally preceded
 * by a plus or minus sign.
 * 
 * @param str The string to check
 * @return true if numeric, false otherwise
 * 
 * @example
 * is_numeric("123")   → true
 * is_numeric("-42")   → true
 * is_numeric("+100")  → true
 * is_numeric("12a")   → false
 * is_numeric("abc")   → false
 */
static bool	is_numeric(const char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Perform cleanup and exit the shell
 * 
 * Frees all allocated memory and exits with the specified status.
 * 
 * @param shell Shell state structure
 * @param status Exit status code
 * @noreturn This function does not return
 */
static void	cleanup_and_exit(t_shell *shell, int status)
{
	if (shell)
	{
		if (shell->env_list)
			free_env(shell->env_list);
	}
	rl_clear_history();
	exit(status);
}

/**
 * @brief Implement exit built-in command
 * 
 * Exits the minishell with the specified exit code.
 * Handles various argument scenarios and validates numeric inputs.
 * 
 * @param args NULL-terminated array of arguments (args[0] is "exit")
 * @param last_exit_status The last command's exit status
 * @param shell Pointer to shell state (for cleanup)
 * @return Does not return on success; returns 1 if too many arguments
 * 
 * @details
 * Behavior:
 * - exit                 → Exit with last_exit_status
 * - exit 0               → Exit with status 0
 * - exit 42              → Exit with status 42
 * - exit 300             → Exit with status 44 (300 % 256)
 * - exit -1              → Exit with status 255 (-1 % 256)
 * - exit abc             → Error: numeric argument required, exit 2
 * - exit 1 2             → Error: too many arguments (DON'T exit)
 * 
 * Exit status rules:
 * - No argument: use last_exit_status
 * - Numeric argument: exit_status = arg % 256
 * - Non-numeric: print error and exit with 2
 * - Too many args: print error and return 1 (don't exit)
 * 
 * @note
 * - Always prints "exit" before exiting
 * - Exit status is taken modulo 256 to fit in unsigned char
 * - Negative numbers work: -1 becomes 255
 * - Only exits if 0 or 1 arguments; returns ERROR if 2+ arguments
 * 
 * @example
 * $ false
 * $ exit
 * exit
 * [Shell exits with status 1]
 * 
 * $ exit 42
 * exit
 * [Shell exits with status 42]
 * 
 * $ exit abc
 * exit
 * minishell: exit: abc: numeric argument required
 * [Shell exits with status 2]
 * 
 * $ exit 1 2
 * exit
 * minishell: exit: too many arguments
 * [Shell continues running, returns 1]
 */
int	builtin_exit(char **args, int last_exit_status, t_shell *shell)
{
	int	exit_status;
	int	arg_count;

	printf("exit\n");
	arg_count = count_args(args);
	if (arg_count == 1)
		cleanup_and_exit(shell, last_exit_status);
	if (!is_numeric(args[1]))
	{
		print_error("exit", args[1], "numeric argument required");
		cleanup_and_exit(shell, SYNTAX_ERROR);
	}
	if (arg_count > 2)
	{
		print_error("exit", NULL, "too many arguments");
		return (ERROR);
	}
	exit_status = ft_atoi(args[1]);
	exit_status = (unsigned char)exit_status;
	cleanup_and_exit(shell, exit_status);
	return (SUCCESS);
}
