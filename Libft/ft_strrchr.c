/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:45:13 by framiran          #+#    #+#             */
/*   Updated: 2025/04/29 19:06:19 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*c_p;
	int				i;
	unsigned char	uc;

	uc = (unsigned char) c;
	c_p = NULL;
	i = 0;
	while (s[i] != '\0')
	{
		if (uc == (unsigned char)s[i])
			c_p = (char *) &s[i];
		i++;
	}
	if (uc == 0)
		return ((char *) &s[i]);
	return (c_p);
}
