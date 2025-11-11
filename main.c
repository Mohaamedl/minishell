#include "minishell.h"

int	main(void)
{

	char *line;
	t_token *head;
	t_ast *first_node;

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
		free_node_list(first_node);
		free_tokens(head);
		free(line);
	}
	return exit_status;
}
