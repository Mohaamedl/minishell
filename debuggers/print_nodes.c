/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:00:17 by framiran          #+#    #+#             */
/*   Updated: 2025/11/13 14:21:00 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("    [REDIR] ");
		printf("%s ",token_type_to_str(redir->type));
		printf(" %s\n", redir->file ? redir->file : "(null)");
		redir = redir->next;
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
	if (cmd->args)
	{
		printf("    Args:");
		for (int i = 0; cmd->args[i]; i++)
			printf(" \"%s\"", cmd->args[i]);
		printf("\n");
	}
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
