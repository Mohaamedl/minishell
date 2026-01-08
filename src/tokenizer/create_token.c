/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:56:37 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:19 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a token from quoted text and adds it to the token list
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @param line Pointer to string starting at a quote character
 * @param quote The quote character (single or double)
 * @return 1 on success, 0 on failure (unclosed quotes)
 *
 * Creates a WORD token from the quoted text. Single quotes disable variable
 * expansion. Double quotes allow expansion except for heredoc delimiters.
 * Sets is_quoted flag to prevent wildcard expansion.
 */
int	create_quoted_token(t_token **last_token, t_token **head, char *line,
		char quote)
{
	char	*str;
	t_token	*token;
	int		is_expandable;
	int		is_op;

	is_op = 0;
	is_expandable = 1;
	if (is_single_quote(*line))
		is_expandable = 0;
	if (last_token && *last_token)
	{
		if ((*last_token)->type == HEREDOC)
			is_expandable = 0;
	}
	str = get_quoted_text(line, quote);
	if (str == NULL)
		return (0);
	else
	{
		token = create_token(str, WORD, is_expandable, is_op);
		token->is_quoted = 1;
		append_token(head, last_token, token);
	}
	return (1);
}

/**
 * @brief Creates a new token with the given parameters
 * @param value String value of the token (must be dynamically allocated)
 * @param type Type of the token (WORD, PIPE, REDIR_IN, etc.)
 * @param is_expandable 1 if token can have variable expansion, 0 otherwise
 * @param is_op 1 if token is an operator, 0 otherwise
 * @return Pointer to the newly created token, or NULL on allocation failure
 *
 * Allocates memory for a new token and initializes all fields.
 * The is_quoted field is set to 0 by default and next is set to NULL.
 */
t_token	*create_token(char *value, t_token_type type, int is_expandable,
		int is_op)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->is_operator = is_op;
	token->type = type;
	token->value = value;
	token->expandable = is_expandable;
	token->is_quoted = 0;
	token->next = NULL;
	return (token);
}
