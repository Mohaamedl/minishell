/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:15 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:26 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the line contains an equals sign for variable assignment
 * @param line Pointer to the string to check
 * @param j Pointer to index that will be updated to position after '=' or end
 * @return 1 if equals sign found, 0 otherwise
 *
 * Scans the line until it finds '=', an operator, a space, a quote, or end.
 * Updates j to point just after the '=' if found.
 */
static int	check_has_equals(char *line, int *j)
{
	*j = 0;
	while (line[*j] && line[*j] != '='
		&& !is_operator(&line[*j])
		&& !is_space(line[*j])
		&& !is_quote(line[*j]))
		(*j)++;
	if (line[*j] == '=')
	{
		(*j)++;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles variable assignment token parsing
 * @param line Pointer to the string to parse
 * @param j Pointer to the current index in the string (positioned after '=')
 * @return 1 if successfully parsed, 0 if unclosed quote found
 *
 * Processes the value part of a variable assignment, handling both quoted
 * and unquoted strings. Advances j to the end of the assignment value.
 */
static int	handle_var_assignment(char *line, int *j)
{
	char	quote_char;

	while (line[*j] && !is_operator(&line[*j]) && !is_space(line[*j]))
	{
		if (is_quote(line[*j]))
		{
			quote_char = line[*j];
			(*j)++;
			while (line[*j] && line[*j] != quote_char)
				(*j)++;
			if (line[*j] != quote_char)
				return (0);
			(*j)++;
		}
		else
		{
			while (line[*j] && !is_quote(line[*j])
				&& !is_space(line[*j])
				&& !is_operator(&line[*j]))
				(*j)++;
		}
	}
	return (1);
}

/**
 * @brief Handles simple word token parsing without variable assignment
 * @param line Pointer to the string to parse
 * @param j Pointer to the current index in the string
 *
 * Advances j through the string until it encounters an operator, space,
 * quote, or end of string.
 */
static void	handle_simple_word(char *line, int *j)
{
	while (line[*j] && !is_operator(&line[*j])
		&& !is_space(line[*j])
		&& !is_quote(line[*j]))
		(*j)++;
}

/**
 * @brief Creates and appends a word token to the token list
 * @param line Pointer to the string containing the word to tokenize
 * @param i Pointer to the current position in the original input
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @return 1 if successful, 0 if variable assignment has unclosed quotes
 *
 * Determines if the word is a variable assignment (contains '=') or a simple
 * word. Parses accordingly, creates a WORD token with the parsed string,
 * and appends it to the token list. Updates i to advance past the processed
 * word.
 */
int	handle_word(char *line, int *i, t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int		has_equals;

	has_equals = check_has_equals(line, &j);
	if (has_equals && !handle_var_assignment(line, &j))
		return (0);
	if (!has_equals)
		handle_simple_word(line, &j);
	str = malloc((j + 1) * sizeof(char));
	ft_memcpy(str, line, j);
	str[j] = '\0';
	token = create_token(str, WORD, 1, 0);
	append_token(head, last_token, token);
	*i += j;
	return (1);
}

