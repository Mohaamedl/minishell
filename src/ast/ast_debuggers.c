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

static void	print_tree_visual_helper(t_ast *root, char *prefix, int is_left)
{
	char *node_str;
	char *new_prefix;
	
	if (root == NULL)
		return;
	
	// Print current node
	printf("%s", prefix);
	printf("%s", is_left ? "├── " : "└── ");
	
	if (root->type == CMD ||root->type == 11 ) //just because was bugging
		node_str = root->cmd->cmd_name;
	else
		node_str = (char *)token_type_to_str(root->type);
	printf("%s\n", node_str);
	
	// Prepare prefix for children
	new_prefix = malloc(strlen(prefix) + 5);
	strcpy(new_prefix, prefix);
	strcat(new_prefix, is_left ? "│   " : "    ");
	
	// Print children (right first, then left for visual clarity)
	if (root->left != NULL || root->right != NULL)
	{
		if (root->right != NULL)
			print_tree_visual_helper(root->right, new_prefix, root->left != NULL);
		if (root->left != NULL)
			print_tree_visual_helper(root->left, new_prefix, 0);
	}
	
	free(new_prefix);
}

void	print_tree_visual(t_ast *root)
{
	char *node_str;
	
	if (root == NULL)
	{
		printf("(empty tree)\n");
		return;
	}
	
	// Print root
	if (root->type == CMD)
		node_str = root->cmd->cmd_name;
	else
		node_str = (char *)token_type_to_str(root->type);
	printf("%s\n", node_str);
	
	// Print children
	if (root->left != NULL || root->right != NULL)
	{
		if (root->right != NULL)
			print_tree_visual_helper(root->right, "", root->left != NULL);
		if (root->left != NULL)
			print_tree_visual_helper(root->left, "", 0);
	}
}
