/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:34:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:34:32 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

void	print_error_prefix(const char *cmd)
{
	write(STDERR, "minishell: ", 11);
	if (cmd)
	{
		write(STDERR, cmd, ft_strlen(cmd));
		write(STDERR, ": ", 2);
	}
}

void	print_error(const char *cmd, const char *arg, const char *msg)
{
	print_error_prefix(cmd);
	if (arg)
	{
		write(STDERR, arg, ft_strlen(arg));
		write(STDERR, ": ", 2);
	}
	if (msg)
		write(STDERR, msg, ft_strlen(msg));
	write(STDERR, "\n", 1);
}
