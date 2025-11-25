#include "minishell.h"

int	handle_quote(char *line, int *i,t_token **last_node, t_token **head)
{
	int status;

	status = create_quoted_token(last_node,head,line,*line);
	*i = *i + get_quoted_size(line,*line) + 2; //passar a frente o tento dentro das quotes e das prprias quotes
	return status; //0-failed 1-sucess
}
void	handle_word(char *line, int *i,t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int is_op;

	is_op = 0;

	j = 1; //vou para o char seguinte apos a letra que encontrei
	while(!is_operator(&line[j]) && !is_space(line[j]) && line[j] != '\0' && !is_quote(line[j])) //validar a mudanca no is_operator, ver o que fazer se receber uma quote
	{
		j++;
	}
	//neste ponto line[j] e' um operador, espaco ou '\0'
	// com j tava deslocado, tamanhao a copiar da str e' j
	str = malloc((j + 1)*sizeof(char));
	ft_memcpy(str, line, j); //ando line um char para a frente para nao copiar a aspa inicial
	str[j] = '\0';
	token = create_token(str, WORD, 1, is_op);
	append_token(head,last_token,token);
	//atualizar i (posicao na line)
	*i = *i + j;
}

void	handle_pipe_or_or(char *line, int *i, t_token **last_token, t_token **head) //em todas estas funcoes de handle, seja o que receber aqui sei que e' um operador valido basta me saber qual
{
	t_token *token;
	char *str;
	int is_op;

	is_op = 1;
	if(*(++line) == '|')
	{
		str = ft_strdup("||");
		token = create_token(str, OR, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("|");
		token = create_token(str, PIPE, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 1;
	}
}
void	handle_and(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token *token;
	char *str;
	int is_op;

	is_op = 1;

	if(*(++line) == '&') //esta verificacao pode ser redundante, confirmar
	{
		str = ft_strdup("&&"); //faco strdup porque ate agora todos os values dos tokens foram dinamicamente alocadas
		token = create_token(str, AND, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 2;
	}
}
void	handle_redin_or_heredoc(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token *token;
	char *str;

	int is_op;

	is_op = 0;
	if (*(++line) == '<')
	{
		str = ft_strdup("<<"); //faco strdup porque ate agora todos os values dos tokens foram dinamicamente alocadas
		token = create_token(str, HEREDOC, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("<");
		token = create_token(str, REDIR_IN, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 1;
	}
}
void	handle_parentesis(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token *token;
	char *str;
	int is_op;

	is_op = 1;

	if(*(line) == '(')
	{
		str = ft_strdup("("); //faco strdup porque ate agora todos os values dos tokens foram dinamicamente alocadas
		token = create_token(str, LPAREN, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 1;
	}
	if(*(line) == ')')
	{
		str = ft_strdup(")"); //faco strdup porque ate agora todos os values dos tokens foram dinamicamente alocadas
		token = create_token(str, RPAREN, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 1;
	}
}
void	handle_redap_or_redout(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token *token;
	char *str;
	int is_op;

	is_op = 0;

	if(*(++line) == '>')
	{
		str = ft_strdup(">>");
		token = create_token(str, APPEND, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup(">");
		token = create_token(str, REDIR_OUT, 1, is_op);
		append_token(head,last_token,token);
		*i = *i + 1;
	}
}
