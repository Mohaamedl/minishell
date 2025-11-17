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

//esta funcao recebe o node com o primeiro parentesis e vai saltar para o ultimo parentesis, a funcao get_OP_node_based_on_type()
//depois avalia se este e o ultimo node ou nao se for retorna null (nao encontrou operador) caso nao seja o ultimo passa para a frente
t_ast	*skip_subtree_nodes(t_ast *tmp_node)
{
	while(tmp_node)
	{
		if(tmp_node -> type == RPAREN)
			return (tmp_node);
		tmp_node = tmp_node -> right;
	}
	return (NULL);
	//se chegar aqui e porque nao encontrou a o parentesis que fecha ")", vou ter que pensar no que fazer neste caso, penso nao ser presico lidar
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

t_ast *get_split_op_node(t_ast *start_node, t_ast *end_node)
{
	t_ast *tmp_node;

	tmp_node = NULL;
	tmp_node = get_OP_node_based_on_type(start_node, end_node, OR);
	if (tmp_node != NULL)
		return tmp_node;
	else
	{
		tmp_node = get_OP_node_based_on_type(start_node, end_node, AND);
		if (tmp_node != NULL)
			return tmp_node;
		else
		{
			tmp_node = get_OP_node_based_on_type(start_node, end_node, PIPE);
			if (tmp_node != NULL)
				return tmp_node;
		}
	}
	return NULL;
}
