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


static void	handle_simple_word(char *line, int *j)
{
	while (line[*j] && !is_operator(&line[*j])
		&& !is_space(line[*j])
		&& !is_quote(line[*j]))
		(*j)++;
}


int	handle_word(char *line, int *i, t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int		has_equals;

	has_equals = check_has_equals(line, &j);//has equals ja acanca o j para = ou ouperador ou espacofim de linha
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

