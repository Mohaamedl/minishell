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
 * - With argument: use provided path
 * 
 * @param args Command arguments
 * @param env Environment list to get HOME
 * @return Target directory path, or NULL if HOME not found
 */
static char	*get_target_dir(char **args, t_env *env)
{
	char	*home;

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
 * - cd /nonexistent      → Error: No such file or directory
 * - cd /root             → Error: Permission denied
 * 
 * Features:
 * - Supports absolute and relative paths
 * - Updates PWD and OLDPWD after successful change
 * - Proper error messages for different failure cases
 * 
 * @note
 * - Only accepts one argument (the path)
 * - Does NOT support 'cd -' (this is not required by subject)
 * - PWD is updated using getcwd() to get canonical path
 * 
 * @example
 * $ pwd
 * /home/user
 * $ cd /tmp
 * $ pwd
 * /tmp
 * $ echo $OLDPWD
 * /home/user
 */
int	builtin_cd(char **args, t_env **env)
{
	char	*target;
	char	*old_pwd;

	if (args[1] && args[2])
	{
		print_error("cd", NULL, "too many arguments");
		return (ERROR);
	}
	target = get_target_dir(args, *env);
	if (!target)
		return (ERROR);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		print_error("cd", target, strerror(errno));
		free(old_pwd);
		return (ERROR);
	}
	update_pwd_vars(env, old_pwd);
	free(old_pwd);
	return (SUCCESS);
}
