/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:58 by mhaddadi          #+#    #+#             */
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

static int	parse_env_var(const char *env_str, char **key, char **value)
{
	char	*equals;
	size_t	key_len;

	equals = ft_strchr(env_str, '=');
	if (!equals)
		return (ERROR);
	key_len = equals - env_str;
	*key = ft_substr(env_str, 0, key_len);
	*value = ft_strdup(equals + 1);
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (SUCCESS);
}

static void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (parse_env_var(envp[i], &key, &value) == SUCCESS)
		{
			node = create_env_node(key, value);
			free(key);
			free(value);
			if (!node)
			{
				free_env(env_list);
				return (NULL);
			}
			add_env_node(&env_list, node);
		}
		i++;
	}
	return (env_list);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
