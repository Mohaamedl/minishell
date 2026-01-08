/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:27:44 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:31 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_token	*last;
	t_token	*head;

	i = 0;
	last = NULL;
	head = NULL;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			skip_spaces(&line[i], &i);
			continue ;
		}
		if (is_quote(line[i]) && !handle_quote(&line[i], &i, &last, &head))
			return (free_tokens(head), NULL);
		if (is_operator(&line[i]))
		{
			handle_ops_and_reds(&line[i], &i, &last, &head);
			continue ;
		}
		if (!handle_word(&line[i], &i, &last, &head))
			return (free_tokens(head), NULL);
	}
	return (head);
}
