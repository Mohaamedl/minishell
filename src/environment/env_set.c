/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:32:04 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:32:19 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

int	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*node;
	t_env	*new_node;
	char	*new_value;

	if (!env || !key || !value)
		return (ERROR);
	node = find_env_node(*env, key);
	if (node)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (ERROR);
		free(node->value);
		node->value = new_value;
		return (SUCCESS);
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (ERROR);
	new_node->next = *env;
	*env = new_node;
	return (SUCCESS);
}

int	unset_env_value(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !key)
		return (ERROR);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (SUCCESS);
}
