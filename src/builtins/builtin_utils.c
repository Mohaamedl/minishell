/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:32:38 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

/**
 * @brief Check if a command is a built-in
 * 
 * Compares the command name against all 7 built-in commands.
 * 
 * @param cmd The command name to check
 * @return 1 if built-in, 0 if external command
 * 
 * @example
 * is_builtin("echo")  → 1
 * is_builtin("ls")    → 0
 */
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/**
 * @brief Execute a built-in command
 * 
 * Routes the command to the appropriate built-in handler.
 * Updates the shell's last exit status with the result.
 * 
 * @param args NULL-terminated array of arguments
 * @param shell Pointer to shell state
 * @return Exit status of the built-in command
 */
int	execute_builtin(char **args, t_shell *shell)
{
	int	status;

	if (!args || !args[0])
		return (ERROR);
	if (ft_strcmp(args[0], "echo") == 0)
		status = builtin_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		status = builtin_cd(args, &shell->env_list);
	else if (ft_strcmp(args[0], "pwd") == 0)
		status = builtin_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		status = builtin_export(args, &shell->env_list);
	else if (ft_strcmp(args[0], "unset") == 0)
		status = builtin_unset(args, &shell->env_list);
	else if (ft_strcmp(args[0], "env") == 0)
		status = builtin_env(shell->env_list);
	else if (ft_strcmp(args[0], "exit") == 0)
		status = builtin_exit(args, shell->last_exit_status, shell);
	else
		return (ERROR);
	shell->last_exit_status = status;
	return (status);
}

/**
 * @brief Validate if a string is a valid identifier
 * 
 * Checks if a string is a valid variable name for export/unset.
 * Valid identifiers must:
 * - Start with a letter or underscore
 * - Contain only letters, digits, and underscores
 * 
 * @param str The string to validate
 * @return 1 if valid, 0 if invalid
 * 
 * @example
 * is_valid_identifier("VAR")      → 1
 * is_valid_identifier("_VAR")     → 1
 * is_valid_identifier("VAR123")   → 1
 * is_valid_identifier("123VAR")   → 0
 * is_valid_identifier("VA-R")     → 0
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Count the number of arguments in an array
 * 
 * Counts elements in a NULL-terminated array.
 * 
 * @param args NULL-terminated array of strings
 * @return Number of arguments (including args[0])
 * 
 * @example
 * count_args({"echo", "hello", NULL})  → 2
 * count_args({"cd", NULL})             → 1
 */
int	count_args(char **args)
{
	int	count;

	if (!args)
		return (0);
	count = 0;
	while (args[count])
		count++;
	return (count);
}
