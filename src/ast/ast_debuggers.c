/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_debuggers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:07:45 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 16:20:15 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints a single AST node to stdout
 *
 * This function prints either the command name (if node is a CMD type)
 * or the string representation of the node's token type.
 *
 * @param node Pointer to the AST node to print
 */
void	print_tree_node(t_ast *node)
{
	if (node->type == CMD)
		printf("%s\n", node->cmd->cmd_name);
	else
		printf("%s\n", token_type_to_str(node->type));
}

/**
 * @brief Recursively prints the entire AST in pre-order traversal
 *
 * Performs a depth-first pre-order traversal of the AST,
 * printing each node using print_tree_node.
 *
 * @param root Pointer to the root node of the tree to print
 */
void	print_tree(t_ast *root)
{
	if (root == NULL)
		return ;
	print_tree_node(root);
	print_tree(root->left);
	print_tree(root->right);
}

/**
 * @brief Helper function for visual tree printing with tree-like formatting
 *
 * Recursively prints the AST with tree-like visual connectors (├── and └──).
 * Right children are printed before left children for better visual clarity.
 * Allocates a new prefix string for each level of recursion.
 *
 * @param root Pointer to current AST node
 * @param prefix String prefix for current indentation level
 * @param is_left Flag indicating if node is a left child (affects connector)
 */
static void	print_tree_visual_helper(t_ast *root, char *prefix, int is_left)
{
	char	*node_str;
	char	*new_prefix;

	if (root == NULL)
		return ;
	printf("%s", prefix);
	printf("%s", is_left ? "├── " : "└── ");
	if (root->type == CMD || root->type == 11)
		node_str = root->cmd->cmd_name;
	else
		node_str = (char *)token_type_to_str(root->type);
	printf("%s\n", node_str);
	new_prefix = malloc(strlen(prefix) + 5);
	strcpy(new_prefix, prefix);
	strcat(new_prefix, is_left ? "│   " : "    ");
	if (root->left != NULL || root->right != NULL)
	{
		if (root->right != NULL)
			print_tree_visual_helper(root->right, new_prefix,
				root->left != NULL);
		if (root->left != NULL)
			print_tree_visual_helper(root->left, new_prefix, 0);
	}
	free(new_prefix);
}

/**
 * @brief Prints the entire AST in a visual tree format
 *
 * Creates a visual representation of the AST using tree connectors.
 * Prints an empty tree message if root is NULL. The root is printed first,
 * then children are printed with appropriate indentation and connectors.
 * Right children are printed before left children.
 *
 * @param root Pointer to the root of the AST to visualize
 */
void	print_tree_visual(t_ast *root)
{
	char	*node_str;

	if (root == NULL)
	{
		printf("(empty tree)\n");
		return ;
	}
	if (root->type == CMD)
		node_str = root->cmd->cmd_name;
	else
		node_str = (char *)token_type_to_str(root->type);
	printf("%s\n", node_str);
	if (root->left != NULL || root->right != NULL)
	{
		if (root->right != NULL)
			print_tree_visual_helper(root->right, "", root->left != NULL);
		if (root->left != NULL)
			print_tree_visual_helper(root->left, "", 0);
	}
}
