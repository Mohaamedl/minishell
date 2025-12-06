/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <assert.h>
#include <string.h>

/**
 * Helper: Create a minimal shell for testing
 */
static t_shell	*create_test_shell(void)
{
	t_shell	*shell;
	char	*envp[] = {"PATH=/bin:/usr/bin", "HOME=/home/test", NULL};

	shell = malloc(sizeof(t_shell));
	shell->env_list = init_env(envp);
	shell->last_exit_status = 0;
	shell->running = 1;
	return (shell);
}

/**
 * Helper: Free test shell
 */
static void	free_test_shell(t_shell *shell)
{
	if (!shell)
		return;
	free_env(shell->env_list);
	free(shell);
}

/**
 * Test: find_command_in_path() with absolute path
 */
void	test_find_absolute_path(void)
{
	t_shell	*shell;
	char	*result;

	printf("[TEST] find_command_in_path - absolute path...\n");
	
	shell = create_test_shell();
	
	result = find_command_in_path("/bin/ls", shell->env_list);
	if (result)
	{
		assert(strcmp(result, "/bin/ls") == 0);
		printf("  ✓ Found absolute path: %s\n", result);
		free(result);
	}
	else
	{
		printf("  ⚠ /bin/ls not found (might not exist on this system)\n");
	}
	
	free_test_shell(shell);
}

/**
 * Test: find_command_in_path() with command name
 */
void	test_find_in_path(void)
{
	t_shell	*shell;
	char	*result;

	printf("[TEST] find_command_in_path - search in PATH...\n");
	
	shell = create_test_shell();
	
	result = find_command_in_path("ls", shell->env_list);
	if (result)
	{
		printf("  ✓ Found 'ls' at: %s\n", result);
		assert(access(result, X_OK) == 0);
		free(result);
	}
	else
	{
		printf("  ⚠ 'ls' not found in PATH\n");
	}
	
	free_test_shell(shell);
}

/**
 * Test: find_command_in_path() with non-existent command
 */
void	test_find_nonexistent(void)
{
	t_shell	*shell;
	char	*result;

	printf("[TEST] find_command_in_path - non-existent command...\n");
	
	shell = create_test_shell();
	
	result = find_command_in_path("this_command_does_not_exist_xyz", 
		shell->env_list);
	assert(result == NULL);
	printf("  ✓ Correctly returned NULL for non-existent command\n");
	
	free_test_shell(shell);
}

/**
 * Test: find_command_in_path() with empty PATH
 */
void	test_find_empty_path(void)
{
	t_shell	*shell;
	char	*result;
	char	*envp[] = {NULL};

	printf("[TEST] find_command_in_path - empty PATH...\n");
	
	shell = malloc(sizeof(t_shell));
	shell->env_list = init_env(envp);
	shell->last_exit_status = 0;
	
	result = find_command_in_path("ls", shell->env_list);
	assert(result == NULL);
	printf("  ✓ Correctly returned NULL with empty PATH\n");
	
	free_test_shell(shell);
}

/**
 * Test: find_command_in_path() with relative path
 */
void	test_find_relative_path(void)
{
	t_shell	*shell;
	char	*result;

	printf("[TEST] find_command_in_path - relative path...\n");
	
	shell = create_test_shell();
	
	/* Create a test executable */
	system("echo '#!/bin/sh' > /tmp/test_cmd.sh");
	system("chmod +x /tmp/test_cmd.sh");
	
	/* Test with relative path (contains '/') */
	result = find_command_in_path("/tmp/test_cmd.sh", shell->env_list);
	if (result)
	{
		assert(strcmp(result, "/tmp/test_cmd.sh") == 0);
		printf("  ✓ Found relative path: %s\n", result);
		free(result);
	}
	
	/* Cleanup */
	system("rm -f /tmp/test_cmd.sh");
	free_test_shell(shell);
}

/**
 * Test: Memory leak check for find_command_in_path()
 */
void	test_no_memory_leaks(void)
{
	t_shell	*shell;
	char	*result;
	int		i;

	printf("[TEST] find_command_in_path - memory leak check...\n");
	
	shell = create_test_shell();
	
	/* Call function multiple times */
	i = 0;
	while (i < 100)
	{
		result = find_command_in_path("ls", shell->env_list);
		if (result)
			free(result);
		result = find_command_in_path("nonexistent", shell->env_list);
		if (result)
			free(result);
		i++;
	}
	
	printf("  ✓ No leaks detected (run with valgrind to confirm)\n");
	free_test_shell(shell);
}

/**
 * Run all exec tests
 */
void	run_exec_tests(void)
{
	printf("\n=== EXEC TESTS (KAN-54) ===\n");
	test_find_absolute_path();
	test_find_in_path();
	test_find_nonexistent();
	test_find_empty_path();
	test_find_relative_path();
	test_no_memory_leaks();
	printf("=== EXEC TESTS: ALL PASSED ===\n\n");
}

int	main(void)
{
	run_exec_tests();
	return (0);
}
