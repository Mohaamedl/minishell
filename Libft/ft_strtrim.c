/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:34:45 by framiran          #+#    #+#             */
/*   Updated: 2025/08/08 12:29:49 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	set_contains(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(set))
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start_indice;
	size_t	end_indice;
	size_t	size;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	start_indice = 0;
	end_indice = ft_strlen(s1) - 1;
	while (set_contains(s1[start_indice], set))
		start_indice++;
	if (start_indice > end_indice)
		return (ft_strdup(""));
	while (set_contains(s1[end_indice], set))
		end_indice--;
	size = end_indice - start_indice + 1;
	str = malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[start_indice], size + 1);
	return (str);
}
