/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:27:44 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:27:55 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_quoted_size(char *line, char quote)
{
	int size;

	size = 0;
	line++; //para passar a prmeira aspa a frente (so quero o conteudo)
	while(*line != quote && *line != '\0')
	{
		size++;
		line++;
	}
	if(*line == '\0') //chegou ao fim se fechar
		return (-1);
	return(size);
}
t_token *create_token(char* value, t_token_type type, int is_expandable, int is_op)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->is_operator = is_op;
	token->type = type;
	token->value = value;
	token->expandable = is_expandable;
	token->is_quoted = 0; // Default: not quoted
	token->next = NULL;
	return token;
}

char *get_quoted_text(char *line,char quote)
{
	int size;
	char *str;
	size = get_quoted_size(line,quote);//este size e´ -1 se a quote nao fechar
	if (size == -1)
		return (NULL);
	else
	{
		str = malloc((size + 1)*sizeof(char));
		ft_memcpy(str, ++line, size); //ando line um char para a frente para nao copiar a aspa inicial
		str[size] = '\0';
		return str;
	}
}

void	append_token(t_token **head, t_token **last_token, t_token *new_token)
{
	if (!new_token)
		return;

	if (*head == NULL) //sefor o primeiro node a ser criado, head aponta para ele
		*head = new_token;
	else
		(*last_token)->next = new_token;
	*last_token = new_token;
}
//Aspas simples '...' e duplas "..." formam um único token mesmo com espaços ou operadores,
//mas enquanto "..." permite expansão de variáveis e escapes, '...' é totalmente literal.
int	create_quoted_token(t_token **last_token, t_token **head, char *line, char quote)
{
	char	*str;
	t_token	*token;
	int is_expandable;
	int is_op;
	is_op = 0;

	is_expandable = 1;
	if(is_single_quote(*line))
		is_expandable = 0;
	if (last_token) //Caso especifico do HEREDOC, se o end of file estiver em aspas (single or double quote) o conteudo nao espande
	{
		if ((*last_token) -> type == HEREDOC)
			is_expandable = 0;
	}
	str = get_quoted_text(line,quote);
	if (str == NULL) //as aspas nao fecharam
		return 0;//failed
	else
	{
		token = create_token(str, WORD, is_expandable, is_op);
		token->is_quoted = 1; // Mark as quoted (no wildcard expansion)
		append_token(head,last_token,token);
	}
	return 1;//sucess
}
// Not interpret unclosed quotes or special characters which are not required by the
//subject such as \ (backslash) or ; (semicolon)

void	skip_spaces(char *line,int *i)
{
	int j;
	j = 0;
	while(is_space(line[j]))
		j++;
	*i = *i + j;
}


void handle_ops_and_reds(char *line, int *i, t_token **last_token, t_token **head)
{
	if (*line == '|')
		handle_pipe_or_or(line,i,last_token,head);  // função que vai criar token PIPE ou OR (||)
	else if (*line == '&')
		handle_and(line, i,last_token,head);         // função que cria token AND (&&)
	else if (*line == '<')
		handle_redin_or_heredoc(line,i,last_token,head);       // função que cria token REDIR_INPUT (<) ou ou HEREDOC (<<)
	else if (*line == '>')
		handle_redap_or_redout(line,i,last_token,head); // função que cria token REDIR_OUTPUT (>) ou REDIR_APPEND (>>)
	else if (*line == '(' || *line == ')')
		handle_parentesis(line,i,last_token,head);
	else if (*line == ';')
		handle_semicolon(line,i,last_token,head);
}

//por agora o trata espacos e aspas;
t_token	*tokenize(char* line)
{
	int	i;
	t_token *last_token;
	t_token *head;

	last_token = NULL;
	head = NULL;
 	i = 0;
	//convem manter esta hierarquia de operacoes exemplo  grep &ola ,quero tratar &ola como palavra e nao separar em tokens
	while(line[i] != 0)
	{
		if(is_space(line[i]))
			skip_spaces(&line[i], &i);
		else if(is_quote(line[i]))
		{
			if(handle_quote(&line[i],&i, &last_token, &head)==0)
			{
				free_tokens(head);//dou free aos tokens que posso eventualmente ter criado ate aqui
				return (NULL);//erro ao tokenizar
			}
		}
		else if(is_operator(&line[i]))
			handle_ops_and_reds(&line[i],&i, &last_token, &head);
		else
			handle_word(&line[i],&i, &last_token, &head);
	}
	return (head);
}
