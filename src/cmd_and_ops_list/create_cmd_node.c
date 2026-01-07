/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:32 by framiran          #+#    #+#             */
/*   Updated: 2025/11/18 13:31:44 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles a redirection token and adds it to the redirect list
 * 
 * Creates a new redirection node from the current token and appends it to
 * the command's redirection list. The file name is taken from the next token.
 * For heredoc, the delimiter never expands, but this information is stored
 * to determine if content variables will expand. Updates the token pointer
 * by skipping both the redirect token and the filename/delimiter token.
 * 
 * @param cmd The command structure to add the redirection to
 * @param tmp_token Pointer to the current token pointer
 * @param redir_list_head Pointer to the head of the redirection list
 * @param redir_list_last Pointer to the last node in the redirection list
 */
void	handle_redirect_token(t_cmd *cmd, t_token **tmp_token,
		t_redir **redir_list_head, t_redir **redir_list_last)
{
	t_redir	*new_redir_node;

	new_redir_node = malloc(sizeof(t_redir));
	if (!new_redir_node)
		return ;
	new_redir_node->type = (*tmp_token)->type;
	new_redir_node->file = (*tmp_token)->next->value;
	new_redir_node->file_name_is_expandable = (*tmp_token)->next->expandable;
	new_redir_node->file_was_expanded = 0;
	if (*redir_list_head == NULL)
	{
		*redir_list_head = new_redir_node;
		*redir_list_last = new_redir_node;
		(*redir_list_last)->next = NULL;
		cmd->redirs = *redir_list_head;
	}
	else
	{
		(*redir_list_last)->next = new_redir_node;
		(*redir_list_last) = new_redir_node;
		(*redir_list_last)->next = NULL;
	}
	(*tmp_token) = (*tmp_token)->next;
	(*tmp_token) = (*tmp_token)->next;
}

/**
 * @brief Handles an argument token and adds it to the argument list
 * 
 * Creates a new argument node from the current token and appends it to
 * the command's argument list. Wildcard expansion is only enabled for
 * unquoted tokens. The value initially points to the token value and
 * may be replaced if expansion occurs. Updates the token pointer to
 * the next token.
 * 
 * @param cmd The command structure to add the argument to
 * @param tmp_token Pointer to the current token pointer
 * @param arg_list_head Pointer to the head of the argument list
 * @param arg_list_last Pointer to the last node in the argument list
 */
void	handle_arg_token(t_cmd *cmd, t_token **tmp_token,
		t_arg **arg_list_head, t_arg **arg_list_last)
{
	t_arg	*new_arg_node;

	new_arg_node = malloc(sizeof(t_arg));
	if (!new_arg_node)
		return ;
	new_arg_node->value = (*tmp_token)->value;
	new_arg_node->is_expandable = (*tmp_token)->expandable;
	new_arg_node->is_wildcard_expandable = !((*tmp_token)->is_quoted);
	new_arg_node->was_expanded = 0;
	if (*arg_list_head == NULL)
	{
		*arg_list_head = new_arg_node;
		*arg_list_last = new_arg_node;
		(*arg_list_last)->next = NULL;
		cmd->args = *arg_list_head;
	}
	else
	{
		(*arg_list_last)->next = new_arg_node;
		(*arg_list_last) = new_arg_node;
		(*arg_list_last)->next = NULL;
	}
	(*tmp_token) = (*tmp_token)->next;
}

/**
 * @brief Creates a command structure from tokens
 * 
 * Parses tokens to build a complete command with its arguments and
 * redirections. The command name is always the first token and is also
 * stored in the arguments list. Continues processing tokens until an
 * operator is found or the token list ends. Updates the token pointer
 * internally, skipping 2 tokens for redirects or 1 token for arguments.
 * 
 * @param tmp_token The first token of the command
 * @return Pointer to the newly created command structure
 */
t_cmd	*create_cmd(t_token *tmp_token)
{
	t_cmd	*cmd;
	t_redir	*redir_list_head;
	t_redir	*redir_list_last;
	t_arg	*arg_list_head;
	t_arg	*arg_list_last;

	redir_list_head = NULL;
	redir_list_last = NULL;
	arg_list_head = NULL;
	arg_list_last = NULL;
	cmd = malloc(sizeof(t_cmd));
	cmd->cmd_name = tmp_token->value;
	cmd->args = NULL;
	cmd->redirs = NULL;
	while (tmp_token && tmp_token->is_operator == 0)
	{
		if (is_redirect_token(tmp_token))
			handle_redirect_token(cmd, &tmp_token, &redir_list_head,
				&redir_list_last);
		else
			handle_arg_token(cmd, &tmp_token, &arg_list_head, &arg_list_last);
	}
	return (cmd);
}

/**
 * @brief Creates a command node and adds it to the AST list
 * 
 * Creates a complete command structure and wraps it in an AST node. If this
 * is the first node (token equals head), initializes both first_node and
 * last_node. Otherwise, appends the new node to the end of the list. Updates
 * the token pointer to skip all tokens belonging to this command (arguments
 * and redirections) by calling update_token_to_eval.
 * 
 * @param tmp_token Pointer to the current token pointer
 * @param head The head of the token list (used to check if first node)
 * @param first_node Pointer to the first node in the AST list
 * @param last_node Pointer to the last node in the AST list
 */
void	create_cmd_node(t_token **tmp_token, t_token *head,
		t_ast **first_node, t_ast **last_node)
{
	t_cmd	*cmd;
	t_ast	*new_node;

	cmd = create_cmd(*tmp_token);
	new_node = create_node(*tmp_token, cmd);
	if (*tmp_token == head)
	{
		*first_node = new_node;
		*last_node = new_node;
		(*last_node)->right = NULL;
	}
	else
	{
		append_node(new_node, *last_node);
		*last_node = new_node;
	}
	update_token_to_eval(tmp_token);
}
