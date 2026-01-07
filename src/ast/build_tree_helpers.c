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

/**
 * @brief Skips over a subtree enclosed in parentheses
 *
 * Starting from a node with an opening parenthesis (LPAREN), this function
 * traverses nodes until finding the matching closing parenthesis (RPAREN).
 * Handles nested parentheses by tracking nesting levels to correctly identify
 * the matching closing parenthesis. Used to skip over subtrees during tree
 * construction.
 *
 * @param tmp_node Pointer to the node with the opening parenthesis
 * @return Pointer to the node with the matching closing parenthesis,
 *         or NULL if not found
 */
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

/**
 * @brief Finds an operator node of a specific type in a range
 *
 * Searches for the first occurrence of an operator of the given type
 * between start_node and end_node. Skips over subtrees enclosed in
 * parentheses during the search.
 *
 * @param start_node Pointer to the first node in the search range
 * @param end_node Pointer to the last node in the search range
 * @param type The token type to search for
 * @return Pointer to the first matching operator node, or NULL if not found
 */
t_ast	*get_OP_node_based_on_type(t_ast *start_node, t_ast *end_node,
		t_token_type type)
{
	t_ast	*tmp_node;

	tmp_node = start_node;
	while (tmp_node)
	{
		if (tmp_node->type == LPAREN)
			tmp_node = skip_subtree_nodes(tmp_node);
		if (tmp_node->type == type)
			return (tmp_node);
		if (tmp_node == end_node)
			break ;
		tmp_node = tmp_node->right;
	}
	return (NULL);
}

/**
 * @brief Finds the rightmost operator of given type(s)
 *
 * For left-to-right associativity, splits at the rightmost operator.
 * This ensures (a op b op c) becomes op(op(a, b), c) not op(a, op(b, c)).
 * Skips over subtrees enclosed in parentheses during the search.
 * The check_and_or parameter determines whether to search for AND/OR
 * operators (when true) or PIPE operators (when false).
 *
 * @param start Pointer to the first node in the search range
 * @param end Pointer to the last node in the search range
 * @param check_and_or If 1, search for AND/OR; if 0, search for PIPE
 * @return Pointer to the rightmost matching operator, or NULL if not found
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
		{
			tmp = skip_subtree_nodes(tmp);
			if (!tmp)
				break ;
		}
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

/**
 * @brief Finds the split operator node for tree construction
 *
 * Searches for the operator to use as the root when splitting a node range
 * into left and right subtrees. Follows operator precedence rules:
 * first searches for AND/OR operators (rightmost), then PIPE operators
 * (rightmost). Returns the first match found according to precedence.
 *
 * @param start_node Pointer to the first node in the range
 * @param end_node Pointer to the last node in the range
 * @return Pointer to the split operator node, or NULL if none found
 */
t_ast	*get_split_op_node(t_ast *start_node, t_ast *end_node)
{
	t_ast	*tmp_node;

	tmp_node = get_rightmost_op(start_node, end_node, 1);
	if (tmp_node != NULL)
		return (tmp_node);
	tmp_node = get_rightmost_op(start_node, end_node, 0);
	return (tmp_node);
}
