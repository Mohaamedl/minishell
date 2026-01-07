/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_node_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:19 by framiran          #+#    #+#             */
/*   Updated: 2025/11/21 13:56:58 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a token represents a redirection operator
 * 
 * Tests if the token value matches any of the four possible redirect types:
 * append (>>), heredoc (<<), output (>), or input (<).
 * 
 * @param tmp_token The token to check
 * @return 1 if the token is a redirect operator, 0 otherwise
 */
int	is_redirect_token(t_token *tmp_token)
{
	if (ft_strncmp(tmp_token->value, ">>", 2) == 0)
		return (1);
	if (ft_strncmp(tmp_token->value, "<<", 2) == 0)
		return (1);
	if (ft_strncmp(tmp_token->value, ">", 1) == 0)
		return (1);
	if (ft_strncmp(tmp_token->value, "<", 1) == 0)
		return (1);
	return (0);
}

/**
 * @brief Updates the token pointer to the next operator or end of list
 * 
 * Called after creating a command node to skip past all tokens belonging
 * to that command (arguments and redirections). Advances the token pointer
 * until an operator is found or the end of the list is reached. When an
 * operator is found, tmp_token is positioned at that operator. If no
 * operator is found, tmp_token becomes NULL, indicating the end of list.
 * 
 * @param tmp_token Pointer to the current token pointer to update
 */
void	update_token_to_eval(t_token **tmp_token)
{
	while ((*tmp_token))
	{
		if ((*tmp_token)->is_operator)
			return ;
		*tmp_token = (*tmp_token)->next;
	}
}