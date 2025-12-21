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
** handle_sigint_heredoc - Handler for SIGINT (Ctrl+C) during heredoc input
**
** Sets global flag and interrupts readline to return control.
** Does not redisplay prompt - lets heredoc function handle cleanup.
*/
static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	rl_done = 1;
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

/*
** setup_signals_heredoc - Configure signals for heredoc input
**
** SIGINT (Ctrl+C): Custom handler that interrupts readline
** SIGQUIT (Ctrl+\): Ignored (SIG_IGN)
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
}
