/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 12:12:01 by francisco         #+#    #+#             */
/*   Updated: 2026/01/12 12:19:35 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles opening and closing quotes while parsing a string.
 *
 * This function updates the quote state and advances the index whenever
 * an opening or closing quote is found. Quote characters are skipped
 * and are not copied to the output string.
 *
 * @param str       The input string being parsed.
 * @param i         Pointer to the current index in the string.
 * @param in_quote  Pointer to the current quote state.
 *                  0 if not inside quotes, otherwise contains
 *                  the quote character (' or ").
 */
static void	handle_quotes(const char *str, int *i, char *in_quote)
{
	if (!*in_quote && (str[*i] == '"' || str[*i] == '\''))
		*in_quote = str[(*i)++];
	else if (*in_quote && str[*i] == *in_quote)
	{
		*in_quote = 0;
		(*i)++;
	}
}

/**
 * @brief Removes only the outer single or double quotes from a string.
 *
 * This function removes matching outer quotes while preserving the
 * content inside them. Inner characters, including quotes that are
 * part of the content, are kept.
 *
 * Example:
 *   "'hello'"  -> "hello"
 *   "\"hi\""   -> "hi"
 *
 * @param str  The input string to process.
 *
 * @return A newly allocated string without the outer quotes,
 *         or NULL if allocation fails or if str is NULL.
 *
 * @note The caller is responsible for freeing the returned string.
 */
char	*remove_quotes(const char *str)
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
		handle_quotes(str, &i, &in_quote);
		if (str[i])
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Duplicates a special shell variable name.
 *
 * Handles special shell variables that consist of a single character
 * after '$', including:
 * - '$'  → PID (symbolic)
 * - '@'  → positional arguments (symbolic)
 * - '?'  → exit status
 *
 * @param c  The special variable character ('$','@','?')
 * @return A newly allocated string containing the variable name,
 *         or NULL if the character is not a recognized special variable
 *         or if allocation fails.
 *
 * @note The caller is responsible for freeing the returned string.
 */
static char	*dup_special_var_name(char c)
{
	if (c == '$')
		return (ft_strdup("$"));
	if (c == '@')
		return (ft_strdup("@"));
	if (c == '?')
		return (ft_strdup("?"));
	return (NULL);
}

/**
 * @brief Extracts a variable name from a string starting with $.
 *
 * Parses the input string starting with '$' and returns a newly allocated
 * string containing the variable name. Handles:
 * - Special variables: $? (exit status), $$ (PID), $@ (arguments)
 * - Regular variables starting with a letter or '_' and followed by
 *   letters, numbers, or underscores.
 *
 * Examples:
 *   "$USER"    → "USER"
 *   "$PATH_TO" → "PATH_TO"
 *   "$?"       → "?"
 *   "$$"       → "$"
 *   "$@"       → "@"
 *
 * @param str  String starting with '$' followed by the variable name.
 * @return A newly allocated string containing the variable name,
 *         or NULL if str is NULL, does not start with '$',
 *         or if allocation fails.
 *
 * @note The caller is responsible for freeing the returned string.
 */
char	*extract_var_name(const char *str)
{
	size_t	len;
	char	*var_name;

	if (!str || *str != '$')
		return (NULL);
	str++;
	if (*str == '$' || *str == '?' || *str == '@')
		return (dup_special_var_name(*str));
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
char	*get_var_value(const char *var_name, t_shell *shell)
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
