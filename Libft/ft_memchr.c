/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:55:16 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:05:52 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_cpy;

	s_cpy = (unsigned char *) s;
	while (n--)
	{
		if (*s_cpy == (unsigned char)c)
			return ((void *) s);
		s++;
		s_cpy++;
	}
	return (NULL);
}
