/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:20 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/14 15:57:21 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Increment the SHLVL environment variable
 *
 * Gets the current SHLVL value, increments it by 1, and updates the
 * environment. If SHLVL doesn't exist, it starts from 0.
 *
 * @param env Pointer to the environment list
 */
