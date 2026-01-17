/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:19:10 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 16:20:16 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees parenthesis nodes when building subtrees
 *
 * This function removes the parenthesis nodes after constructing subtrees
 * by freeing the left child of the start node and the right child of the
 * end node, then setting those pointers to NULL. Used when parentheses are
 * no longer needed after subtree construction.
 *
 * @param start_node Pointer to the node after the opening parenthesis
 * @param end_node Pointer to the node before the closing parenthesis
 */
void	free_parentesis_nodes(t_ast *start_node, t_ast *end_node)
{
	free_node(start_node->left);
	free_node(end_node->right);
	start_node->left = NULL;
	end_node->right = NULL;
}

/**
 * @brief Recursively frees an entire AST
 *
 * Performs a post-order traversal to free all nodes in the tree.
 * Saves pointers to children before freeing the current node to avoid
 * use-after-free errors.
 *
 * @param root Pointer to the root of the tree to free
 */
void	free_tree(t_ast *root)
{
	t_ast	*tmp_left;
	t_ast	*tmp_right;

	if (root == NULL)
		return ;
	tmp_left = root->left;
	tmp_right = root->right;
	free_node(root);
	free_tree(tmp_left);
	free_tree(tmp_right);
}
