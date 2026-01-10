/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:53:32 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/13 12:54:13 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Remove quotes from a string
 * 
 * Removes only the outermost layer of quotes, preserving nested quotes.
 * Example: '"hello"' → "hello", "'world'" → world
 * 
 * @param str String potentially containing quotes
 * @return Newly allocated string without outer quotes
 */
static char	*remove_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	char	in_quote;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	while (str[i])
	{
		if (!in_quote && (str[i] == '"' || str[i] == '\''))
		{
			in_quote = str[i];
			i++;
			continue;
		}
		if (in_quote && str[i] == in_quote)
		{
			in_quote = 0;
			i++;
			continue;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Extract variable name from string starting with $
 * 
 * Extracts the variable name after $ until finding a non-alphanumeric char.
 * Handles special variables: $? (exit status), $$ (PID), $@ (args).
 * Special variables $$ and $@ are treated as expandable to empty string.
 * Examples: "$USER" → "USER", "$PATH_TO" → "PATH_TO", "$?" → "?"
 * 
 * @param str String starting with $ followed by variable name
 * @return Newly allocated string with variable name, NULL on error
 */
static char	*extract_var_name(const char *str)
{
	size_t	len;
	char	*var_name;

	if (!str || *str != '$')
		return (NULL);
	str++;
	if (*str == '?')
		return (ft_strdup("?"));
	if (*str == '$' || *str == '@')
		return (ft_strdup(str[0] == '$' ? "$" : "@"));
	len = 0;
	if (str[len] && (ft_isalpha(str[len]) || str[len] == '_'))
	{
		len++;
		while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
			len++;
	}
	if (len == 0)
		return (NULL);
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_memcpy(var_name, str, len);
	var_name[len] = '\0';
	return (var_name);
}

/**
 * @brief Get the value to replace a variable expansion
 * 
 * Returns the value of the variable or "$?" special case.
 * For "$?", returns the last exit status as a string.
 * Special variables $$ and $@ expand to empty string (not implemented).
 * 
 * @param var_name Variable name (without $)
 * @param shell Shell state containing env and exit status
 * @return Variable value or empty string if not found (not NULL)
 */
static char	*get_var_value(const char *var_name, t_shell *shell)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->last_exit_status));
	if (ft_strcmp(var_name, "$") == 0 || ft_strcmp(var_name, "@") == 0)
		return (ft_strdup(""));
	value = get_env_value(shell->env_list, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

/**
 * @brief Expand a single $VAR in a string
 * 
 * Builds a new string replacing first occurrence of $VAR with its value.
 * If $ is not followed by a valid variable name, keeps the $ as-is.
 * 
 * @param str String containing $VAR
 * @param shell Shell state
 * @param skip_invalid Set to 1 if $ was invalid (not followed by var name)
 * @return New string with expansion
 */
static char	*expand_one_var(char *str, t_shell *shell, int *skip_invalid)
{
	char	*dollar_pos;
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*before;
	char	*after;
	size_t	var_len;

	dollar_pos = ft_strchr(str, '$');
	if (!dollar_pos)
		return (ft_strdup(str));
	var_name = extract_var_name(dollar_pos);
	if (!var_name)
	{
		*skip_invalid = 1;
		return (ft_strdup(str));
	}
	var_value = get_var_value(var_name, shell);
	var_len = ft_strlen(var_name) + 1;
	if (ft_strcmp(var_name, "?") == 0)
		var_len = 2;
	before = ft_substr(str, 0, dollar_pos - str);
	after = ft_strdup(dollar_pos + var_len);
	result = ft_strjoin(before, var_value);
	free(before);
	before = result;
	result = ft_strjoin(before, after);
	free(before);
	free(after);
	free(var_name);
	free(var_value);
	*skip_invalid = 0;
	return (result);
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
			break;
	}
	return (current);
}

/**
 * @brief Expand variables in command arguments
 * 
 * Iterates through all arguments and expands those marked as expandable.
 * Also removes quotes from all arguments.
 * Replaces argument values with expanded versions and marks them as expanded.
 * First removes quotes from original token, then expands variables if
 * the argument is expandable.
 * 
 * @param args Linked list of arguments
 * @param shell Shell state
 */
void	expand_cmd_args(t_arg *args, t_shell *shell)
{
	t_arg	*current;
	char	*without_quotes;
	char	*expanded;

	current = args;
	while (current)
	{
		if (current->value)
		{
			without_quotes = remove_quotes(current->value);
			if (current->is_expandable && without_quotes)
			{
				expanded = expand_variables(without_quotes, shell);
				free(without_quotes);
				if (expanded)
				{
					current->value = expanded;
					current->was_expanded = 1;
				}
			}
			else if (without_quotes)
			{
				current->value = without_quotes;
				current->was_expanded = 1;
			}
		}
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
