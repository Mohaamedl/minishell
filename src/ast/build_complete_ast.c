/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_complete_ast.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 12:35:00 by framiran          #+#    #+#             */
/*   Updated: 2026/01/14 12:35:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Build complete AST from token list
 *
 * Creates a complete Abstract Syntax Tree by first building a flat list
 * of commands and operators, then converting it to a binary tree, and
 * finally processing any parenthesis subtrees.
 *
 * @param head Head of the token list
 * @return Root of the complete AST
 */
t_ast	*build_complete_ast(t_token *head)
{
	t_ast	*root;
	t_ast	*last_node;

	root = build_cmds_and_ops_list(head);
	if (!root)
		return (NULL);
	last_node = get_last_node(root);
	root = build_tree(root, last_node);
	build_sub_trees(&root);
	return (root);
}
