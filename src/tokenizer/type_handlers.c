/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_handlers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:15 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:28:21 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles quoted text and creates corresponding token
 * @param line Pointer to string starting at a quote character
 * @param i Pointer to the current index in the string
 * @param last_node Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @return 1 on success, 0 on failure
 *
 * Creates a quoted token and advances the index past the entire quoted
 * section including both opening and closing quotes.
 */
int	handle_quote(char *line, int *i, t_token **last_node, t_token **head)
{
	int	status;

	status = create_quoted_token(last_node, head, line, *line);
	*i = *i + get_quoted_size(line, *line) + 2;
	return (status);
}

/**
 * @brief Parses and creates a WORD token from the input string
 * @param line Pointer to string starting at a word character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @return 1 on success, 0 on failure
 *
 * Handles variable assignments (VAR=value) by reading the entire assignment
 * including quoted segments in the value part. For regular words, reads until
 * a delimiter (operator, space, or quote) is encountered. Creates a WORD token
 * with the extracted content and advances the index accordingly.
 */
int	handle_word(char *line, int *i, t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int		is_op;
	int		has_equals;
	char	quote_char;

	is_op = 0;
	has_equals = 0;
	j = 0;
	while (line[j] && line[j] != '=' && !is_operator(&line[j])
		&& !is_space(line[j]) && !is_quote(line[j]))
		j++;
	if (line[j] == '=')
	{
		has_equals = 1;
		j++;
	}
	if (has_equals)
	{
		while (line[j] && !is_operator(&line[j]) && !is_space(line[j]))
		{
			if (is_quote(line[j]))
			{
				quote_char = line[j];
				j++;
				while (line[j] && line[j] != quote_char)
					j++;
				if (line[j] != quote_char)
					return (0);
				j++;
			}
			else
			{
				while (line[j] && !is_quote(line[j]) && !is_space(line[j])
					&& !is_operator(&line[j]))
					j++;
			}
		}
	}
	else
	{
		while (!is_operator(&line[j]) && !is_space(line[j])
			&& line[j] != '\0' && !is_quote(line[j]))
			j++;
	}
	str = malloc((j + 1) * sizeof(char));
	ft_memcpy(str, line, j);
	str[j] = '\0';
	token = create_token(str, WORD, 1, is_op);
	append_token(head, last_token, token);
	*i = *i + j;
	return (1);
}


/**
 * @brief Handles pipe (|) or OR (||) operators
 * @param line Pointer to string starting at a pipe character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an OR token if two consecutive pipes are found,
 * or a PIPE token for a single pipe. Advances the index by 2 for OR
 * or 1 for PIPE.
 */
void	handle_pipe_or_or(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(++line) == '|')
	{
		str = ft_strdup("||");
		token = create_token(str, OR, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("|");
		token = create_token(str, PIPE, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles AND (&&) operator
 * @param line Pointer to string starting at an ampersand character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates an AND token when two consecutive ampersands are found.
 * Advances the index by 2.
 */
void	handle_and(char *line, int *i, t_token **last_token, t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(++line) == '&')
	{
		str = ft_strdup("&&");
		token = create_token(str, AND, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
}

/**
 * @brief Handles input redirection (<) or heredoc (<<) operators
 * @param line Pointer to string starting at a less-than character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either a HEREDOC token if two consecutive less-than signs are found,
 * or a REDIR_IN token for a single less-than sign. Advances the index
 * by 2 for HEREDOC or 1 for REDIR_IN.
 */
void	handle_redin_or_heredoc(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 0;
	if (*(++line) == '<')
	{
		str = ft_strdup("<<");
		token = create_token(str, HEREDOC, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup("<");
		token = create_token(str, REDIR_IN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles parentheses tokens
 * @param line Pointer to string starting at a parenthesis character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an LPAREN token for '(' or an RPAREN token for ')'.
 * Advances the index by 1.
 */
void	handle_parentesis(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 1;
	if (*(line) == '(')
	{
		str = ft_strdup("(");
		token = create_token(str, LPAREN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
	if (*(line) == ')')
	{
		str = ft_strdup(")");
		token = create_token(str, RPAREN, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles output redirection (>) or append (>>) operators
 * @param line Pointer to string starting at a greater-than character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates either an APPEND token if two consecutive greater-than signs
 * are found, or a REDIR_OUT token for a single greater-than sign.
 * Advances the index by 2 for APPEND or 1 for REDIR_OUT.
 */
void	handle_redap_or_redout(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	is_op = 0;
	if (*(++line) == '>')
	{
		str = ft_strdup(">>");
		token = create_token(str, APPEND, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 2;
	}
	else
	{
		str = ft_strdup(">");
		token = create_token(str, REDIR_OUT, 1, is_op);
		append_token(head, last_token, token);
		*i = *i + 1;
	}
}

/**
 * @brief Handles semicolon (;) operator
 * @param line Pointer to string starting at a semicolon character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Creates a SEMICOLON token. Advances the index by 1.
 * Note: The line parameter is unused in this function.
 */
void	handle_semicolon(char *line, int *i, t_token **last_token,
		t_token **head)
{
	t_token	*token;
	char	*str;
	int		is_op;

	(void)line;
	is_op = 1;
	str = ft_strdup(";");
	token = create_token(str, SEMICOLON, 1, is_op);
	append_token(head, last_token, token);
	*i = *i + 1;
}