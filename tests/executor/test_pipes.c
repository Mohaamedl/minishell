/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipes.c                                       :+:      :+:    :+:   */
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
#include <fcntl.h>

/**
 * Test: create_pipes() with 2 commands
 */
void	test_create_pipes_two_commands(void)
{
	int	*pipes;
	int	num_commands;

	printf("[TEST] create_pipes - 2 commands (1 pipe)...\n");
	
	num_commands = 2;
	pipes = create_pipes(num_commands);
	
	assert(pipes != NULL);
	
	/* Verify pipes are valid file descriptors */
	assert(fcntl(pipes[0], F_GETFD) != -1); /* read end */
	assert(fcntl(pipes[1], F_GETFD) != -1); /* write end */
	
	printf("  ✓ Created 1 pipe: [%d, %d]\n", pipes[0], pipes[1]);
	
	close_all_pipes(pipes, 1);
	free(pipes);
}

/**
 * Test: create_pipes() with 3 commands
 */
void	test_create_pipes_three_commands(void)
{
	int	*pipes;
	int	num_commands;
	int	i;

	printf("[TEST] create_pipes - 3 commands (2 pipes)...\n");
	
	num_commands = 3;
	pipes = create_pipes(num_commands);
	
	assert(pipes != NULL);
	
	/* Verify all 4 file descriptors (2 pipes × 2 FDs) */
	i = 0;
	while (i < 4)
	{
		assert(fcntl(pipes[i], F_GETFD) != -1);
		i++;
	}
	
	printf("  ✓ Created 2 pipes: [%d,%d] [%d,%d]\n", 
		pipes[0], pipes[1], pipes[2], pipes[3]);
	
	close_all_pipes(pipes, 2);
	free(pipes);
}

/**
 * Test: create_pipes() with 1 command (should return NULL)
 */
void	test_create_pipes_single_command(void)
{
	int	*pipes;

	printf("[TEST] create_pipes - 1 command (no pipe needed)...\n");
	
	pipes = create_pipes(1);
	assert(pipes == NULL);
	printf("  ✓ Correctly returned NULL for single command\n");
}

/**
 * Test: Pipe communication works correctly
 */
void	test_pipe_communication(void)
{
	int		*pipes;
	char	write_buf[] = "Hello, pipe!";
	char	read_buf[100];
	ssize_t	n;

	printf("[TEST] create_pipes - pipe communication...\n");
	
	pipes = create_pipes(2);
	assert(pipes != NULL);
	
	/* Write to pipe */
	n = write(pipes[1], write_buf, strlen(write_buf));
	assert(n == (ssize_t)strlen(write_buf));
	
	/* Read from pipe */
	n = read(pipes[0], read_buf, sizeof(read_buf) - 1);
	assert(n > 0);
	read_buf[n] = '\0';
	
	assert(strcmp(write_buf, read_buf) == 0);
	printf("  ✓ Pipe communication successful: '%s'\n", read_buf);
	
	close_all_pipes(pipes, 1);
	free(pipes);
}

/**
 * Test: close_all_pipes() closes all FDs
 */
void	test_close_all_pipes(void)
{
	int	*pipes;
	int	i;

	printf("[TEST] close_all_pipes - verify all FDs closed...\n");
	
	pipes = create_pipes(3); /* 2 pipes = 4 FDs */
	assert(pipes != NULL);
	
	/* Verify FDs are open */
	i = 0;
	while (i < 4)
	{
		assert(fcntl(pipes[i], F_GETFD) != -1);
		i++;
	}
	
	close_all_pipes(pipes, 2);
	
	/* Verify FDs are closed (fcntl should return -1) */
	i = 0;
	while (i < 4)
	{
		assert(fcntl(pipes[i], F_GETFD) == -1);
		i++;
	}
	
	printf("  ✓ All 4 file descriptors closed successfully\n");
	free(pipes);
}

/**
 * Test: No FD leaks with multiple pipe creation/destruction
 */
void	test_no_fd_leaks(void)
{
	int	*pipes;
	int	i;
	int	initial_fd;
	int	final_fd;

	printf("[TEST] create_pipes - FD leak check...\n");
	
	/* Get initial FD count (open a file to get next FD) */
	initial_fd = open("/dev/null", O_RDONLY);
	close(initial_fd);
	
	/* Create and destroy pipes multiple times */
	i = 0;
	while (i < 50)
	{
		pipes = create_pipes(4); /* 3 pipes */
		if (pipes)
		{
			close_all_pipes(pipes, 3);
			free(pipes);
		}
		i++;
	}
	
	/* Check final FD - should be same as initial */
	final_fd = open("/dev/null", O_RDONLY);
	close(final_fd);
	
	assert(initial_fd == final_fd);
	printf("  ✓ No FD leaks (initial FD=%d, final FD=%d)\n", 
		initial_fd, final_fd);
}

/**
 * Test: setup_pipe_fds() for first command
 */
void	test_setup_first_command(void)
{
	int		*pipes;
	pid_t	pid;
	int		status;

	printf("[TEST] setup_pipe_fds - first command in pipeline...\n");
	
	pipes = create_pipes(3); /* 3 commands, 2 pipes */
	assert(pipes != NULL);
	
	pid = fork();
	if (pid == 0)
	{
		/* Child: first command (index 0) */
		setup_pipe_fds(pipes, 0, 3);
		
		/* stdout should be redirected to pipe */
		write(STDOUT_FILENO, "test", 4);
		exit(0);
	}
	
	/* Parent */
	close(pipes[1]); /* Close write end */
	
	char buf[10];
	read(pipes[0], buf, 4);
	buf[4] = '\0';
	
	assert(strcmp(buf, "test") == 0);
	printf("  ✓ First command stdout redirected to pipe\n");
	
	close_all_pipes(pipes, 2);
	free(pipes);
	waitpid(pid, &status, 0);
}

/**
 * Run all pipe tests
 */
void	run_pipe_tests(void)
{
	printf("\n=== PIPE TESTS (KAN-52) ===\n");
	test_create_pipes_two_commands();
	test_create_pipes_three_commands();
	test_create_pipes_single_command();
	test_pipe_communication();
	test_close_all_pipes();
	test_no_fd_leaks();
	test_setup_first_command();
	printf("=== PIPE TESTS: ALL PASSED ===\n\n");
}

int	main(void)
{
	run_pipe_tests();
	return (0);
}
