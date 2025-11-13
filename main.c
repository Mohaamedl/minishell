#include "minishell.h"



int	main(void)
{

	char *line;
	t_token *head;
	t_ast *first_node;
	t_ast *end_node; //podera nao ser preciso se eu ao cortar em dois num pipe por exemplo definir right do que node a esquerda do pipe como null
	t_ast *root_node;

	int exit_status = 0;
	while (1)
	{
		line = readline("minishell$>");
		//add_history(line);
		head = tokenize(line);
		if (!head)
		{
			fprintf(stderr, "Erro ao tokenizar.\n");
			free(line);
			continue;
		}
		print_tokens(head);
		first_node = build_cmds_and_ops_list(head);
		print_nodes(first_node);
		end_node = get_last_node(first_node);//vai buscar o ultimo node da lista, preciso dele na funcao build_tree()
		root_node = build_tree(first_node, end_node); //esta funcao cria a tree mas nao lida com as subtrees
		build_sub_trees(&root_node);
		//free_node_list(first_node);
		free_tokens(head);
		free(line);
	}
	return exit_status;
}
