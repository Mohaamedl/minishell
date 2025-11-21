/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/21 11:48:46 by framiran         ###   ########.fr       */
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
		head = tokenize(line);// vai receber o struct shell para atualizar o status code quando as aspas nao fecharem;
		if (!head)
		{
			printf("Error: Unclosed quotes\n"); //esta e a unica validacao que faco antes de ter a lista de tokens, neste ponto so avalio se as quotes fecham ou nao
			free(line);
			continue; //passa para a proxima execucao do while(nao faco nada do que vem abaixo)
		}
		if (validate_token_list(head) == 1)// vai receber o struct shell para atualizar o status code se a syntax nao for valida; poderar ter uma
		{									//ponderar ter uma var msg nessa struct para definir a msg a retornar
			first_node = build_cmds_and_ops_list(head);
			print_nodes(first_node);
			end_node = get_last_node(first_node);
			root_node = build_tree(first_node, end_node);
			build_sub_trees(&root_node);
			print_tree(root_node);
			free_tree(root_node);
		}
		free_tokens(head);
		free(line);
	}
	return (0);
}
