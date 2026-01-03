/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:32:47 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expand tilde (~) in path to HOME directory
 * 
 * Expands tilde at the beginning of path:
 * - "~" -> HOME
 * - "~/path" -> HOME/path
 * 
 * @param path The path to expand
 * @param env Environment list to get HOME
 * @return Expanded path (must be freed), or NULL if HOME not set
 */
static char	*expand_tilde(const char *path, t_env *env)
{
	char	*home;
	char	*expanded;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value(env, "HOME");
	if (!home)
	{
		print_error("cd", NULL, "HOME not set");
		return (NULL);
	}
	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
		expanded = ft_strjoin(home, path + 1);
	else
		return (ft_strdup(path));
	return (expanded);
}

/**
 * @brief Update PWD and OLDPWD environment variables
 * 
 * After successfully changing directory, this function updates
 * the PWD and OLDPWD environment variables to reflect the change.
 * 
 * @param env Pointer to environment list
 * @param old_pwd The previous working directory
 * @return 0 on success, 1 on failure
 */
static int	update_pwd_vars(t_env **env, const char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd: getcwd");
		return (ERROR);
	}
	if (old_pwd)
		set_env_value(env, "OLDPWD", old_pwd);
	set_env_value(env, "PWD", new_pwd);
	free(new_pwd);
	return (SUCCESS);
}

/**
 * @brief Get target directory for cd command
 * 
 * Determines the target directory based on arguments:
 * - No argument: use HOME directory
 * - "-" argument: use OLDPWD (previous directory)
 * - With argument: use provided path
 * 
 * @param args Command arguments
 * @param env Environment list to get HOME/OLDPWD
 * @return Target directory path, or NULL if HOME/OLDPWD not found
 */
static char	*get_target_dir(char **args, t_env *env)
{
	char	*home;
	char	*oldpwd;

	if (!args[1])
	{
		home = get_env_value(env, "HOME");
		if (!home)
		{
			print_error("cd", NULL, "HOME not set");
			return (NULL);
		}
		return (home);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		oldpwd = get_env_value(env, "OLDPWD");
		if (!oldpwd || !*oldpwd)
		{
			print_error("cd", NULL, "OLDPWD not set");
			return (NULL);
		}
		return (oldpwd);
	}
	return (args[1]);
}

/**
 * @brief Implement cd built-in command
 * 
 * Changes the current working directory to the specified path.
 * Updates PWD and OLDPWD environment variables on success.
 * 
 * @param args NULL-terminated array of arguments (args[0] is "cd")
 * @param env Pointer to environment linked list
 * @return 0 on success, 1 on failure
 * 
 * @details
 * Behavior:
 * - cd /tmp              → Change to /tmp
 * - cd ..                → Go up one directory
 * - cd                   → Go to HOME directory
 * - cd -                 → Change to previous directory (OLDPWD)
 * - cd ~                 → Change to HOME directory
 * - cd ~/Desktop         → Change to HOME/Desktop
 * - cd /nonexistent      → Error: No such file or directory
 * - cd /root             → Error: Permission denied
 * 
 * Features:
 * - Supports absolute and relative paths
 * - Supports tilde expansion (~)
 * - Updates PWD and OLDPWD after successful change
 * - Proper error messages for different failure cases
 * - Prints directory when using 'cd -' (bash behavior)
 * 
 * @note
 * - Only accepts one argument (the path)
 * - PWD is updated using getcwd() to get canonical path
 * - When 'cd -' is used, the new directory is printed to stdout
 * 
 * @example
 * $ pwd
 * /home/user
 * $ cd /tmp
 * $ pwd
 * /tmp
 * $ cd -
 * /home/user
 * $ echo $OLDPWD
 * /tmp
 */
int	builtin_cd(char **args, t_env **env)
{
	char	*target;
	char	*expanded;
	char	*old_pwd;
	int		print_dir;

	if (args[1] && args[2])
	{
		print_error("cd", NULL, "too many arguments");
		return (ERROR);
	}
	target = get_target_dir(args, *env);
	if (!target)
		return (ERROR);
	expanded = expand_tilde(target, *env);
	if (!expanded)
		return (ERROR);
	print_dir = (args[1] && ft_strncmp(args[1], "-", 2) == 0);
	old_pwd = getcwd(NULL, 0);
	if (chdir(expanded) == -1)
	{
		print_error("cd", args[1], strerror(errno));
		free(old_pwd);
		free(expanded);
		return (ERROR);
	}
	if (print_dir)
		ft_putendl_fd(expanded, STDOUT_FILENO);
	update_pwd_vars(env, old_pwd);
	free(old_pwd);
	free(expanded);
	return (SUCCESS);
}
