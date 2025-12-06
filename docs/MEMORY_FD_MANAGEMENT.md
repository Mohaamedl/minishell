# 🔒 Memory & FD Management Guidelines

**CRITICAL**: Zero tolerance for leaks in minishell project

---

## 🎯 Golden Rules

### Rule #1: Every malloc() needs a free()
```c
// ✅ GOOD
char *str = malloc(10);
if (!str)
    return (NULL);
// ... use str ...
free(str);

// ❌ BAD
char *str = malloc(10);
// ... use str ...
// Missing free() - LEAK!
```

### Rule #2: Every open/pipe/dup needs a close()
```c
// ✅ GOOD
int fd = open("file", O_RDONLY);
// ... use fd ...
close(fd);

// ❌ BAD
int fd = open("file", O_RDONLY);
// ... use fd ...
// Missing close() - FD LEAK!
```

---

## 🔧 Common Scenarios

### 1. Pipe Management

```c
// ✅ CORRECT: Close all unused pipe ends
int pipefd[2];
pipe(pipefd);

pid_t pid = fork();
if (pid == 0)
{
    // Child: read from pipe
    close(pipefd[1]);  // Close write end (not used)
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);  // Close after dup
    // ... exec ...
}
else
{
    // Parent: write to pipe
    close(pipefd[0]);  // Close read end (not used)
    // ... write ...
    close(pipefd[1]);  // Close write end when done
    waitpid(pid, NULL, 0);
}

// ❌ WRONG: Leaving pipes open
pipe(pipefd);
fork();
// Missing closes - DEADLOCK!
```

### 2. Multiple Pipes (Pipeline)

```c
// For: cmd1 | cmd2 | cmd3
// Create 2 pipes: pipe[0] and pipe[1]

// ✅ CORRECT Pattern
int **pipes = create_pipes(2);  // Allocate 2 pipes

// Fork cmd1
pid1 = fork();
if (pid1 == 0)
{
    close(pipes[0][0]);  // Not reading from pipe[0]
    close(pipes[1][0]);  // Not reading from pipe[1]
    close(pipes[1][1]);  // Not writing to pipe[1]
    
    dup2(pipes[0][1], STDOUT_FILENO);
    close(pipes[0][1]);
    exec_cmd1();
}

// Fork cmd2
pid2 = fork();
if (pid2 == 0)
{
    close(pipes[1][0]);  // Not reading from pipe[1]
    
    dup2(pipes[0][0], STDIN_FILENO);
    close(pipes[0][0]);
    close(pipes[0][1]);  // Already used
    
    dup2(pipes[1][1], STDOUT_FILENO);
    close(pipes[1][1]);
    exec_cmd2();
}

// Fork cmd3
pid3 = fork();
if (pid3 == 0)
{
    close(pipes[0][0]);  // Not needed
    close(pipes[0][1]);  // Not needed
    
    dup2(pipes[1][0], STDIN_FILENO);
    close(pipes[1][0]);
    exec_cmd3();
}

// ⚠️ CRITICAL: Parent MUST close ALL pipes
close(pipes[0][0]);
close(pipes[0][1]);
close(pipes[1][0]);
close(pipes[1][1]);

// Free pipe arrays
free(pipes[0]);
free(pipes[1]);
free(pipes);
```

### 3. Saved File Descriptors

```c
// ✅ CORRECT: Save and restore
int saved_stdin = dup(STDIN_FILENO);
int saved_stdout = dup(STDOUT_FILENO);

// ... apply redirections ...
// ... execute builtin ...

// Restore
dup2(saved_stdin, STDIN_FILENO);
dup2(saved_stdout, STDOUT_FILENO);

// ⚠️ CRITICAL: Close saved FDs
close(saved_stdin);
close(saved_stdout);

// ❌ WRONG: Not closing saved FDs
dup2(saved_stdin, STDIN_FILENO);
// Missing close(saved_stdin) - FD LEAK!
```

### 4. Memory Before Execve

```c
// ✅ CORRECT: Free before exec
char **args = convert_args_to_array(cmd);
char **env_array = env_list_to_array(shell->env);
char *path = find_command_in_path(cmd->name, shell->env);

// Free everything before execve
// (execve replaces process, so memory would leak)
free_args_array(args);
free_env_array(env_array);
free(path);

execve(path, args, env_array);

// If execve fails:
perror("execve");
exit(127);

// ❌ WRONG: Not freeing before exec
execve(path, args, env_array);  // Memory leaked if exec succeeds
```

### 5. PATH Search

