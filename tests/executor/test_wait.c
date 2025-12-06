/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <assert.h>
#include <signal.h>

/**
 * Test: wait_for_process() with normal exit
 */
void	test_wait_normal_exit(void)
{
	pid_t	pid;
	int		exit_code;

	printf("[TEST] wait_for_process - normal exit...\n");
	
	pid = fork();
	if (pid == 0)
		exit(5);
	
	exit_code = wait_for_process(pid);
	assert(exit_code == 5);
	printf("  ✓ Correctly captured exit code: %d\n", exit_code);
}

/**
 * Test: wait_for_process() with signal termination
 */
void	test_wait_signal_termination(void)
{
	pid_t	pid;
	int		exit_code;

	printf("[TEST] wait_for_process - signal termination...\n");
	
	pid = fork();
	if (pid == 0)
	{
		/* Child: kill itself with SIGTERM */
		kill(getpid(), SIGTERM);
		exit(0);
	}
	
	exit_code = wait_for_process(pid);
	/* Signal termination: 128 + signal_number */
	assert(exit_code == 128 + SIGTERM);
	printf("  ✓ Correctly captured signal exit: %d (128 + %d)\n", 
		exit_code, SIGTERM);
}

/**
 * Test: wait_for_pipeline() with multiple processes
 */
void	test_wait_pipeline(void)
{
	pid_t	pids[3];
	int		i;
	int		last_exit;

	printf("[TEST] wait_for_pipeline - multiple processes...\n");
	
	i = 0;
	while (i < 3)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			exit(i + 1);
		i++;
	}
	
	/* Should return exit code of last process (3) */
	last_exit = wait_for_pipeline(pids, 3);
	assert(last_exit == 3);
	printf("  ✓ Pipeline returned last exit code: %d\n", last_exit);
}

/**
 * Test: wait_for_pipeline() with different exit codes
 */
void	test_pipeline_mixed_exits(void)
{
	pid_t	pids[4];
	int		exits[] = {0, 1, 127, 42};
	int		i;
	int		result;

	printf("[TEST] wait_for_pipeline - mixed exit codes...\n");
	
	i = 0;
	while (i < 4)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			exit(exits[i]);
		i++;
	}
	
	result = wait_for_pipeline(pids, 4);
	assert(result == 42); /* Last command's exit */
	printf("  ✓ Correctly returned last exit code: %d\n", result);
}

/**
 * Run all wait tests
 */
void	run_wait_tests(void)
{
	printf("\n=== WAIT TESTS (KAN-55) ===\n");
	test_wait_normal_exit();
	test_wait_signal_termination();
	test_wait_pipeline();
	test_pipeline_mixed_exits();
	printf("=== WAIT TESTS: ALL PASSED ===\n\n");
}

int	main(void)
{
	run_wait_tests();
	return (0);
}
