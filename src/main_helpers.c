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

/**
 * @brief Get the last node of the AST linked list
 *
 * Traverses the AST linked list through the right pointers to find the last
 * node. This is needed for the build_tree() function.
 *
 * @param head Head of the AST linked list
 * @return Pointer to the last node in the list, or NULL if list is empty
 */
t_ast	*get_last_node(t_ast *head)
{
	t_ast	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->right == NULL)
			return (tmp);
		tmp = tmp->right;
	}
	return (tmp);
}
