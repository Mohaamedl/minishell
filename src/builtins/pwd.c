/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:33:26 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:36 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

/**
 * @brief Implement pwd built-in command
 * 
 * Prints the current working directory to standard output.
 * Uses getcwd() with NULL buffer to let the system allocate memory.
 * 
 * @return 0 on success, 1 on failure
 * 
 * @details
 * Behavior:
 * - pwd                       → /home/user/project\n
 * - No options are supported
 * - Returns 0 on success
 * - Returns 1 if getcwd() fails
 * 
 * @note
 * - Uses getcwd(NULL, 0) for automatic memory allocation
 * - Memory is freed after printing
 * - Errors are printed to stderr
 * 
 * @example
 * $ pwd
 * /home/user/minishell
 * $ echo $?
 * 0
 */
int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (ERROR);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);
}
