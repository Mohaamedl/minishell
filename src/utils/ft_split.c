/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moh <moh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:00:00 by moh               #+#    #+#             */
/*   Updated: 2025/11/08 10:00:00 by moh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*get_next_word(const char *s, char c, size_t *index)
{
	size_t	start;
	size_t	len;
	char	*word;

	while (s[*index] && s[*index] == c)
		(*index)++;
	start = *index;
	len = 0;
	while (s[*index] && s[*index] != c)
	{
		len++;
		(*index)++;
	}
	word = ft_substr(s, start, len);
	return (word);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	size_t	word_count;
	size_t	i;
	size_t	index;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(s, c, &index);
		if (!result[i])
		{
			free_array(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
