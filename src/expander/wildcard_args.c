/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:53:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/13 12:53:12 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a new argument node
 *
 * Allocates and initializes a new t_arg node with the given value.
 * Sets expansion flags to indicate the argument has been processed.
 *
 * @param value String value for the argument
 * @return Pointer to new argument node, NULL on allocation failure
 */
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

/**
 * @brief Append a new argument node to the linked list
 *
 * Adds the new argument to the end of the list, updating head if empty
 * and tail pointer to maintain the end of the list.
 *
 * @param head Pointer to head of argument list
 * @param tail Pointer to tail of argument list
 * @param new_arg New argument node to append
 */
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

/**
 * @brief Expand matched wildcard filenames into argument nodes
 *
 * Creates argument nodes from array of matched filenames and appends them
 * to the list. Frees the matches array after processing. On failure during
 * node creation, frees all matches and returns early.
 *
 * @param matches Array of matched filename strings
 * @param head Pointer to head of argument list
 * @param tail Pointer to tail of argument list
 */
static void	expand_matched_args(char **matches, t_arg **head, t_arg **tail)
{
	t_arg	*new_arg;
	int		i;
	int		j;

	i = 0;
	while (matches[i])
	{
		new_arg = create_arg_node(matches[i]);
		if (new_arg)
			append_arg(head, tail, new_arg);
		else
		{
			j = 0;
			while (matches[j])
			{
				free(matches[j]);
				j++;
			}
			free(matches);
			return ;
		}
		i++;
	}
	free(matches);
}

/**
 * @brief Copy an argument node to a new list
 *
 * Duplicates the value of the current argument and creates a new node
 * with the copied value, appending it to the list. On allocation failure
 * the duplicated value is freed.
 *
 * @param current Argument node to copy
 * @param head Pointer to head of new argument list
 * @param tail Pointer to tail of new argument list
 */
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

/**
 * @brief Expand wildcards in argument list to matching filenames
 *
 * Processes each argument in the list. If argument is wildcard-expandable
 * and contains wildcards, expands it to matching files. Otherwise copies
 * the argument as-is. Returns a new argument list with expansions applied.
 *
 * @param args Original argument list
 * @return New argument list with wildcards expanded
 */
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
