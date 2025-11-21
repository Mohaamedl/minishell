/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_node_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:19 by framiran          #+#    #+#             */
/*   Updated: 2025/11/21 12:10:38 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// possible redirect types: < ,> ,>>, <<
int	is_redirect_token(t_token *tmp_token)
{
	if(ft_strncmp(tmp_token -> value, ">>", 2) == 0)
		return 1;
	if(ft_strncmp(tmp_token -> value, "<<", 2) == 0)
		return 1;
	if(ft_strncmp(tmp_token -> value, ">", 1) == 0)
		return 1;
	if(ft_strncmp(tmp_token -> value, "<", 1) == 0)
		return 1;
	return 0;
}


void	update_token_to_eval(t_token **tmp_token) //esta funcao e chamada para atualizar o token a avaliar depois de criar um node comando (tenho de saltar redirects e argumantos a frente)
{
	while((*tmp_token))
	{
		if((*tmp_token) ->is_operator) //se emcontrar um operador atualiza tmp_token para a posicao desse operador
			return;
		*tmp_token = (*tmp_token) -> next;
	}
	//se sair aqui, nao encontrou nenhum operador antes do fim entao acabou a construcao da lista (tmp_token = NULL)
}



