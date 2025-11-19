/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:45:20 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:06:17 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int	h;
	int	n;

	if (!*needle)
		return ((char *)haystack);
	if (!*haystack || len == 0)
		return (NULL);
	h = 0;
	n = 0;
	while ((size_t) h <= len -1)
	{
		while (needle[n] == haystack[h + n] && (size_t)(h + n) <= len - 1)
		{
			n++;
			if (needle[n] == '\0')
				return ((char *) &haystack[h]);
		}
		n = 0;
		h++;
	}
	return (NULL);
}
