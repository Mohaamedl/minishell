/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/08 20:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_matches(const char *pattern);
char	**get_matches(const char *pattern, int count);

static void	sort_matches(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static char	**create_single_result(const char *pattern)
{
	char	**result;

	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = ft_strdup(pattern);
	result[1] = NULL;
	return (result);
}

char	**expand_wildcard(const char *pattern)
{
	int		count;
	char	**matches;

	if (!pattern || !has_wildcard(pattern))
		return (create_single_result(pattern));
	count = count_matches(pattern);
	if (count == 0)
		return (create_single_result(pattern));
	matches = get_matches(pattern, count);
	if (!matches)
		return (NULL);
	sort_matches(matches, count);
	return (matches);
}

void	free_wildcard_matches(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*join_wildcard_matches(char **matches)
{
	char	*result;
	char	*temp;
	int		i;

	if (!matches || !matches[0])
		return (NULL);
	result = ft_strdup(matches[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (matches[i])
	{
		temp = ft_strjoin(result, " ");
		free(result);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, matches[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
