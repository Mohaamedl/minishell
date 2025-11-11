/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:18:03 by framiran          #+#    #+#             */
/*   Updated: 2025/11/11 10:18:18 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	free_tokens(t_token *head)
{
	t_token *tmp;
	t_token *next_token;
	tmp = head;
	while(tmp)
	{
		next_token = tmp->next;
		free(tmp -> value);
		free(tmp);
		tmp = next_token;
	}
}
