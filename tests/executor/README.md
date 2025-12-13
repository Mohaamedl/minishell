# Executor Module Tests

## Overview
Comprehensive test suite for the command execution module (KAN-52 to KAN-55).

## Test Coverage

### KAN-53: Process Creation (`test_process.c`)
- **test_create_process_success**: Verifies fork() creates child process correctly
- **test_multiple_forks**: Tests multiple sequential fork operations
- **Coverage**: Parent/child PID handling, exit codes, no FD leaks

### KAN-55: Process Waiting (`test_wait.c`)
- **test_wait_normal_exit**: Verifies normal exit code capture (0-255)
- **test_wait_signal_termination**: Tests signal termination (128 + signal)
- **test_wait_pipeline**: Tests waiting for multiple processes
- **test_pipeline_mixed_exits**: Verifies last command's exit is returned
- **Coverage**: Exit status extraction, signal handling, pipeline behavior

### KAN-54: Command Execution (`test_exec.c`)
- **test_find_absolute_path**: Tests absolute path resolution
- **test_find_in_path**: Tests PATH directory search
- **test_find_nonexistent**: Tests non-existent command handling
- **test_find_empty_path**: Tests behavior with no PATH
- **test_find_relative_path**: Tests relative path handling
- **test_no_memory_leaks**: Verifies no memory leaks in PATH search
- **Coverage**: PATH parsing, execve preparation, error handling

### KAN-52: Pipe Management (`test_pipes.c`)
- **test_create_pipes_two_commands**: Tests single pipe creation
- **test_create_pipes_three_commands**: Tests multiple pipe creation
- **test_create_pipes_single_command**: Tests edge case (no pipe needed)
- **test_pipe_communication**: Verifies data transfer through pipes
- **test_close_all_pipes**: Tests all FDs are properly closed
- **test_no_fd_leaks**: Verifies no FD leaks over multiple operations
- **test_setup_first_command**: Tests FD setup for pipeline commands
- **Coverage**: Pipe creation, FD management, communication, leak prevention

## Running Tests

### Quick Run
```bash
cd tests/executor
./run_executor_tests.sh
```

### Individual Tests
```bash
# Compile and run individual test
gcc -Wall -Wextra -Werror -g -I../../include -I../../Libft \
    test_process.c ../../src/executor/process.c -L../../Libft -lft \
    -o test_process && ./test_process
```

### Memory Leak Check
```bash
# Run with valgrind
valgrind --leak-check=full --show-leak-kinds=all \
    --track-fds=yes ./test_process
```

### FD Leak Check
```bash
# Check for open file descriptors
lsof -p $$ | grep -E "pipe|test"
```

## Test Expectations

### All tests should:
1. ✓ Pass all assertions
2. ✓ Show no memory leaks (valgrind)
3. ✓ Show no FD leaks
4. ✓ Follow norminette standards
5. ✓ Clean up all resources

### Expected Output
```
=== PROCESS TESTS (KAN-53) ===
[TEST] create_process - basic fork...
  ✓ Fork successful, child PID: 12345
[TEST] create_process - multiple forks...
  ✓ All 3 forks successful
=== PROCESS TESTS: ALL PASSED ===

=== WAIT TESTS (KAN-55) ===
[TEST] wait_for_process - normal exit...
  ✓ Correctly captured exit code: 5
[TEST] wait_for_process - signal termination...
  ✓ Correctly captured signal exit: 143 (128 + 15)
=== WAIT TESTS: ALL PASSED ===

...
```

## Norminette Compliance

All test files follow 42 norminette standards:
- Function names in snake_case
- Max 25 lines per function
- Max 80 columns per line
- Proper header comments
- No forbidden functions

## Debugging Tips

### Test Failures
```bash
# Run with debug output
gcc -g -DDEBUG test_process.c ... && gdb ./test_process
```

### Memory Issues
```bash
# Detailed leak check
valgrind --leak-check=full --show-leak-kinds=all \
    --track-origins=yes ./test_pipes
```

### FD Tracking
```bash
# Monitor FD usage during test
watch -n 0.1 'lsof -p $(pgrep test_pipes) | wc -l'
```

## Integration with CI/CD

These tests are designed to run in automated environments:
- Exit code 0 on success
- Exit code 1 on failure
- No interactive input required
- Deterministic results

## Future Enhancements

- [ ] Add stress tests (1000+ forks)
- [ ] Test edge cases (RLIMIT_NOFILE)
- [ ] Add performance benchmarks
- [ ] Test with different shell environments
- [ ] Add fuzzing tests

## Related Documentation

- [EXECUTION_PLAN.md](/docs/EXECUTION_PLAN.md) - Implementation details
- [ARCHITECTURE.md](/docs/ARCHITECTURE.md) - Overall architecture
- [TESTING.md](/docs/TESTING.md) - Testing strategy
