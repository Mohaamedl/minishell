/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:53:32 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/12 12:21:11 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expand a single $VAR in a string
 *
 * Replaces the first occurrence of a variable expansion ($VAR or $?)
 * with its corresponding value.
 * If '$' is not followed by a valid variable name, the string is
 * returned unchanged and skip_invalid is set.
 *
 * @param str String containing a variable to expand
 * @param shell Shell state
 * @param skip_invalid Set to 1 if the '$' is not a valid expansion
 * @return Newly allocated string with the expansion applied
 */
static char	*expand_one_var(char *str, t_shell *shell, int *skip_invalid)
{
	char	*dollar;
	char	*var_name;
	char	*value;

	dollar = ft_strchr(str, '$');
	if (!dollar)
		return (ft_strdup(str));
	var_name = extract_var_name(dollar);
	if (!var_name)
	{
		*skip_invalid = 1;
		return (ft_strdup(str));
	}
	value = get_var_value(var_name, shell);
	*skip_invalid = 0;
	str = build_expanded(str, value, dollar, get_var_len(var_name));
	free(var_name);
	free(value);
	return (str);
}

/**
 * @brief Expand all variables in a string
 * 
 * Repeatedly expands all $VAR and $? occurrences in the string.
 * Stops when no more valid $ expansions are found.
 * 
 * @param str String potentially containing $VAR
 * @param shell Shell state
 * @return New string with all expansions done
 */
char	*expand_variables(char *str, t_shell *shell)
{
	char	*current;
	char	*next;
	int		skip_invalid;

	if (!str || !shell)
		return (NULL);
	current = ft_strdup(str);
	while (ft_strchr(current, '$'))
	{
		skip_invalid = 0;
		next = expand_one_var(current, shell, &skip_invalid);
		free(current);
		current = next;
		if (!current)
			return (ft_strdup(str));
		if (skip_invalid)
			break ;
	}
	return (current);
}

/**
 * @brief Expand a single command argument
 *
 * Removes quotes from the argument value and expands any variables
 * if the argument is marked as expandable.
 *
 * Updates the argument's value in-place and sets the was_expanded flag.
 *
 * @param arg Pointer to the argument to expand
 * @param shell Shell state used for variable expansion
 *
 * @note Frees intermediate strings used during quote removal and expansion.
 */
static void	expand_single_arg(t_arg *arg, t_shell *shell)
{
	char	*without_quotes;
	char	*expanded;

	if (!arg->value)
		return ;
	without_quotes = remove_quotes(arg->value);
	if (arg->is_expandable && without_quotes)
	{
		expanded = expand_variables(without_quotes, shell);
		free(without_quotes);
		if (expanded)
		{
			arg->value = expanded;
			arg->was_expanded = 1;
		}
	}
	else if (without_quotes)
	{
		arg->value = without_quotes;
		arg->was_expanded = 1;
	}
}

/**
 * @brief Expand variables in all command arguments
 *
 * Iterates through a linked list of arguments, removing quotes
 * and expanding variables for each argument that is expandable.
 *
 * @param args Head of the linked list of command arguments
 * @param shell Shell state used for variable expansion
 *
 * @note Each argument's value may be replaced with a newly allocated string.
 */
void	expand_cmd_args(t_arg *args, t_shell *shell)
{
	t_arg	*current;

	current = args;
	while (current)
	{
		expand_single_arg(current, shell);
		current = current->next;
	}
}

/**
 * @brief Expand variables in redirection file names
 * 
 * Iterates through all redirections and expands file names marked as expandable.
 * Replaces file name values with expanded versions and marks them as expanded.
 * For HEREDOC, file_name_is_expandable=1 means content variables expand, but
 * the filename (delimiter) never expands.
 * 
 * @param redirs Linked list of redirections
 * @param shell Shell state
 */
void	expand_redirection_files(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;
	char	*expanded;

	current = redirs;
	while (current)
	{
		if (current->file_name_is_expandable && current->file
			&& current->type != HEREDOC)
		{
			expanded = expand_variables(current->file, shell);
			if (expanded)
			{
				current->file = expanded;
				current->file_was_expanded = 1;
			}
		}
		current = current->next;
	}
}
