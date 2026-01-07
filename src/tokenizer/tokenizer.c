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

/**
 * @brief Calculates the size of content within quotes
 * @param line Pointer to the string starting at a quote character
 * @param quote The quote character to match (single or double quote)
 * @return Size of content between quotes, or -1 if quote is not closed
 *
 * Skips the opening quote and counts characters until the closing quote.
 * Returns -1 if the string ends before finding the closing quote.
 */
int	get_quoted_size(char *line, char quote)
{
	int	size;

	size = 0;
	line++;
	while (*line != quote && *line != '\0')
	{
		size++;
		line++;
	}
	if (*line == '\0')
		return (-1);
	return (size);
}

/**
 * @brief Creates a new token with the given parameters
 * @param value String value of the token (must be dynamically allocated)
 * @param type Type of the token (WORD, PIPE, REDIR_IN, etc.)
 * @param is_expandable 1 if token can have variable expansion, 0 otherwise
 * @param is_op 1 if token is an operator, 0 otherwise
 * @return Pointer to the newly created token, or NULL on allocation failure
 *
 * Allocates memory for a new token and initializes all fields.
 * The is_quoted field is set to 0 by default and next is set to NULL.
 */
t_token	*create_token(char *value, t_token_type type, int is_expandable,
		int is_op)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->is_operator = is_op;
	token->type = type;
	token->value = value;
	token->expandable = is_expandable;
	token->is_quoted = 0;
	token->next = NULL;
	return (token);
}

/**
 * @brief Extracts text content from within quotes
 * @param line Pointer to string starting at a quote character
 * @param quote The quote character to match
 * @return Dynamically allocated string with content between quotes, or NULL
 *
 * Allocates memory and copies the content between the opening and closing
 * quotes. Returns NULL if the quote is not closed properly.
 */
char	*get_quoted_text(char *line, char quote)
{
	int		size;
	char	*str;

	size = get_quoted_size(line, quote);
	if (size == -1)
		return (NULL);
	else
	{
		str = malloc((size + 1) * sizeof(char));
		ft_memcpy(str, ++line, size);
		str[size] = '\0';
		return (str);
	}
}

/**
 * @brief Appends a token to the end of the token list
 * @param head Pointer to pointer of the first token in the list
 * @param last_token Pointer to pointer of the last token in the list
 * @param new_token Token to append to the list
 *
 * If the list is empty, sets head to point to new_token. Otherwise, links
 * the new token to the end of the list. Updates last_token to point to
 * the newly added token. Does nothing if new_token is NULL.
 */
void	append_token(t_token **head, t_token **last_token, t_token *new_token)
{
	if (!new_token)
		return ;
	if (*head == NULL)
		*head = new_token;
	else
		(*last_token)->next = new_token;
	*last_token = new_token;
}

/**
 * @brief Creates a token from quoted text and adds it to the token list
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 * @param line Pointer to string starting at a quote character
 * @param quote The quote character (single or double)
 * @return 1 on success, 0 on failure (unclosed quotes)
 *
 * Creates a WORD token from the quoted text. Single quotes disable variable
 * expansion. Double quotes allow expansion except for heredoc delimiters.
 * Sets is_quoted flag to prevent wildcard expansion.
 */
int	create_quoted_token(t_token **last_token, t_token **head, char *line,
		char quote)
{
	char	*str;
	t_token	*token;
	int		is_expandable;
	int		is_op;

	is_op = 0;
	is_expandable = 1;
	if (is_single_quote(*line))
		is_expandable = 0;
	if (last_token && *last_token)
	{
		if ((*last_token)->type == HEREDOC)
			is_expandable = 0;
	}
	str = get_quoted_text(line, quote);
	if (str == NULL)
		return (0);
	else
	{
		token = create_token(str, WORD, is_expandable, is_op);
		token->is_quoted = 1;
		append_token(head, last_token, token);
	}
	return (1);
}

/**
 * @brief Skips whitespace characters in a string
 * @param line Pointer to the string to process
 * @param i Pointer to the current index in the string
 *
 * Advances the index past all consecutive whitespace characters
 * (space, tab, vertical tab, form feed, carriage return).
 */
void	skip_spaces(char *line, int *i)
{
	int	j;

	j = 0;
	while (is_space(line[j]))
		j++;
	*i = *i + j;
}

/**
 * @brief Routes operator/redirection handling to appropriate handler
 * @param line Pointer to string starting at an operator character
 * @param i Pointer to the current index in the string
 * @param last_token Pointer to pointer of the last token in the list
 * @param head Pointer to pointer of the first token in the list
 *
 * Dispatches to specific handler functions based on the operator type:
 * pipe/OR (|), AND (&&), input redirection/heredoc (<),
 * output redirection/append (>), parentheses, or semicolon.
 */
void	handle_ops_and_reds(char *line, int *i, t_token **last_token,
		t_token **head)
{
	if (*line == '|')
		handle_pipe_or_or(line, i, last_token, head);
	else if (*line == '&')
		handle_and(line, i, last_token, head);
	else if (*line == '<')
		handle_redin_or_heredoc(line, i, last_token, head);
	else if (*line == '>')
		handle_redap_or_redout(line, i, last_token, head);
	else if (*line == '(' || *line == ')')
		handle_parentesis(line, i, last_token, head);
	else if (*line == ';')
		handle_semicolon(line, i, last_token, head);
}

/**
 * @brief Tokenizes a command line string into a linked list of tokens
 * @param line The command line string to tokenize
 * @return Pointer to the first token, or NULL on error
 *
 * Parses the input string and creates tokens for words, operators,
 * redirections, and quoted strings. Handles spaces, quotes, and operators
 * according to shell tokenization rules. Returns NULL if unclosed quotes
 * are encountered or memory allocation fails.
 */
t_token	*tokenize(char *line)
{
	int		i;
	t_token	*last_token;
	t_token	*head;

	last_token = NULL;
	head = NULL;
	i = 0;
	while (line[i] != 0)
	{
		if (is_space(line[i]))
			skip_spaces(&line[i], &i);
		else if (is_quote(line[i]))
		{
			if (handle_quote(&line[i], &i, &last_token, &head) == 0)
			{
				free_tokens(head);
				return (NULL);
			}
		}
		else if (is_operator(&line[i]))
			handle_ops_and_reds(&line[i], &i, &last_token, &head);
		else
		{
			if (handle_word(&line[i], &i, &last_token, &head) == 0)
			{
				free_tokens(head);
				return (NULL);
			}
		}
	}
	return (head);
}
