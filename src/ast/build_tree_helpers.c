/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:15:51 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 14:18:45 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//esta funcao recebe o node com o primeiro parentesis e vai saltar para o ultimo parentesis correspondente
//agora conta niveis de aninhamento para lidar com parentesis aninhados ((a && b))
t_ast	*skip_subtree_nodes(t_ast *tmp_node)
{
	int	level;

	level = 0;
	while (tmp_node)
	{
		if (tmp_node->type == LPAREN)
			level++;
		if (tmp_node->type == RPAREN)
		{
			level--;
			if (level == 0)
				return (tmp_node);
		}
		tmp_node = tmp_node->right;
	}
	return (NULL);
}

t_ast	*get_OP_node_based_on_type(t_ast *start_node, t_ast *end_node, t_token_type type)
{
	t_ast *tmp_node;

	tmp_node = start_node;
	while(tmp_node)
	{
		if(tmp_node ->type == LPAREN)
			tmp_node = skip_subtree_nodes(tmp_node);
		if (tmp_node ->type == type)
			return tmp_node;
		if(tmp_node == end_node)//tinha chegado ao fim e ja avaliei este node nas linhas acima entao dou break do ciclo
			break;
		tmp_node = tmp_node -> right;
	}
	return (NULL);
}

/**
 * @brief Find the rightmost operator of given type(s)
 * 
 * For left-to-right associativity, we need to split at the rightmost operator.
 * This ensures (a op b op c) becomes op(op(a, b), c) not op(a, op(b, c)).
 */
static t_ast	*get_rightmost_op(t_ast *start, t_ast *end, int check_and_or)
{
	t_ast	*tmp;
	t_ast	*rightmost;

	rightmost = NULL;
	tmp = start;
	while (tmp)
	{
		if (tmp->type == LPAREN)
			tmp = skip_subtree_nodes(tmp);
		if (check_and_or && (tmp->type == OR || tmp->type == AND))
			rightmost = tmp;
		else if (!check_and_or && tmp->type == PIPE)
			rightmost = tmp;
		if (tmp == end)
			break ;
		tmp = tmp->right;
	}
	return (rightmost);
}

t_ast	*get_split_op_node(t_ast *start_node, t_ast *end_node)
{
	t_ast	*tmp_node;

	// First priority: rightmost AND or OR (same precedence, left-associative)
	tmp_node = get_rightmost_op(start_node, end_node, 1);
	if (tmp_node != NULL)
		return (tmp_node);
	// Second priority: rightmost PIPE
	tmp_node = get_rightmost_op(start_node, end_node, 0);
	return (tmp_node);
}
