/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moh <moh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:00:00 by moh               #+#    #+#             */
/*   Updated: 2025/11/08 10:00:00 by moh              ###   ########.fr       */
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
