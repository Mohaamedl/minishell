/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:47:15 by framiran          #+#    #+#             */
/*   Updated: 2025/10/02 16:23:50 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

char	**free_array(char **array, int last_index)
{
	while (last_index-- > 0)
		free(array[last_index]);
	free(array);
	return (NULL);
}

char	**fill_array(char **array, char const *s, char c)
{
	size_t		i;
	size_t		j;
	size_t		start;

	i = 0;
	start = 0;
	j = 0;
	while (j <= ft_strlen(s))
	{
		if (s[j] == c || s[j] == '\0')
		{
			if (j > start)
			{
				array[i] = malloc(j - start + 1);
				if (!array[i])
					return (free_array(array, i));
				ft_strlcpy(array[i++], &s[start], j - start + 1);
			}
			start = j + 1;
		}
		j++;
	}
	array[i] = NULL;
	return (array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;

	if (!s)
		return (NULL);
	array = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	return (fill_array(array, s, c));
}
