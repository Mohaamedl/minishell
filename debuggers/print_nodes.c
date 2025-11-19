/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:00:17 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 14:15:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("    [REDIR] ");
		printf("type: %s |",token_type_to_str(redir->type));
		printf("file name: %s | Expandable file name: %i\n", redir->file ? redir->file : "(null)", redir->file_name_is_expandable);
		redir = redir->next;
	}
}

void	print_args(t_arg *arg)
{
	while (arg)
	{
		printf("    [ARG] ");
		printf("value: %s | Expandable: %i\n", arg->value ? arg->value : "(null)", arg->is_expandable);
		arg = arg->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	if (!cmd)
	{
		printf("  [CMD] NULL\n");
		return;
	}
	printf("  [CMD] %s\n", cmd->cmd_name ? cmd->cmd_name : "(null)");
	print_args(cmd->args);
	print_redirs(cmd->redirs);
}

void	print_nodes(t_ast *node)
{
	int	i = 0;

	while (node)
	{
		printf("NODE #%d\n", i);
		printf("  Type: %s\n", token_type_to_str(node->type));
		if (node->cmd)
			print_cmd(node->cmd);
		else
			printf("  [CMD] NULL\n");
		printf("----------------------\n");
		node = node->right;
		i++;
	}
}
