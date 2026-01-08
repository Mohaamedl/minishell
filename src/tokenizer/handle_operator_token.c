/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operator_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:41:48 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:20 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles pipe (|) or OR (||) operators
 * @param line Pointer to string starting at a pipe character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an OR token if two consecutive pipes are found,
 * or a PIPE token for a single pipe. Advances the index by 2 for OR
 * or 1 for PIPE.
 */
void	handle_pipe_or_or(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(++line) == '|')
	{
		str = ft_strdup("||");
		token = create_token(str, OR, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("|");
		token = create_token(str, PIPE, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles AND (&&) operator
 * @param line Pointer to string starting at an ampersand character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates an AND token when two consecutive ampersands are found.
 * Advances the index by 2.
 */
void	handle_and(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(++line) == '&')
	{
		str = ft_strdup("&&");
		token = create_token(str, AND, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
}
