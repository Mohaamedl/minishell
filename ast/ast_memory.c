/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:10 by framiran          #+#    #+#             */
/*   Updated: 2025/11/13 14:22:03 by framiran         ###   ########.fr       */
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
