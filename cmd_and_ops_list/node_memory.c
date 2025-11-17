/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:16:51 by framiran          #+#    #+#             */
/*   Updated: 2025/11/17 14:53:31 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirects_list(t_redir *head)
{
	//nao ou free a file porque ja dou free em free_token(), (e um ponteiro para um value de determinado token)
	//nao existe memoria dinamicamente alocada nesta struct a nao ser a propria struct
	t_redir *tmp;
	t_redir *next_redir;
	tmp = head;
	while(tmp)
	{
		next_redir = tmp -> next;
		free(tmp);
		tmp = next_redir;
	}
}
//nao existe memoria dinamocamente alocada nesta lista a nao ser o propria lista, cada argumento value e' um pointer para value de um determinado token
void	free_args_list(t_arg *head)
{
	t_arg *tmp;
	t_arg *next_arg;
	tmp = head;
	while(tmp)
	{
		next_arg = tmp -> next;
		free(tmp);
		tmp = next_arg;
	}
}

void	free_cmd(t_cmd *cmd)
{
	free_redirects_list(cmd -> redirs);
	free_args_list(cmd -> args);
	//nao dou free ao cmd_name porque 'e um ponteiro para o value do token e ja dou free a essa memoria alocada em free_token()
	free(cmd);
}

void	free_node(t_ast *node)
{
	if(node -> cmd != NULL)
		free_cmd(node -> cmd);
	free(node);
}
void	free_node_list(t_ast *head) //esta funcao serve apenas para dar free a uma lista de comandos e operadores,
{									//e uma funcao temporaria, no futuro terei que a alterar para dar free a arvore
	t_ast *tmp;
	t_ast *next_node;
	tmp = head;
	while(tmp)
	{
		next_node = tmp -> right;
		free_node(tmp);
		tmp = next_node;
	}
}
