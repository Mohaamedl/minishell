/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:47:15 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:24 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles quoted text and creates corresponding token
 * @param line Pointer to string starting at a quote character
 * @param i Pointer to the current index in the string
 * @param last_node Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @return 1 on success, 0 on failure
 *
 * Creates a quoted token and advances the index past the entire quoted
 * section including both opening and closing quotes.
 */
int	handle_quote(char *line, int *i, t_token **last_node, t_token **head)
{
	int	status;

	status = create_quoted_token(last_node, head, line, *line);
	*i = *i + get_quoted_size(line, *line) + 2;
	return (status);
}

/**
 * @brief Handles parentheses tokens
 * @param line Pointer to string starting at a parenthesis character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an LPAREN token for '(' or an RPAREN token for ')'.
 * Advances the index by 1.
 */
void	handle_parentesis(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(line) == '(')
	{
		str = ft_strdup("(");
		token = create_token(str, LPAREN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
	if (*(line) == ')')
	{
		str = ft_strdup(")");
		token = create_token(str, RPAREN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles semicolon (;) operator
 * @param line Pointer to string starting at a semicolon character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates a SEMICOLON token. Advances the index by 1.
 * Note: The line parameter is unused in this function.
 */
void	handle_semicolon(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	(void)line;
	is_op = 1;
	str = ft_strdup(";");
	token = create_token(str, SEMICOLON, 1, is_op);
	append_token(head, last_token, token);
	*i = *i + 1;
}
