/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:47:39 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:05:21 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			total;
	unsigned char	*ptr;
	size_t			i;

	if (size != 0 && nmemb > __SIZE_MAX__ / size)
		return (NULL);
	total = nmemb * size;
	ptr = (unsigned char *)malloc(total);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < total)
		ptr[i++] = 0;
	return ((void *)ptr);
}
