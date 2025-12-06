# 🎯 Executor Module

**Module**: Command Execution & Process Management  
**Issues**: KAN-52 to KAN-57  
**Author**: minishell team  
**Date**: December 6, 2025

---

## 📋 Overview

This module handles all external command execution, process management, and pipeline operations for the minishell project.

---

## 📁 Module Structure

```
src/executor/
├── process.c          # KAN-53: Fork wrapper
├── exec.c             # KAN-54: execve + PATH search
├── wait.c             # KAN-55: waitpid wrapper
├── pipes.c            # KAN-52: Pipe management
├── command_router.c   # KAN-56: Builtin vs external routing
└── README.md          # This file
```

---

## 🔧 Components

### 1. process.c (KAN-53)
**Purpose**: Safe process creation

**Functions**:
- `pid_t create_process(void)` - Fork with error handling

**Responsibilities**:
- Call fork()
- Handle fork errors
- Return pid (parent) or 0 (child)

---

### 2. exec.c (KAN-54)
**Purpose**: Execute external commands

**Functions**:
- `char *find_command_in_path(char *cmd, t_env *env)` - Search PATH
- `int execute_external_cmd(char **args, t_shell *shell)` - Execute command

**Responsibilities**:
- PATH directory search
- Absolute/relative path handling
- execve() call
- Error codes (127, 126)

**Algorithm**:
```
1. If cmd contains '/': use directly
2. Else:
   - Get PATH from environment
   - Split by ':'
   - For each directory:
     * Try dir/cmd
     * Check if executable
   - Return path or NULL
3. Convert env list to array
4. execve(path, args, env_array)
```

---

### 3. wait.c (KAN-55)
**Purpose**: Collect child process exit status

**Functions**:
- `int wait_for_process(pid_t pid)` - Wait for single process
- `int wait_for_pipeline(pid_t *pids, int count)` - Wait for pipeline

**Responsibilities**:
- waitpid() call
- Exit status extraction (WEXITSTATUS)
- Signal handling (128 + signal)

**Exit Status**:
```c
if (WIFEXITED(status))
    return WEXITSTATUS(status);  // Normal exit
else if (WIFSIGNALED(status))
    return 128 + WTERMSIG(status);  // Killed by signal
```

---

### 4. pipes.c (KAN-52)
**Purpose**: Create and manage pipelines

**Functions**:
- `int *create_pipes(int num_commands)` - Allocate pipes
- `void setup_pipe_fds(int *pipes, int index, int total)` - Setup FDs for command
- `void close_all_pipes(int *pipes, int count)` - Close all pipe FDs

**Pipe Structure**:
```
For: cmd1 | cmd2 | cmd3
Pipes: 2 (n-1)

cmd1: stdout → pipe[0][1]
cmd2: stdin  ← pipe[0][0]
      stdout → pipe[1][1]
cmd3: stdin  ← pipe[1][0]
```

**Critical**: Always close unused pipe ends to prevent deadlock!

---

### 5. command_router.c (KAN-56)
**Purpose**: Route commands to appropriate execution path

**Function**:
- `int execute_command(t_ast *node, t_shell *shell, int in_pipe)`

**Decision Logic**:
```c
if (is_builtin(cmd)) {
    if (in_pipe) {
        fork();  // Builtins in pipes need fork
        if (child) execute_builtin();
    } else {
        execute_builtin();  // No fork for standalone builtins
    }
} else {
    fork();
    if (child) execute_external();
}
waitpid();
```

**Why no fork for standalone builtins?**
- `cd` must change parent's directory
- `export`/`unset` must modify parent's environment
- `exit` must exit parent shell

---

## 🔄 Execution Flow

```
┌─────────────────────────────────────────┐
│         Command Router                  │
│         (command_router.c)              │
└─────────────┬───────────────────────────┘
              │
      ┌───────┴────────┐
      │                │
   Builtin?         External
      │                │
      NO               YES
      │                │
      │           ┌────▼────┐
      │           │  Fork   │
      │           │ (process.c)
      │           └────┬────┘
      │                │
      │           ┌────▼────┐
      │           │  Exec   │
      │           │ (exec.c)│
      │           └────┬────┘
      │                │
      │           ┌────▼────┐
      │           │  Wait   │
      │           │ (wait.c)│
      │           └─────────┘
      │
   ┌──▼──┐
   │Built│
   │ in  │
   └─────┘
```

---

## 🧪 Testing

### Test Files
```
tests/executor/
├── test_fork.sh
├── test_execve.sh
├── test_wait.sh
├── test_pipes.sh
└── test_routing.sh
```

