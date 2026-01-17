/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token_list_helpers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 14:58:32 by framiran          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:33 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a token is a connector operator
 * @param token Pointer to the token to check
 * @return 1 if token is PIPE, OR, or AND, 0 otherwise
 */
int	is_connector_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == PIPE || token->type == OR || token->type == AND);
}

/**
 * @brief Finds the token immediately before the given operator token
 * @param head Pointer to the first token in the list
 * @param op_token Pointer to the operator token
 * @return Pointer to the token before op_token, or NULL if not found
 */
t_token	*find_left_token(t_token *head, t_token *op_token)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->next == op_token)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

/**
 * @brief Checks if parentheses are balanced in the token list
 * @param head Pointer to the first token in the list
 * @return 1 if parentheses are balanced, 0 otherwise
 *
 * Counts opening and closing parentheses to ensure they are properly matched.
 * Detects mismatched or unbalanced parentheses and prints appropriate error
 * messages.
 */
int	check_balance(t_token *head)
{
	t_token	*tmp;
	int		open;

	open = 0;
	tmp = head;
	while (tmp)
	{
		if (tmp->type == LPAREN)
			open++;
		if (tmp->type == RPAREN)
			open--;
		if (open < 0)
		{
			printf("syntax error near unexpected token `%s'\n", tmp->value);
			return (0);
		}
		tmp = tmp->next;
	}
	if (open == 0)
		return (1);
	else
	{
		printf("syntax error near unexpected token `('\n");
		return (0);
	}
}

/**
 * @brief Prints a syntax error message for an unexpected token.
 *
 * This function displays a shell-like syntax error message. If a token
 * is provided, it prints the token that caused the error. Otherwise,
 * it reports an unexpected newline.
 *
 * @param token The unexpected token value, or NULL if the error is
 *              caused by an unexpected newline.
 *
 * @return Always returns 0 to indicate a syntax error.
 */
static int	syntax_error(char *token)
{
	if (token)
		printf("syntax error near unexpected token `%s'\n", token);
	else
		printf("syntax error near unexpected token `newline'\n");
	return (0);
}

/**
 * @brief Validates content inside and around parentheses
 * @param head Pointer to the first token in the list
 * @return 1 if parentheses content is valid, 0 otherwise
 *
 * Checks that opening parentheses are followed by valid tokens (WORD or LPAREN)
 * and closing parentheses are preceded by valid tokens (WORD or RPAREN).
 * Ensures parentheses contain valid command structures.
 */
int	check_valid_content(t_token *head)
{
	t_token	*tmp;
	t_token	*token_before_rparen;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == LPAREN)
		{
			if (!tmp->next)
				return (syntax_error(NULL));
			if (tmp->next->type != WORD && tmp->next->type != LPAREN)
				return (syntax_error(tmp->value));
		}
		else if (tmp->type == RPAREN)
		{
			token_before_rparen = find_left_token(head, tmp);
			if (!token_before_rparen)
				return (syntax_error(NULL));
			if (token_before_rparen->type != WORD
				&& token_before_rparen->type != RPAREN)
				return (syntax_error(tmp->value));
		}
		tmp = tmp->next;
	}
	return (1);
}
