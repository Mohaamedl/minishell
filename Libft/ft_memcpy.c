/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:55:21 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:05:55 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dst_cpy;
	unsigned char	*src_cpy;

	dst_cpy = (unsigned char *) dst;
	src_cpy = (unsigned char *) src;
	if (dst == (void *) 0 && src == (void *) 0)
		return (dst);
	while (n--)
		*(dst_cpy++) = *(src_cpy++);
	return (dst);
}
