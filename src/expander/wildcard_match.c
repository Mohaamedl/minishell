/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/08 20:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

int	match_pattern(const char *pattern, const char *str);

static int	fill_match(char **matches, struct dirent *entry, int i)
{
	matches[i] = ft_strdup(entry->d_name);
	if (!matches[i])
		return (0);
	return (1);
}

static void	cleanup_matches(char **matches, int i, DIR *dir)
{
	while (--i >= 0)
		free(matches[i]);
	free(matches);
	closedir(dir);
}

static char	**init_matches(int count, DIR **dir)
{
	char	**matches;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	*dir = opendir(".");
	if (!*dir)
	{
		free(matches);
		return (NULL);
	}
	return (matches);
}

char	**get_matches(const char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				i;

	matches = init_matches(count, &dir);
	if (!matches)
		return (NULL);
	i = 0;
	entry = readdir(dir);
	while (entry && i < count)
	{
		if (entry->d_name[0] != '.' || pattern[0] == '.')
		{
			if (match_pattern(pattern, entry->d_name))
			{
				if (!fill_match(matches, entry, i++))
					return (cleanup_matches(matches, i, dir), NULL);
			}
		}
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
	return (matches);
}
