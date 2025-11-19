/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:11:44 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:05:45 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*curr_node;
	t_list	*next_node;

	if (!lst || !*lst || !del)
		return ;
	curr_node = *lst;
	while (curr_node)
	{
		next_node = curr_node -> next;
		del(curr_node -> content);
		free (curr_node);
		curr_node = next_node;
	}
	*lst = NULL;
}
