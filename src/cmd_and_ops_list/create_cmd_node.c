/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:07:32 by framiran          #+#    #+#             */
/*   Updated: 2025/11/18 13:31:44 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	handle_redirect_token(t_cmd *cmd,t_token **tmp_token, t_redir **redir_list_head, t_redir **redir_list_last)
{
	t_redir	*new_redir_node;

	new_redir_node = malloc(sizeof(t_redir));

	new_redir_node = malloc(sizeof(t_redir));
	if (!new_redir_node)
		return;
	new_redir_node -> type = (*tmp_token) -> type;
	new_redir_node -> file = (*tmp_token) -> next -> value; // o file name e o token a seguir ao token de redirect
	new_redir_node -> file_name_is_expandable = (*tmp_token) -> next -> expandable;
	new_redir_node -> file_was_expanded = 0; // Initially points to token value
	if(*redir_list_head == NULL)//ainda nao existe head, este sera o primeiro redir_node
	{
		*redir_list_head = new_redir_node;
		*redir_list_last = new_redir_node;
		(*redir_list_last) -> next = NULL;
		cmd -> redirs = *redir_list_head; //ligo o cmd ao primeiro redirect emcontrado
	}
	else
	{
		(*redir_list_last) -> next = new_redir_node;
		(*redir_list_last) = new_redir_node;
		(*redir_list_last) ->next = NULL;
	}
	//atualizo o token a avaliar (salto 2 tokens, o token do redir e o filename/end of file no caso do heredoc)
	(*tmp_token) = (*tmp_token) -> next;
	(*tmp_token) = (*tmp_token) -> next;
}

void	handle_arg_token(t_cmd *cmd, t_token **tmp_token, t_arg **arg_list_head, t_arg **arg_list_last)
{
	t_arg	*new_arg_node;

	new_arg_node = malloc(sizeof(t_arg));
	if (!new_arg_node)
		return;
	new_arg_node -> value = (*tmp_token) ->value;
	new_arg_node -> is_expandable = (*tmp_token) -> expandable;
	// Wildcard expands only for unquoted tokens
	new_arg_node -> is_wildcard_expandable = !((*tmp_token) -> is_quoted);
	new_arg_node -> was_expanded = 0; // Initially points to token value

	if(*arg_list_head == NULL)//ainda nao existe head, este sera o primeiro arg
	{
		*arg_list_head = new_arg_node;
		*arg_list_last = new_arg_node;
		(*arg_list_last) ->next = NULL;
		cmd -> args = *arg_list_head; //ligo o cmd ao primeiro arg emcontrado
	}
	else
	{
		(*arg_list_last) -> next = new_arg_node;
		(*arg_list_last) = new_arg_node;
		(*arg_list_last) ->next = NULL;
	}
	//atualizo o token a avaliar
	(*tmp_token) = (*tmp_token) -> next;
}

t_cmd	*create_cmd(t_token *tmp_token)
{
	t_cmd	*cmd;
	t_redir	*redir_list_head; //ponderar fazer uma estrutura que engloba o head e o last das listas por ser um padrao recorrente e poupar linhas de codigo
	t_redir	*redir_list_last;
	t_arg	*arg_list_head;
	t_arg	*arg_list_last;

	redir_list_head = NULL;
	redir_list_last = NULL;
	arg_list_head = NULL;
	arg_list_last = NULL;

	cmd = malloc(sizeof(t_cmd));
	cmd -> cmd_name = tmp_token ->value; //o cmd name e sempre o primeiro token, nao passo um token para a frente porque o cmd name tambem e guardado nos args
	cmd -> args = NULL;
	cmd -> redirs = NULL;
	while(tmp_token && tmp_token->is_operator == 0)//aqui vou guardar os args e os redirects; atualizo o token a analizar dentro destas funcoes, se for redirect salto 2 tokens, no caso s ser arg salto 1
	{
		if(is_redirect_token(tmp_token))//se encontrar um token de redirect
			handle_redirect_token(cmd,&tmp_token, &redir_list_head, &redir_list_last);//vou adicionar a redirecao encontrada a lista de redirecoes
		else // e um argumento
			handle_arg_token(cmd,&tmp_token,&arg_list_head, &arg_list_last);//guardo esse argumento na lista de argumentos
	}
	//chegamos ao fim dos tokens ou a um operador, retorno o comando criado
	return cmd;
}

void	create_cmd_node(t_token **tmp_token,t_token *head, t_ast **first_node, t_ast **last_node) // o inicio da node list sera sempre o primeiro comando que recebe, tenho que atualizar este pointer apenas uma vez quando encontro o primeiro comando(primeiro token)
{
	t_cmd *cmd;
	t_ast *new_node;

	cmd = create_cmd(*tmp_token);
	new_node  = create_node(*tmp_token, cmd);
	if(*tmp_token == head) // no caso de ser o primeiro node:
	{
		*first_node = new_node;
		*last_node = new_node;
		(*last_node) -> right = NULL;
	}
	else
	{
		append_node(new_node,*last_node);//ligo estes dois nodes e atualizo last_node
		*last_node = new_node; // agora last_node aponta para o novo node que foi adicionado ao fim da lista
	}
	update_token_to_eval(tmp_token); //atualizar o proximo token a avaliar, (tenho de passar todos os tokens que pertencem a este CMD),
}
