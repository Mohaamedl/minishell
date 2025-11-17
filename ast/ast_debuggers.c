/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_debuggers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:07:45 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 13:13:34 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tree_node(t_ast *node)
{
	if(node->type == CMD)
		printf("%s\n", node->cmd->cmd_name);
	else
		printf("%s\n",token_type_to_str(node->type));
}
void	print_tree(t_ast *root)
{
	if(root == NULL)
		return;
	print_tree_node(root);
	print_tree(root->left);
	print_tree(root->right);
}
