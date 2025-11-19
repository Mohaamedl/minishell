/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:55:24 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:05:56 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_cpy;
	unsigned char	*src_cpy;

	dst_cpy = (unsigned char *) dst;
	src_cpy = (unsigned char *) src;
	if (dst > src)
	{
		while (len--)
			*(dst_cpy + len) = *(src_cpy + len);
	}
	else if (dst_cpy != src_cpy)
	{
		while (len--)
			*(dst_cpy++) = *(src_cpy++);
	}
	return (dst);
}
