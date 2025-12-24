/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:21:56 by framiran          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:46 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <minishell.h>

int cmd_name_is_redir(char *cmd_name)
{
	if (ft_strcmp(cmd_name, ">>") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "<<") == 0)
		return 1;
	if (ft_strcmp(cmd_name, ">") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "<") == 0)
		return 1;
	return 0;
}


/**
 * @brief Save the current standard input and output file descriptors.
 *
 * Duplicates the standard input (STDIN_FILENO) and output (STDOUT_FILENO)
 * file descriptors and stores them in the provided array for later restoration.
 *
 * @param saved_std_fds Array to store the saved stdin and stdout.
 */
void	save_std_fds(int saved_std_fds[2])
{
	saved_std_fds[0] = dup(STDIN_FILENO);
	saved_std_fds[1] = dup(STDOUT_FILENO);
}

/**
 * @brief Restore the standard input and output file descriptors from saved values.
 *
 * Restores the standard input and output streams using the saved file descriptors
 * and closes the saved descriptors to release resources.
 *
 * @param saved_stdin The saved standard input file descriptor.
 * @param saved_stdout The saved standard output file descriptor.
 */
void restore_std_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

/**
 * @brief Convert argument linked list to NULL-terminated array
 *
 * Converts the t_arg linked list to an array format needed for execution.
 * First element is the command name, followed by arguments.
 *
 * @param cmd The command structure containing name and args
 * @return NULL-terminated array of strings, NULL on error
 */
static char	**convert_args_to_array(t_cmd *cmd)
{
	char	**args;
	t_arg	*current;
	int		count;
	int		i;

	if (!cmd || !cmd->cmd_name)
		return (NULL);
	// Count arguments (cmd->args includes cmd_name as first element)
	count = 0;
	current = cmd->args;
	while (current)
	{
		count++;
		current = current->next;
	}
	// Allocate array
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	// Fill array directly from args list (which includes cmd_name)
	i = 0;
	current = cmd->args;
	while (current)
	{
		args[i++] = current->value;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

/**
 * @brief Prepare the command arguments for execution.
 *
 * Expands the arguments and redirection files for the command. This includes
 * handling any environment variable expansion or other necessary processing.
 * It returns the arguments in the form of a `char**` array ready for execution.

 * @return A `char**` array of command arguments, or `NULL` if the command is invalid.
 */
char **prepare_cmd_for_execution(t_cmd *cmd, t_shell *shell)
{
	char **args;
	if (!cmd || !cmd->cmd_name)
		return NULL;
	expand_cmd_args(cmd->args, shell);
	expand_redirection_files(cmd->redirs, shell); //need to che how the heredoc expansion is being done
	args = convert_args_to_array(cmd);
	return args;
}
