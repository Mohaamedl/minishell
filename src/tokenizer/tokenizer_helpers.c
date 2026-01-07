/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:02 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:30 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the string starts with an operator
 * @param line Pointer to the string to check
 * @return 1 if line starts with an operator, 0 otherwise
 *
 * Recognizes both single-character operators (|, <, >, ;, (, ))
 * and two-character operators (||, &&, >>). Returns 0 for NULL input.
 */
int	is_operator(char *line)
{
	if (!line)
		return (0);
	if (line[0] == '|' && line[1] == '|')
		return (1);
	if (line[0] == '&' && line[1] == '&')
		return (1);
	if (line[0] == '>' && line[1] == '>')
		return (1);
	if (line[0] == ')')
		return (1);
	if (line[0] == '(')
		return (1);
	if (line[0] == '|' || line[0] == '<' || line[0] == '>' || line[0] == ';')
		return (1);
	return (0);
}

/**
 * @brief Checks if a character is a quote
 * @param c Character to check
 * @return 1 if c is a single or double quote, 0 otherwise
 */
int	is_quote(char c)
{
	return (c == '\"' || c == '\'');
}

/**
 * @brief Checks if a character is a single quote
 * @param c Character to check
 * @return 1 if c is a single quote, 0 otherwise
 */
int	is_single_quote(char c)
{
	return (c == '\'');
}

/**
 * @brief Checks if a character is a double quote
 * @param c Character to check
 * @return 1 if c is a double quote, 0 otherwise
 */
int	is_double_quote(char c)
{
	return (c == '\"');
}

/**
 * @brief Checks if a character is a whitespace character
 * @param c Character to check
 * @return 1 if c is space, tab, vertical tab, form feed or carriage return
 */
int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}
