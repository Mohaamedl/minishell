/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_var_expand_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 11:51:54 by framiran          #+#    #+#             */
/*   Updated: 2026/01/12 12:19:34 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calculate the length of a variable expression
 *
 * Determines the total length of a variable expression including the '$'
 * character. For the special variable '?' it returns 2, otherwise returns
 * the length of the variable name plus 1.
 *
 * @param var_name The variable name (without the '$' prefix)
 * @return The total length of the variable expression (including '$')
 */
size_t	get_var_len(char *var_name)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (2);
	return (ft_strlen(var_name) + 1);
}

/**
 * @brief Join two strings and free both inputs
 *
 * Allocates a new string containing the concatenation of s1 and s2.
 * Frees both input strings after joining.
 *
 * @param s1 First string (must be dynamically allocated)
 * @param s2 Second string (must be dynamically allocated)
 * @return Newly allocated joined string, or NULL on allocation failure
 *
 * @note Both s1 and s2 are freed regardless of success.
 */
char	*strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

/**
 * @brief Build expanded string replacing a single variable
 *
 * Constructs a new string by concatenating:
 *  - the part before the '$'
 *  - the expanded variable value
 *  - the part after the variable
 *
 * @param str Original string containing the variable
 * @param value Expanded variable value
 * @param dollar Pointer to the '$' character in str
 * @param len Length of the variable expression to replace (e.g. $VAR or $?)
 * @return Newly allocated expanded string
 *
 * @note Does not free the original string.
 */
char	*build_expanded(char *str, char *value,
							char *dollar, size_t len)
{
	char	*res;

	res = ft_substr(str, 0, dollar - str);
	res = strjoin_free(res, ft_strdup(value));
	res = strjoin_free(res, ft_strdup(dollar + len));
	return (res);
}