```c
// ✅ CORRECT: Free all allocated strings
char *path_env = get_env_value(env, "PATH");
char **directories = ft_split(path_env, ':');

int i = 0;
char *full_path = NULL;
while (directories[i])
{
    full_path = ft_strjoin_three(directories[i], "/", cmd);
    if (access(full_path, X_OK) == 0)
        break;  // Found!
    free(full_path);  // ⚠️ Free if not found
    full_path = NULL;
    i++;
}

// Free split array
i = 0;
while (directories[i])
    free(directories[i++]);
free(directories);

return (full_path);  // Caller must free

// ❌ WRONG: Not freeing constructed paths
while (directories[i])
{
    full_path = ft_strjoin_three(directories[i], "/", cmd);
    if (access(full_path, X_OK) == 0)
        break;
    // Missing free(full_path) - LEAK!
    i++;
}
```

### 6. Convert Args to Array

```c
// ✅ CORRECT: Caller must free array (not strings)
char **convert_args_to_array(t_cmd *cmd)
{
    int count = count_args(cmd->args);
    char **args = malloc(sizeof(char *) * (count + 1));
    
    int i = 0;
    t_arg *current = cmd->args;
    while (current)
    {
        args[i++] = current->value;  // ⚠️ Point to existing strings
        current = current->next;
    }
    args[i] = NULL;
    
    return (args);
}

// Usage:
char **args = convert_args_to_array(cmd);
// ... use args ...
free(args);  // ⚠️ Only free array, NOT strings (belong to AST)

// ❌ WRONG: Freeing strings that belong to AST
char **args = convert_args_to_array(cmd);
int i = 0;
while (args[i])
    free(args[i++]);  // WRONG! These strings are in AST
free(args);
```

---

## 🧪 Verification Tools

### Valgrind - Memory Leaks
```bash
# Check for memory leaks
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./minishell

# Check for FD leaks
valgrind --leak-check=full \
         --track-fds=yes \
         ./minishell
```

### lsof - Open File Descriptors
```bash
# While minishell is running:
# In another terminal:
ps aux | grep minishell  # Get PID
lsof -p <PID>            # List open FDs

# Should only see:
# - 0: stdin
# - 1: stdout
# - 2: stderr
# - readline library files
```

### Custom FD Check
```c
// Debug function to list open FDs
void	debug_print_open_fds(void)
{
	int		fd;
	int		count;

	count = 0;
	fd = 3;  // Start after stdin/stdout/stderr
	while (fd < 1024)
	{
		if (fcntl(fd, F_GETFD) != -1)
		{
			printf("FD %d is open\n", fd);
			count++;
		}
		fd++;
	}
	printf("Total open FDs: %d\n", count);
}
```

---

## ✅ Checklist Before Commit

- [ ] Run valgrind - 0 leaks
- [ ] Check with --track-fds=yes - no FD leaks
- [ ] Test with lsof - only expected FDs
- [ ] All malloc() have free()
- [ ] All open/pipe/dup have close()
- [ ] All pipe ends closed in parent
- [ ] All pipe ends closed in children
- [ ] Saved FDs restored and closed
- [ ] Memory freed before execve()
- [ ] PATH search frees all allocated memory
- [ ] Array conversions don't leak

---

## 🎯 Testing Commands

```bash
# Memory leak test
echo "echo test" | valgrind --leak-check=full ./minishell

# FD leak test
echo "ls | grep txt | wc -l" | valgrind --track-fds=yes ./minishell

# Pipeline stress test
valgrind ./minishell << EOF
ls | cat | cat | cat | cat | cat | cat | cat | cat | cat
exit
EOF

# Multiple commands
valgrind ./minishell << EOF
ls
pwd
echo test
export VAR=value
env | grep VAR
cd /tmp
pwd
exit
EOF
```

---

## 🔥 Common Mistakes

### ❌ Mistake 1: Not closing pipe after dup2
```c
dup2(pipefd[0], STDIN_FILENO);
// Missing: close(pipefd[0]);
```

### ❌ Mistake 2: Not closing in parent
```c
// Parent creates pipe and forks
pipe(pipefd);
fork();
// Missing: close(pipefd[0]); close(pipefd[1]);
```

### ❌ Mistake 3: Freeing AST strings
```c
char **args = convert_args_to_array(cmd);
free(args[0]);  // WRONG! Belongs to AST
```

### ❌ Mistake 4: Not freeing before exec
```c
char *path = find_path(cmd);
execve(path, args, env);
// Memory leaked if exec succeeds
```

### ❌ Mistake 5: Leaking in PATH search
```c
while (dirs[i])
{
    path = join(dirs[i], cmd);
    if (access(path, X_OK) == 0)
        break;
    // Missing: free(path);
    i++;
}
```

---

**Remember**: Clean code = No leaks. Always verify with valgrind!

