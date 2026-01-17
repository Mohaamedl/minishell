/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:54:05 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/13 12:54:13 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

int	match_pattern(const char *pattern, const char *str);

/**
 * @brief Fill matches array with duplicate of directory entry name
 *
 * Duplicates the entry name and stores it in the matches array at index i.
 *
 * @param matches Array to store matched filenames
 * @param entry Directory entry to duplicate
 * @param i Index in matches array
 * @return 1 on success, 0 on allocation failure
 */
static int	fill_match(char **matches, struct dirent *entry, int i)
{
	matches[i] = ft_strdup(entry->d_name);
	if (!matches[i])
		return (0);
	return (1);
}

/**
 * @brief Clean up matches array and close directory on error
 *
 * Frees all previously allocated strings in matches array up to index i,
 * then frees the array itself and closes the directory.
 *
 * @param matches Array of allocated strings to free
 * @param i Number of strings to free
 * @param dir Directory handle to close
 */
static void	cleanup_matches(char **matches, int i, DIR *dir)
{
	while (--i >= 0)
		free(matches[i]);
	free(matches);
	closedir(dir);
}

/**
 * @brief Initialize matches array and open current directory
 *
 * Allocates array for storing matched filenames and opens current directory
 * for reading. On failure, cleans up allocated resources.
 *
 * @param count Number of matches expected (array size)
 * @param dir Pointer to store opened directory handle
 * @return Allocated matches array, NULL on failure
 */
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

/**
 * @brief Get array of filenames matching the pattern
 *
 * Reads current directory and collects all entries that match the pattern.
 * Skips hidden files unless pattern starts with dot. Returns NULL-terminated
 * array of matched filenames. Caller must free returned array and strings.
 *
 * @param pattern Wildcard pattern to match
 * @param count Expected number of matches (array size)
 * @return Array of matched filename strings, NULL on error
 */
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
