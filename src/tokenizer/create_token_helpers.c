/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:58:15 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:17 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calculates the size of content within quotes
 * @param line Pointer to the string starting at a quote character
 * @param quote The quote character to match (single or double quote)
 * @return Size of content between quotes, or -1 if quote is not closed
 *
 * Skips the opening quote and counts characters until the closing quote.
 * Returns -1 if the string ends before finding the closing quote.
 */
int	get_quoted_size(char *line, char quote)
{
	int	size;

	size = 0;
	line++;
	while (*line != quote && *line != '\0')
	{
		size++;
		line++;
	}
	if (*line == '\0')
		return (-1);
	return (size);
}

/**
 * @brief Extracts text content from within quotes
 * @param line Pointer to string starting at a quote character
 * @param quote The quote character to match
 * @return Dynamically allocated string with content between quotes, or NULL
 *
 * Allocates memory and copies the content between the opening and closing
 * quotes. Returns NULL if the quote is not closed properly.
 */
char	*get_quoted_text(char *line, char quote)
{
	int		size;
	char	*str;

	size = get_quoted_size(line, quote);
	if (size == -1)
		return (NULL);
	else
	{
		str = malloc((size + 1) * sizeof(char));
		ft_memcpy(str, ++line, size);
		str[size] = '\0';
		return (str);
	}
}

/**
 * @brief Appends a token to the end of the token list
 * @param head Pointer to pointer of the first token in the list
 * @param last_token Pointer to pointer of the last token in the list
 * @param new_token Token to append to the list
 *
 * If the list is empty, sets head to point to new_token. Otherwise, links
 * the new token to the end of the list. Updates last_token to point to
 * the newly added token. Does nothing if new_token is NULL.
 */
void	append_token(t_token **head, t_token **last_token, t_token *new_token)
{
	if (!new_token)
		return ;
	if (*head == NULL)
		*head = new_token;
	else
		(*last_token)->next = new_token;
	*last_token = new_token;
}
