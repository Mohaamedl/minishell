#include "minishell.h"

//1- token WORD encontrada no inicio da list ou logo apos um operador e o nume de um comando, o que vem a seguir sao args ou redirecionamentos
//2- operadores separam comandos

void	append_node(t_ast *new_node,t_ast *last_node)
{
	last_node -> right = new_node;
	new_node -> left = last_node;
	new_node -> right = NULL; //por agora o new_node aponta so para tras (left)
}
t_ast	*create_node(t_token *token, t_cmd* cmd)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if(cmd == NULL) //neste caso vou criar um node operador
	{
		new_node -> cmd = NULL;
		new_node -> type = token ->type ;
	}
	else //estou num comando
	{
		new_node -> cmd = cmd;
		new_node -> type = CMD;
	}
	return (new_node);
}

t_ast	*build_cmds_and_ops_list(t_token *head)
{
	int		in_operator;
	t_token	*tmp_token = head;
	t_ast	*first_node;
	t_ast	*last_node;

	in_operator = 0;
	first_node = NULL;
	last_node = NULL;
	while(tmp_token)
	{
		if(tmp_token -> is_operator == 0) //o token nao e um operador
		{
			create_cmd_node(&tmp_token, head, &first_node, &last_node);//nestas funcoes atualizo o token que vou analizar a seguir (no caso de um comando passo a frente os tokens de args e redirects)
			in_operator = 0;
		}
		else //o token e um operador
		{
			create_op_node(&tmp_token, &last_node);
			in_operator = 1;
		}
	}
	return first_node;
}

