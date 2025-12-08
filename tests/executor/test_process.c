/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <assert.h>
#include <sys/wait.h>

/**
 * Test: create_process() should successfully create a child process
 * 
 * Expected behavior:
 * - Parent receives child's PID (> 0)
 * - Child receives 0
 * - No FD leaks
 */
void	test_create_process_success(void)
{
	pid_t	pid;
	int		status;

	printf("[TEST] create_process - basic fork...\n");
	
	pid = create_process();
	
	if (pid == 0)
	{
		/* Child process */
		exit(42);
	}
	else if (pid > 0)
	{
		/* Parent process */
		assert(pid > 0);
		waitpid(pid, &status, 0);
		assert(WIFEXITED(status));
		assert(WEXITSTATUS(status) == 42);
		printf("  ✓ Fork successful, child PID: %d\n", pid);
	}
	else
	{
		printf("  ✗ Fork failed\n");
		assert(0);
	}
}

/**
 * Test: Multiple sequential forks should work correctly
 */
void	test_multiple_forks(void)
{
	pid_t	pids[3];
	int		i;
	int		status;

	printf("[TEST] create_process - multiple forks...\n");
	
	i = 0;
	while (i < 3)
	{
		pids[i] = create_process();
		if (pids[i] == 0)
		{
			/* Child: exit with index */
			exit(i);
		}
		assert(pids[i] > 0);
		i++;
	}
	
	/* Parent: wait for all children */
	i = 0;
	while (i < 3)
	{
		waitpid(pids[i], &status, 0);
		assert(WIFEXITED(status));
		assert(WEXITSTATUS(status) == i);
		i++;
	}
	
	printf("  ✓ All 3 forks successful\n");
}

/**
 * Run all process tests
 */
void	run_process_tests(void)
{
	printf("\n=== PROCESS TESTS (KAN-53) ===\n");
	test_create_process_success();
	test_multiple_forks();
	printf("=== PROCESS TESTS: ALL PASSED ===\n\n");
}

int	main(void)
{
	run_process_tests();
	return (0);
}
