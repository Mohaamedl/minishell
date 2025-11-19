/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_op_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:40 by framiran          #+#    #+#             */
/*   Updated: 2025/11/13 11:33:15 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_op_node(t_token **tmp_token, t_token *head,t_ast **first_node, t_ast **last_node)
{
	t_ast *new_node;

	new_node  = create_node(*tmp_token, NULL);
	if(*tmp_token == head) // no caso de ser o primeiro node:
	{
		*first_node = new_node;
		*last_node = new_node;
	}
	else
	{
		append_node(new_node,*last_node);//ligo estes dois nodes e atualizo last_node
		*last_node = new_node; // agora last_node aponta para o novo node que foi adicionado ao fim da lista
	}
	*tmp_token = (*tmp_token) -> next;
}
