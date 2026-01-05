/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:35:11 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:35:13 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** handle_sigint - Handler for SIGINT (Ctrl+C) in interactive mode
**
** Sets global flag, prints newline, and redisplays prompt.
** The flag is checked in main loop to set exit status to 130.
*/
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** setup_signals_interactive - Configure signals for interactive prompt
**
** SIGINT (Ctrl+C): Custom handler to redisplay prompt
** SIGQUIT (Ctrl+\): Ignored (SIG_IGN) - fixes issue #23
*/
void	setup_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	rl_event_hook = NULL;  // Clear any event hook from heredoc mode
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** handle_sigint_heredoc - Handler for SIGINT (Ctrl+C) during heredoc input
**
** Sets global flag and prints newline.
** Does NOT use SA_RESTART so readline() will return NULL immediately.
*/
static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT, "\n", 1);
	rl_done = 1;  // Force readline to exit its loop
}

/*
** heredoc_event_hook - Event hook called by readline periodically
**
** Checks if SIGINT was received and forces readline to exit if so.
** Returns non-zero to tell readline to abort.
*/
static int	heredoc_event_hook(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_done = 1;
		return (1);  // Non-zero tells readline to abort
	}
	return (0);
}

/*
** setup_signals_heredoc - Configure signals for heredoc input
**
** SIGINT (Ctrl+C): Handler that causes readline to exit
** SIGQUIT (Ctrl+\): Ignored
** 
** Key difference: No SA_RESTART flag, so readline() returns NULL on Ctrl+C
** Also sets up event hook to check for signals periodically
*/
void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;  // NO SA_RESTART - this makes readline() return NULL
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	rl_event_hook = heredoc_event_hook;  // Check for signals periodically
}

/*
** setup_signals_executing - Configure signals during command execution
**
** Both SIGINT and SIGQUIT are ignored by parent during child execution.
*/
void	setup_signals_executing(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
