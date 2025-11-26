#include "minishell.h"

int validate_redirections(t_token *head)
{
	t_token *tmp;
	tmp = head;
	while(tmp)
	{
		if(is_redirect_token(tmp))//se for um redirect token tem que ter um file name associado no caso do heredoc tem de ter um Terminador
		{
			if(tmp->next==NULL)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
			if(tmp->next->type != WORD)
			{
				printf("syntax error near unexpected token `%s'\n",tmp -> value);
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int is_connector_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == PIPE || token->type == OR || token->type == AND);
}

int validate_right_and_left_tokens(t_token *left, t_token *right, t_token *op_token)
{
	if(left == NULL || right == NULL)
	{
		printf("syntax error near unexpected token `%s'\n",op_token->value);
		return 0;
	}
	if(left->type != WORD && left->type != RPAREN)
	{
		printf("syntax error near unexpected token `%s'\n",op_token->value);
		return 0;
	}
	if(right->type != WORD && right->type != LPAREN &&!is_redirect_token(right))
	{
		printf("syntax error near unexpected token `%s'\n",op_token->value);
		return 0;
	}
	return 1;
}

t_token *find_left_token(t_token *head,t_token *op_token)
{
	t_token *tmp;
	tmp = head;
	while(tmp)
	{
		if(tmp ->next == op_token)
			return (tmp);
		tmp = tmp -> next;
	}
	return (NULL);
}

int validate_connector_operators(t_token *head)
{
	t_token *tmp;
	tmp = head;
	t_token *token_before_op;
	while(tmp)
	{
		if(is_connector_operator(tmp))//se for um redirect token tem que ter um file name associado no caso do heredoc tem de ter um Terminador
		{
			token_before_op = find_left_token(head,tmp);//retorna NULL se nao encontrar
			if(validate_right_and_left_tokens(token_before_op,tmp->next, tmp) == 0)
				return 0;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_balance(t_token *head)
{
	t_token *tmp;
	int	open;

	open = 0;
	tmp = head;
	while(tmp)
	{
		if (tmp->type == LPAREN)
			open++;
		if (tmp ->type == RPAREN)
			open--;
		if (open < 0)
		{
			printf("syntax error near unexpected token `%s'\n", tmp ->value);
			return 0;
		}

		tmp = tmp->next;
	}
	if(open == 0)
		return 1;
	else
	{
		printf("syntax error near unexpected token `('\n");
		return 0;
	}

}

int check_valid_content(t_token *head)
{
	t_token *tmp;
	t_token *token_before_RPAREN;

	tmp = head;
	while(tmp)
	{
		if (tmp->type == LPAREN)
		{
			if(tmp->next != NULL)
			{
				if(tmp->next->type != WORD && tmp->next->type != LPAREN)
				{
					printf("syntax error near unexpected token `%s'\n",tmp -> value);
					return 0;
				}

			}
			else
			{
				printf("syntax error near unexpected token `newline'\n");
				return 0;
			}
		}
		if (tmp ->type == RPAREN)
		{
			token_before_RPAREN = find_left_token(head,tmp);
			if(token_before_RPAREN != NULL)
			{
				if(token_before_RPAREN->type != WORD  && token_before_RPAREN->type != RPAREN)
				{
					printf("syntax error near unexpected token `%s'\n",tmp -> value);
					return 0;
				}
			}
			else
			{
				printf("syntax error near unexpected token `newline'\n");
				return 0;
			}
		}
		tmp = tmp->next;
	}
	return 1;
}

int validate_parentesis(t_token *head)
{
	if(check_balance(head)==0) //retorna 0 se os parentesis nao estiverem balanciados
		return (0);
	if(check_valid_content(head) == 0)
		return 0;
	return 1;
}

int validate_token_list(t_token *head)
{
	if(validate_redirections(head) == 0)//valida se as redirecoes tem sempre um file name a frente
		return 0;
	if(validate_connector_operators(head) == 0)//valida os operadores AND, OR ou PIPE
		return 0;
	if(validate_parentesis(head) == 0)//valida parentesis
		return 0;
	return 1;
}

