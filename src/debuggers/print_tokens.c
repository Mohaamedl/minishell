/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:59:24 by framiran          #+#    #+#             */
/*   Updated: 2025/11/10 16:00:40 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

const char *token_type_to_str(t_token_type type)
{
	if (type == WORD)
		return "WORD";
	if (type == REDIR_IN)
		return "REDIR_IN";
	if (type == REDIR_OUT)
		return "REDIR_OUT";
	if (type == APPEND)
		return "APPEND";
	if (type == HEREDOC)
		return "HEREDOC";
	if (type == PIPE)
		return "PIPE";
	if (type == OR)
		return "OR";
	if (type == AND)
		return "AND";
	if (type == RPAREN)
		return "RPAREN";
	if (type == LPAREN)
		return "LPAREN";
	if (type == CMD)
		return "CMD";
	return "UNKNOWN";
}

void	print_tokens(t_token *tmp)
{
	while(tmp)
	{
		printf("Token: \"%s\"| type: %s | Expandable: %d | is operator: %d \n", tmp->value,  token_type_to_str(tmp->type), tmp ->expandable, tmp->is_operator);
		tmp = tmp->next;
	}
}
