/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/19 20:21:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(void)
{
	char *line;
	t_token *head;
	t_ast *first_node;
	t_ast *end_node;
	t_ast *root_node;

	while (1)
	{
		line = readline("minishell$>");
		if (!line)
			break ;
		head = tokenize(line);
		if (!head)
		{
			fprintf(stderr, "Erro ao tokenizar.\n");
			free(line);
			continue;
		}
		first_node = build_cmds_and_ops_list(head);
		end_node = get_last_node(first_node);
		root_node = build_tree(first_node, end_node);
		build_sub_trees(&root_node);
		print_tree(root_node);
		free_tree(root_node);
		free_tokens(head);
		free(line);
	}
	return (0);
}
