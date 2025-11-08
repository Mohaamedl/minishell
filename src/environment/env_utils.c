/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:32:11 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:32:19 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

void	print_env_list(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void	free_env_array(char **env_array)
{
	free_array(env_array);
}

static char	*create_env_entry(t_env *env)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, env->value);
	free(temp);
	return (result);
}

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	int		size;
	int		i;

	size = env_list_size(env);
	env_array = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		env_array[i] = create_env_entry(env);
		if (!env_array[i++])
		{
			free_env_array(env_array);
			return (NULL);
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
