#include "minishell.h"

/**
 * @brief Check if a string contains only whitespace
 *
 * @param str String to check
 * @return 1 if only whitespace/empty, 0 otherwise
 */
int	is_empty_or_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\r')
			return (0);
		str++;
	}
	return (1);
}

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
