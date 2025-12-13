/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:28:02 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/07 18:28:10 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int is_operator(char *line)
{
	if (!line)
	    return (0);
	if (line[0] == '|' && line[1] == '|')  // "||"
		return (1);
	if (line[0] == '&' && line[1] == '&')  // "&&"
		return (1);
	if (line[0] == '>' && line[1] == '>')  // ">>"
		return (1);
	//agora verifico os operadores de um char
	if (line[0] == ')')
		return (1);
	if (line[0] == '(')
		return (1);
	if (line[0] == '|' || line[0] == '<' || line[0] == '>' || line[0] == ';')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	return(c == '\"' || c == '\'');
}
int is_single_quote(char c)
{
	return(c == '\'');
}
int	is_double_quote(char c)
{
	return(c == '\"');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}
