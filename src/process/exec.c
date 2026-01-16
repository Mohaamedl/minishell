/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:23:29 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:23:33 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free split PATH directories array
 *
 * @param dirs Array of directory strings
 */
static void	free_split_dirs(char **dirs)
{
	int	i;

	if (!dirs)
		return ;
	i = 0;
	while (dirs[i])
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
}

/**
 * @brief Construct full path and check if executable
 *
 * @param dir Directory path
 * @param cmd Command name
 * @return char* Full path if executable, NULL otherwise (must free)
 */
static char	*try_directory(char *dir, char *cmd)
{
	char	*full_path;
	char	*temp;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

/**
 * @brief Search for command in PATH directories
 *
 * Searches through all directories in PATH environment variable
 * to find an executable command.
 *
 * @param cmd Command name
 * @param env Environment list
 * @return char* Full path to command (caller must free), NULL if not found
 */
static char	*search_in_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**directories;
	char	*result;
	int		i;

	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	directories = ft_split(path_env, ':');
	if (!directories)
		return (NULL);
	i = 0;
	result = NULL;
	while (directories[i] && !result)
	{
		result = try_directory(directories[i], cmd);
		i++;
	}
	free_split_dirs(directories);
	return (result);
}

/**
 * @brief Find command path (absolute, relative, or in PATH)
 *
 * Algorithm:
 * 1. If cmd contains '/': use as-is (absolute or relative path)
 * 2. Otherwise: search in PATH directories
 *
 * @param cmd Command name
 * @param env Environment list
 * @return char* Full path to command (caller must free), NULL if not found
 */
char	*find_command_in_path(char *cmd, t_env *env)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (search_in_path(cmd, env));
}

/**
 * @brief Execute external command using execve
 *
 * This function:
 * 1. Finds command in PATH
 * 2. Converts environment to array
 * 3. Executes command with execve
 * 4. Handles errors (command not found, permission denied)
 *
 * Note: This function only returns on error (execve replaces process)
 *
 * @param args Command arguments (args[0] is command name)
 * @param shell Shell state structure
 * @return int Exit status (only on error: 127 or 126)
 */
int	execute_external_cmd(char **args, t_shell *shell)
{
	char	*cmd_path;
	char	**env_array;

	if (!args || !args[0])
		return (CMD_NOT_FOUND);
	cmd_path = find_command_in_path(args[0], shell->env_list);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (CMD_NOT_FOUND);
	}
	env_array = env_list_to_array(shell->env_list);
	if (!env_array)
	{
		free(cmd_path);
		return (ERROR);
	}
	execve(cmd_path, args, env_array);
	perror("minishell: execve");
	free(cmd_path);
	free_split_dirs(env_array);
	return (CMD_NOT_EXECUTABLE);
}
