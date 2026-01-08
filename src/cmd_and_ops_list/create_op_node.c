/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_op_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:40 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 16:11:35 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates an operator node and adds it to the AST list
 * 
 * Creates an AST node for an operator token (no command structure). If this
 * is the first node (token equals head), initializes both first_node and
 * last_node. Otherwise, appends the new node to the end of the list using
 * append_node. Updates the token pointer to the next token in the list.
 * 
 * @param tmp_token Pointer to the current token pointer (operator token)
 * @param head The head of the token list (used to check if first node)
 * @param first_node Pointer to the first node in the AST list
 * @param last_node Pointer to the last node in the AST list
 */
void	create_op_node(t_token **tmp_token, t_token *head,
		t_ast **first_node, t_ast **last_node)
{
	t_ast	*new_node;

	new_node = create_node(*tmp_token, NULL);
	if (*tmp_token == head)
	{
		*first_node = new_node;
		*last_node = new_node;
	}
	else
	{
		append_node(new_node, *last_node);
		*last_node = new_node;
	}
	*tmp_token = (*tmp_token)->next;
}
