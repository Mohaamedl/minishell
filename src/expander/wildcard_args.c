/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/08 20:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arg	*create_arg_node(char *value)
{
	t_arg	*new_arg;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return (NULL);
	new_arg->value = value;
	new_arg->is_expandable = 0;
	new_arg->is_wildcard_expandable = 0;
	new_arg->was_expanded = 1;
	new_arg->next = NULL;
	return (new_arg);
}

static void	append_arg(t_arg **head, t_arg **tail, t_arg *new_arg)
{
	if (!*head)
	{
		*head = new_arg;
		*tail = new_arg;
	}
	else
	{
		(*tail)->next = new_arg;
		*tail = new_arg;
	}
}

static void	expand_matched_args(char **matches, t_arg **head, t_arg **tail)
{
	t_arg	*new_arg;
	int		i;

	i = 0;
	while (matches[i])
	{
		new_arg = create_arg_node(matches[i]);
		if (new_arg)
			append_arg(head, tail, new_arg);
		i++;
	}
	free(matches);
}

static void	copy_arg(t_arg *current, t_arg **head, t_arg **tail)
{
	t_arg	*new_arg;
	char	*dup_value;

	dup_value = ft_strdup(current->value);
	new_arg = create_arg_node(dup_value);
	if (new_arg)
		append_arg(head, tail, new_arg);
	else
		free(dup_value);
}

t_arg	*expand_wildcards_in_args(t_arg *args)
{
	t_arg	*current;
	t_arg	*new_head;
	t_arg	*new_tail;
	char	**matches;

	new_head = NULL;
	new_tail = NULL;
	current = args;
	while (current)
	{
		if (current->is_wildcard_expandable && has_wildcard(current->value))
		{
			matches = expand_wildcard(current->value);
			if (matches)
				expand_matched_args(matches, &new_head, &new_tail);
		}
		else
			copy_arg(current, &new_head, &new_tail);
		current = current->next;
	}
	return (new_head);
}