### Test Coverage

**Process Creation**:
- ✓ Successful fork
- ✓ Fork error handling
- ✓ Child isolation

**Command Execution**:
- ✓ Absolute paths (`/bin/ls`)
- ✓ PATH search (`ls`)
- ✓ Relative paths (`./script`)
- ✓ Command not found (127)
- ✓ Permission denied (126)

**Pipes**:
- ✓ 2-command pipeline
- ✓ 3-command pipeline
- ✓ 10-command pipeline
- ✓ FD leak check

**Routing**:
- ✓ Builtin without pipe
- ✓ Builtin in pipe
- ✓ External command
- ✓ cd changes parent dir

---

## 🐛 Common Issues & Memory/FD Management

### ⚠️ CRITICAL: Zero Tolerance Rules

**NO MEMORY LEAKS - EVER**
- Every malloc() MUST have a corresponding free()
- Verify with: `valgrind --leak-check=full ./minishell`

**NO FD LEAKS - EVER**
- Every open/pipe/dup MUST have a close()
- Verify with: `valgrind --track-fds=yes ./minishell`
- Check with: `lsof -p <pid>`

### Common Issues

#### 1. FD Leaks in Pipes
**Problem**: Pipes not closed → process hangs or FD exhaustion  
**Solution**: 
- Close ALL unused pipe ends immediately
- Parent: close ALL pipes after forking all children
- Child: close ALL pipes except those needed
- Close pipe FD after dup2()

```c
// ✅ CORRECT
pipe(pipefd);
pid = fork();
if (pid == 0)
{
    close(pipefd[1]);           // Close unused write end
    dup2(pipefd[0], STDIN);
    close(pipefd[0]);           // Close after dup2
}
else
{
    close(pipefd[0]);           // Close unused read end
    close(pipefd[1]);           // Close write end when done
}
```

#### 2. Zombie Processes
**Problem**: Child processes not reaped → resource leak  
**Solution**: ALWAYS call waitpid() for ALL children

#### 3. Memory Before Execve
**Problem**: Allocated memory not freed before execve()  
**Solution**: Free ALL memory before execve (process gets replaced)

```c
// ✅ CORRECT
char **args = convert_args(cmd);
char *path = find_path(cmd);
// Free before exec (process will be replaced)
free(args);
free(path);
execve(path, args, env);
```

#### 4. PATH Search Leaks
**Problem**: Constructed paths not freed  
**Solution**: Free each attempted path, free split directories array

#### 5. Saved FDs Not Closed
**Problem**: dup() for saving FDs, but not closing saved FDs  
**Solution**: Always close saved FDs after restoration

```c
// ✅ CORRECT
saved_in = dup(STDIN);
// ... redirections ...
dup2(saved_in, STDIN);
close(saved_in);           // MUST close!
```

#### 6. Builtin in Pipe
**Problem**: `export` in pipe doesn't affect parent  
**Solution**: This is correct! Pipes run in child, can't modify parent

### Verification
```bash
# Memory leaks
valgrind --leak-check=full ./minishell

# FD leaks
valgrind --track-fds=yes ./minishell

# Open FDs while running
lsof -p $(pgrep minishell)
```

See `/docs/MEMORY_FD_MANAGEMENT.md` for detailed guidelines.

---

## 📚 Related Modules

- **AST Executor** (`src/executer/execute_ast.c`): Calls command_router
- **Builtins** (`src/builtins/`): Builtin implementations
- **Environment** (`src/environment/`): Environment access
- **Redirections** (`src/executer/apply_redirections.c`): FD redirection

---

## ✅ Norminette Guidelines

**Function Limits**:
- Max 25 lines per function
- Max 4 parameters
- Max 5 variables

**Strategies**:
- Break complex logic into helpers
- Use structures for multiple params
- Extract conditions into functions

---

## 📖 Usage Example

```c
// In execute_ast.c
if (node->type == CMD)
{
    status = execute_command(node, shell, in_pipeline);
}
else if (node->type == PIPE)
{
    // Setup pipes
    pipes = create_pipes(cmd_count);
    
    // Fork for each command
    for each command
    {
        pid = create_process();
        if (pid == 0)
        {
            setup_pipe_fds(pipes, i, cmd_count);
            execute_command(node, shell, 1);
            exit(status);
        }
    }
    
    // Close all pipes in parent
    close_all_pipes(pipes, cmd_count - 1);
    
    // Wait for all children
    status = wait_for_pipeline(pids, cmd_count);
}
```

---

**Status**: 🚧 In Development  
**Last Updated**: December 6, 2025
