/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:32 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:28:37 by mhaddadi         ###   ########.fr       */
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
	t_token	*token_before_RPAREN;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == LPAREN)
		{
			if (tmp->next != NULL)
			{
				if (tmp->next->type != WORD && tmp->next->type != LPAREN)
				{
					printf("syntax error near unexpected token `%s'\n",
						tmp->value);
					return (0);
				}
			}
			else
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
		}
		if (tmp->type == RPAREN)
		{
			token_before_RPAREN = find_left_token(head, tmp);
			if (token_before_RPAREN != NULL)
			{
				if (token_before_RPAREN->type != WORD
					&& token_before_RPAREN->type != RPAREN)
				{
					printf("syntax error near unexpected token `%s'\n",
						tmp->value);
					return (0);
				}
			}
			else
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
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