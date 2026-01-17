/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 12:53:41 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/13 12:54:13 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

/**
 * @brief Match a pattern with wildcards against a string
 *
 * Recursively matches a pattern containing wildcards (*) against a string.
 * The asterisk matches any sequence of characters including empty string.
 * Multiple consecutive asterisks are treated as one.
 *
 * @param pattern Pattern string that may contain wildcards
 * @param str String to match against the pattern
 * @return 1 if pattern matches, 0 otherwise
 */
int	match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*')
	{
		while (*(pattern + 1) == '*')
			pattern++;
		if (*(pattern + 1) == '\0')
			return (1);
		while (*str)
		{
			if (match_pattern(pattern + 1, str))
				return (1);
			str++;
		}
		return (match_pattern(pattern + 1, str));
	}
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

/**
 * @brief Check if a string contains wildcard characters
 *
 * Scans through the string to detect presence of asterisk wildcard.
 *
 * @param str String to check for wildcards
 * @return 1 if string contains wildcards, 0 otherwise
 */
int	has_wildcard(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}

/**
 * @brief Count how many files match the given pattern in current directory
 *
 * Opens current directory and counts entries that match the wildcard pattern.
 * Skips hidden files (starting with .) unless pattern starts with dot.
 *
 * @param pattern Wildcard pattern to match against directory entries
 * @return Number of matching files, 0 if directory cannot be opened
 */
int	count_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || pattern[0] == '.')
		{
			if (match_pattern(pattern, entry->d_name))
				count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}
