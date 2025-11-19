/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:55:32 by framiran          #+#    #+#             */
/*   Updated: 2025/08/08 14:52:50 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	number_of_letters_dst;
	size_t	number_of_letters_src;
	size_t	start_indice;
	int		remaining_size;

	if (size <= ft_strlen(dst))
		return (size + ft_strlen(src));
	number_of_letters_dst = ft_strlen(dst);
	number_of_letters_src = ft_strlen(src);
	remaining_size = size - number_of_letters_dst - 1;
	if (remaining_size < 0)
		return (size + number_of_letters_src);
	start_indice = number_of_letters_dst;
	while (remaining_size-- && *src)
	{
		dst[start_indice] = *src++;
		start_indice++;
	}
	dst[start_indice] = '\0';
	return (number_of_letters_dst + number_of_letters_src);
}
/*#include <string.h>
int main()
{
	char dest[30];
	memset(dest, 0, 30);
	char *src = (char *)"AAAAAAAAA";
	memset(dest, 'C', 5); // dest = "CCCCCA\0..."
	ft_strlcat(dest, src, -1);
}*/
