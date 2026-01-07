/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:18:03 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:28 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees all tokens in a linked list
 * @param head Pointer to the first token in the list
 *
 * Iterates through the token list, freeing each token's value and the token
 * structure itself. Handles NULL pointers safely.
 */
void	free_tokens(t_token *head)
{
	t_token	*tmp;
	t_token	*next_token;

	tmp = head;
	while (tmp)
	{
		next_token = tmp->next;
		free(tmp->value);
		free(tmp);
		tmp = next_token;
	}
}
