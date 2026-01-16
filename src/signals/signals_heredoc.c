/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/13 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle SIGINT (Ctrl+C) during heredoc input
 * 
 * Sets global flag and prints newline. Does NOT use SA_RESTART
 * so readline() will return NULL immediately.
 * 
 * @param sig Signal number (unused)
 */
static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT, "\n", 1);
	rl_done = 1;
}

/**
 * @brief Event hook called by readline periodically
 * 
 * Checks if SIGINT was received and forces readline to exit.
 * 
 * @return Non-zero to tell readline to abort, 0 to continue
 */
static int	heredoc_event_hook(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Configure signals for heredoc input
 * 
 * SIGINT (Ctrl+C): Handler that causes readline to exit
 * SIGQUIT (Ctrl+\): Ignored
 * 
 * Key difference: No SA_RESTART flag, so readline() returns NULL on Ctrl+C.
 * Also sets up event hook to check for signals periodically.
 */
void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	rl_event_hook = heredoc_event_hook;
}
