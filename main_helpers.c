#include "minishell.h"

//vai buscar o ultimo node da lista, preciso dele na funcao build_tree()
t_ast *	get_last_node(t_ast *head)
{
	t_ast *tmp;
	tmp = head;
	while(tmp)
	{
		if(tmp ->right == NULL) //cheguei ao fim da lista
			return tmp;
		tmp = tmp ->right;
	}
	return tmp;
}
