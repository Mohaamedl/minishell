/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:34:16 by framiran          #+#    #+#             */
/*   Updated: 2025/08/08 12:30:51 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_size(long n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

static void	fill_number(char *str, long ln, int size)
{
	int	i;

	i = 1;
	while (ln > 0)
	{
		str[size - i] = '0' + (ln % 10);
		ln /= 10;
		i++;
	}
}

char	*ft_itoa(int n)
{
	long	ln;
	int		size;
	char	*str;

	ln = (long) n;
	size = calculate_size(ln);
	str = (char *) malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[size] = '\0';
	if (ln == 0)
		str[0] = '0';
	if (ln < 0)
	{
		str[0] = '-';
		ln = -ln;
	}
	fill_number(str, ln, size);
	return (str);
}
/*
int main()
{
	char* str = ft_itoa(-0);
	printf("%s\n",str);
}*/
