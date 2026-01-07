/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:16:51 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 14:53:31 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the entire redirections linked list
 * 
 * Iterates through the redirection list and frees each node. If a file
 * path was expanded (malloc'd by expansion), it frees that memory as well.
 * Original token values are not freed here as they are freed with the
 * token list.
 * 
 * @param head The head of the redirections list to free
 */
void	free_redirects_list(t_redir *head)
{
	t_redir	*tmp;
	t_redir	*next_redir;

	tmp = head;
	while (tmp)
	{
		next_redir = tmp->next;
		if (tmp->file_was_expanded && tmp->file)
			free(tmp->file);
		free(tmp);
		tmp = next_redir;
	}
}

/**
 * @brief Frees the entire arguments linked list
 * 
 * Iterates through the argument list and frees each node. If an argument
 * value was expanded (malloc'd by expansion), it frees that memory as well.
 * Original token values are not freed here as they are freed with the
 * token list.
 * 
 * @param head The head of the arguments list to free
 */
void	free_args_list(t_arg *head)
{
	t_arg	*tmp;
	t_arg	*next_arg;

	tmp = head;
	while (tmp)
	{
		next_arg = tmp->next;
		if (tmp->was_expanded && tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = next_arg;
	}
}

/**
 * @brief Frees a command structure and its associated lists
 * 
 * Frees the command's redirection list, argument list, and the command
 * structure itself. The cmd_name is not freed as it points to a token
 * value which is freed separately in free_token().
 * 
 * @param cmd The command structure to free
 */
void	free_cmd(t_cmd *cmd)
{
	free_redirects_list(cmd->redirs);
	free_args_list(cmd->args);
	free(cmd);
}

/**
 * @brief Frees a single AST node and its command if present
 * 
 * If the node contains a command structure, frees it using free_cmd.
 * Then frees the node itself. Does not free neighboring nodes.
 * 
 * @param node The AST node to free
 */
void	free_node(t_ast *node)
{
	if (node->cmd != NULL)
		free_cmd(node->cmd);
	free(node);
}

/**
 * @brief Frees the entire AST node list
 * 
 * Iterates through the AST list following the right pointers and frees
 * each node. This is a temporary function for the linked list structure.
 * In the future, this will be adapted to free a proper tree structure.
 * 
 * @param head The head of the AST list to free
 */
void	free_node_list(t_ast *head)
{
	t_ast	*tmp;
	t_ast	*next_node;

	tmp = head;
	while (tmp)
	{
		next_node = tmp->right;
		free_node(tmp);
		tmp = next_node;
	}
}
