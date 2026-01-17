/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection_token.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:35:06 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:21 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles output redirection (>) or append (>>) operators
 * @param line Pointer to string starting at a greater-than character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an APPEND token if two consecutive greater-than signs
 * are found, or a REDIR_OUT token for a single greater-than sign.
 * Advances the index by 2 for APPEND or 1 for REDIR_OUT.
 */
void	handle_redap_or_redout(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 0;
	if (*(++line) == '>')
	{
		str = ft_strdup(">>");
		token = create_token(str, APPEND, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup(">");
		token = create_token(str, REDIR_OUT, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles input redirection (<) or heredoc (<<) operators
 * @param line Pointer to string starting at a less-than character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either a HEREDOC token if two consecutive less-than signs are found,
 * or a REDIR_IN token for a single less-than sign. Advances the index
 * by 2 for HEREDOC or 1 for REDIR_IN.
 */
void	handle_redin_or_heredoc(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 0;
	if (*(++line) == '<')
	{
		str = ft_strdup("<<");
		token = create_token(str, HEREDOC, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("<");
		token = create_token(str, REDIR_IN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}
