/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:44:03 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 16:20:15 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Recursively builds an AST from a range of nodes
 *
 * Constructs an Abstract Syntax Tree by finding the split operator node
 * (OR, AND, or PIPE) and recursively building left and right subtrees.
 * When the main tree is returned (without considering subtrees), only
 * isolated "leaves" remain (commands or subtrees). The split operator
 * is found based on precedence (OR/AND first, then PIPE). If no operator
 * is found, returns the start node (an isolated command or subtree).
 *
 * @param start_node Pointer to the first node in the range
 * @param end_node Pointer to the last node in the range
 * @return Pointer to the root of the constructed AST
 */
t_ast	*build_tree(t_ast *start_node, t_ast *end_node)
{
	t_ast	*tmp_node;
	t_ast	*original_start_node;
	t_ast	*original_end_node;

	original_start_node = start_node;
	original_end_node = end_node;
	tmp_node = get_split_op_node(start_node, end_node);
	if (tmp_node != NULL)
	{
		end_node = tmp_node->left;
		end_node->right = NULL;
		tmp_node->left = build_tree(original_start_node, end_node);
		start_node = tmp_node->right;
		start_node->left = NULL;
		tmp_node->right = build_tree(start_node, original_end_node);
		return (tmp_node);
	}
	else
		return (start_node);
}

/**
 * @brief Recursively builds subtrees for parenthesized expressions
 *
 * Processes the AST to build subtrees for expressions within parentheses.
 * When a parenthesis node is found, extracts the content between the
 * parentheses and builds a subtree from it. The root pointer may change
 * if the root is a subtree (e.g., (a && b) will point to && instead of
 * the opening parenthesis). Recursively processes operator nodes to handle
 * nested subtrees.
 *
 * @param root_node Double pointer to the root node (may be modified)
 */
void	build_sub_trees(t_ast **root_node)
{
	t_ast	*start_node;
	t_ast	*end_node;

	if (!root_node || !(*root_node))
		return ;
	if ((*root_node)->type == CMD)
		return ;
	if ((*root_node)->type == LPAREN)
	{
		start_node = (*root_node)->right;
		end_node = skip_subtree_nodes(*root_node);
		if (!end_node || !end_node->left)
			return ;
		end_node = end_node->left;
		free_parentesis_nodes(start_node, end_node);
		*root_node = build_tree(start_node, end_node);
		build_sub_trees(root_node);
		return ;
	}
	build_sub_trees(&((*root_node)->left));
	build_sub_trees(&((*root_node)->right));
}
