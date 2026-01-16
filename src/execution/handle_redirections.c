/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 16:22:41 by framiran          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:44 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
 * @brief Check if redirection list has output redirections
 * 
 * @param first_redir First redirection in list
 * @return 1 if has output/append redirections, 0 otherwise
 */
int	has_out_redirs(t_redir *first_redir)
{
	t_redir	*curr_redir;

	curr_redir = first_redir;
	while (curr_redir != NULL)
	{
		if (curr_redir->type == REDIR_OUT || curr_redir->type == APPEND)
			return (1);
		curr_redir = curr_redir->next;
	}
	return (0);
}

/**
 * @brief Check if redirection list has input redirections
 * 
 * @param first_redir First redirection in list
 * @return 1 if has input redirections or heredocs, 0 otherwise
 */
int	has_in_redirs(t_redir *first_redir)
{
	t_redir	*curr_redir;

	curr_redir = first_redir;
	while (curr_redir != NULL)
	{
		if (curr_redir->type == REDIR_IN)
			return (1);
		curr_redir = curr_redir->next;
	}
	if (has_heredocs(first_redir))
		return (1);
	return (0);
}
