/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:53:57 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/13 12:54:13 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_matches(const char *pattern);
char	**get_matches(const char *pattern, int count);

/**
 * @brief Sort array of strings in lexicographic order
 *
 * Implements bubble sort to arrange matched filenames alphabetically.
 * Used to ensure wildcard expansions are in predictable order.
 *
 * @param arr Array of strings to sort
 * @param count Number of strings in array
 */
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

/**
 * @brief Create a single-element result array containing the pattern
 *
 * Used when wildcard expansion finds no matches or pattern has no wildcards.
 * Returns the original pattern as the only result.
 *
 * @param pattern Original pattern string to return
 * @return Array with pattern as only element, NULL on allocation failure
 */
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

/**
 * @brief Expand wildcard pattern to array of matching filenames
 *
 * If pattern contains wildcards, matches against current directory files.
 * Returns sorted array of matches, or pattern itself if no matches found.
 * If no wildcards present, returns pattern unchanged.
 *
 * @param pattern Wildcard pattern to expand
 * @return Array of matched filenames, NULL-terminated
 */
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

/**
 * @brief Free array of wildcard match strings
 *
 * Iterates through array freeing each string, then frees the array itself.
 *
 * @param arr Array of strings to free
 */
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

/**
 * @brief Join array of wildcard matches into single space-separated string
 *
 * Concatenates all matched filenames with spaces between them.
 * Used when converting matched files back to a single string representation.
 *
 * @param matches Array of matched filename strings
 * @return Single string with all matches joined by spaces, NULL on error
 */
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
