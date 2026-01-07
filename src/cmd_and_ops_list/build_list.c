/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:21:40 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:21:51 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Appends a new node to the end of the linked list
 * 
 * This function links a new node to the last node in the list by setting
 * the appropriate pointers. The new node's right pointer is set to NULL
 * and its left pointer to the last node. The last node's right pointer
 * is updated to point to the new node.
 * 
 * @param new_node The node to append to the list
 * @param last_node The current last node in the list
 */
void	append_node(t_ast *new_node, t_ast *last_node)
{
	last_node->right = new_node;
	new_node->left = last_node;
	new_node->right = NULL;
}

/**
 * @brief Creates a new AST node for either a command or an operator
 * 
 * Allocates and initializes a new AST node. If cmd is NULL, creates an
 * operator node with the token's type. Otherwise, creates a command node
 * with the provided command structure. WORD tokens found at the beginning
 * of the list or after an operator are command names, followed by arguments
 * or redirections. Operators separate commands.
 * 
 * @param token The token associated with this node
 * @param cmd The command structure (NULL for operator nodes)
 * @return Pointer to the newly created AST node
 */
t_ast	*create_node(t_token *token, t_cmd *cmd)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (cmd == NULL)
	{
		new_node->cmd = NULL;
		new_node->type = token->type;
		new_node->right = NULL;
		new_node->left = NULL;
	}
	else
	{
		new_node->cmd = cmd;
		new_node->type = CMD;
		new_node->right = NULL;
		new_node->left = NULL;
	}
	return (new_node);
}

/**
 * @brief Builds a linked list of command and operator nodes from tokens
 * 
 * Iterates through the token list and creates AST nodes for each command
 * or operator found. Commands are created when a non-operator token is
 * encountered, and operator nodes are created for operator tokens. The
 * function updates the token pointer as it processes arguments and redirects
 * for commands.
 * 
 * @param head The head of the token list to process
 * @return Pointer to the first node of the created AST list
 */
t_ast	*build_cmds_and_ops_list(t_token *head)
{
	t_token	*tmp_token;
	t_ast	*first_node;
	t_ast	*last_node;

	tmp_token = head;
	first_node = NULL;
	last_node = NULL;
	while (tmp_token)
	{
		if (tmp_token->is_operator == 0)
			create_cmd_node(&tmp_token, head, &first_node, &last_node);
		else
			create_op_node(&tmp_token, head, &first_node, &last_node);
	}
	return (first_node);
}

