/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:32 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:34 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validates that all redirection tokens have a filename following them
 * @param head Pointer to the first token in the list
 * @return 1 if all redirections are valid, 0 otherwise
 *
 * Checks each redirection token (<, >, <<, >>) to ensure it is followed by
 * a WORD token that represents the filename or heredoc delimiter. Prints
 * syntax error messages for invalid redirections.
 */
int	validate_redirections(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		if (is_redirect_token(tmp))
		{
			if (tmp->next == NULL)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
			if (tmp->next->type != WORD)
			{
				printf("syntax error near unexpected token `%s'\n",
					tmp->value);
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

/**
 * @brief Validates tokens on left and right sides of a connector operator
 * @param left Pointer to the token before the operator
 * @param right Pointer to the token after the operator
 * @param op_token Pointer to the operator token
 * @return 1 if both sides are valid, 0 otherwise
 *
 * Ensures connector operators (|, ||, &&) have valid tokens on both sides.
 * Left side must be WORD or RPAREN. Right side must be WORD, LPAREN, or
 * a redirection token. Prints syntax error for invalid configurations.
 */
int	validate_right_and_left_tokens(t_token *left, t_token *right,
		t_token *op_token)
{
	if (left == NULL || right == NULL)
	{
		printf("syntax error near unexpected token `%s'\n", op_token->value);
		return (0);
	}
	if (left->type != WORD && left->type != RPAREN)
	{
		printf("syntax error near unexpected token `%s'\n", op_token->value);
		return (0);
	}
	if (right->type != WORD && right->type != LPAREN
		&& !is_redirect_token(right))
	{
		printf("syntax error near unexpected token `%s'\n", op_token->value);
		return (0);
	}
	return (1);
}

/**
 * @brief Validates all connector operators in the token list
 * @param head Pointer to the first token in the list
 * @return 1 if all connector operators are valid, 0 otherwise
 *
 * Iterates through the token list and validates that each connector operator
 * (PIPE, OR, AND) has valid tokens on both left and right sides.
 */
int	validate_connector_operators(t_token *head)
{
	t_token	*tmp;
	t_token	*token_before_op;

	tmp = head;
	while (tmp)
	{
		if (is_connector_operator(tmp))
		{
			token_before_op = find_left_token(head, tmp);
			if (validate_right_and_left_tokens(token_before_op, tmp->next,
					tmp) == 0)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

/**
 * @brief Validates all parentheses in the token list
 * @param head Pointer to the first token in the list
 * @return 1 if parentheses are valid, 0 otherwise
 *
 * Performs complete validation of parentheses including balance check
 * and content validation.
 */
int	validate_parentesis(t_token *head)
{
	if (check_balance(head) == 0)
		return (0);
	if (check_valid_content(head) == 0)
		return (0);
	return (1);
}

/**
 * @brief Validates the entire token list for syntax errors
 * @param head Pointer to the first token in the list
 * @return 1 if token list is valid, 0 if errors are found
 *
 * Performs comprehensive validation of the token list including checks for
 * unsupported tokens (semicolon), proper redirection syntax, valid connector
 * operators, and balanced parentheses. Prints specific error messages for
 * each type of syntax error encountered.
 */
int	validate_token_list(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == SEMICOLON)
		{
			printf("syntax error near unexpected token `%s'\n", tmp->value);
			return (0);
		}
		tmp = tmp->next;
	}
	if (validate_redirections(head) == 0)
		return (0);
	if (validate_connector_operators(head) == 0)
		return (0);
	if (validate_parentesis(head) == 0)
		return (0);
	return (1);
}
