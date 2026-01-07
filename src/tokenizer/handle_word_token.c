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
 * @brief Parses and creates a WORD token from the input string
 * @param line Pointer to string starting at a word character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @return 1 on success, 0 on failure
 *
 * Handles variable assignments (VAR=value) by reading the entire assignment
 * including quoted segments in the value part. For regular words, reads until
 * a delimiter (operator, space, or quote) is encountered. Creates a WORD token
 * with the extracted content and advances the index accordingly.
 */
int	handle_word(char *line, int *i, t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int		is_op;
	int		has_equals;
	char	quote_char;

	is_op = 0;
	has_equals = 0;
	j = 0;
	while (line[j] && line[j] != '=' && !is_operator(&line[j])
		&& !is_space(line[j]) && !is_quote(line[j]))
		j++;
	if (line[j] == '=')
	{
		has_equals = 1;
		j++;
	}
	if (has_equals)
	{
		while (line[j] && !is_operator(&line[j]) && !is_space(line[j]))
		{
			if (is_quote(line[j]))
			{
				quote_char = line[j];
				j++;
				while (line[j] && line[j] != quote_char)
					j++;
				if (line[j] != quote_char)
					return (0);
				j++;
			}
			else
			{
				while (line[j] && !is_quote(line[j]) && !is_space(line[j])
					&& !is_operator(&line[j]))
					j++;
			}
		}
	}
	else
	{
		while (!is_operator(&line[j]) && !is_space(line[j])
			&& line[j] != '\0' && !is_quote(line[j]))
			j++;
	}
	str = malloc((j + 1) * sizeof(char));
	ft_memcpy(str, line, j);
	str[j] = '\0';
	token = create_token(str, WORD, 1, is_op);
	append_token(head, last_token, token);
	*i = *i + j;
	return (1);
}
