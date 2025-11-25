#include "minishell.h"

void syntax_error(t_token *tok)
{
	if (tok == NULL)
		printf("bash: syntax error near unexpected token `newline'\n");
	else
		printf("bash: syntax error near unexpected token `%s'\n", tok->value);
}

int validate_token_list_recursive(t_token **head)
{
	if(parse_and_or(head) == 0)//esta funcao verifica se tem pipelines(comandos entre pipes) validos antes e depois de todos os operadores OR ou AND
		return 0;
	else
		return 1;
}

int parse_and_or(t_token **head)
{
	if(parse_pipeline(head) == 0) //verifica se tem uma pipeline valida
		return 0;
	while (*head != NULL &&((*head)->type == AND || (*head)->type == OR))// se nao chegar ao fim e encontrar um operador AND ou OR
	{
		*head = (*head)->next; // consome operador
		if (*head == NULL) //nao tem nada apos um operador
		{
			syntax_error(NULL);
			return 0;
		}//caso tenha algo a frente de um operador foi ver se e uma pipeline valida
		if(parse_pipeline(head) == 0) //verifica se tem uma pipeline valida
			return 0;
	}
	return 1;
}

int parse_pipeline(t_token **head)
{
	if(parse_cmd(head) == 0) //verifica se tem cmd valido
		return 0;
	while (*head != NULL && (*head)->type == PIPE)
	{
		*head = (*head)->next; // consome operador
		if (*head == NULL) //nao tem nada apos um pipe
		{
			syntax_error(NULL);
			return 0;
		}
		if(parse_cmd(head) == 0) //verifica se tem cmd valido
			return 0;
	}
	return 1;
}

int parse_cmd(t_token **head)
{
	// SUBSHELL
	if ((*head)->type == LPAREN)
	{
		if(parse_subshell(head) == 0)
			return 0;
	}
	if((*head)->type != WORD)		//um comando valido comeca com um token WORD
	{
		syntax_error(*head);
		return 0;
	}
	while (*head != NULL && ((*head)->type == WORD || is_redirect_token(*head)))
	{
		if ((*head)->type == WORD)
			*head = (*head)->next; // consome operador
		else
		{
			*head = (*head)->next; // consome operador
			if (*head == NULL)
			{
				syntax_error(NULL);
				return 0;
			}
			if ((*head)->type != WORD)
			{
				syntax_error(*head);
				return 0;
			}
			*head = (*head)->next; // consome operador
		}
	}
	if(*head != NULL)
		return
	return 1;
}

int parse_subshell(t_token **head)
{
	*head = (*head)->next; // consome operador
	if (*head == NULL)//nada dentro dos parentesis
	{
		syntax_error(NULL);
		return 0;
	}
	if (!validate_token_list_recursive(head))
		return 0;
	if (*head == NULL || (*head)->type != RPAREN)//se no fim de tudo nao chegar ao RPAREN a subshell nao fechou
	{
		syntax_error(*head);
		return 0;
	}
	*head = (*head)->next; // consome operador RPAREN
	return 1;
}



