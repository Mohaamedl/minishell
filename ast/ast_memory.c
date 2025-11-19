/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:10 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 14:25:08 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//esta funcao serve para me livrar dos nos de parentesis quando ja nao preciso mais deles (quando construo as subtrees)
void	free_parentesis_nodes(t_ast *start_node,t_ast *end_node)
{
	free_node(start_node -> left);
	free_node(end_node -> right);
	start_node -> left = NULL;
	end_node -> right = NULL;
}

void free_tree(t_ast *root)
{
	t_ast *tmp_left;
	t_ast *tmp_right;
	if(root == NULL)
		return;
	tmp_left = root -> left;
	tmp_right = root ->right;
	free_node(root);
	free_tree(tmp_left);
	free_tree(tmp_right);
}
