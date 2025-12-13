# 🚀 External Command Execution - Implementation Plan

**Issues**: KAN-52 to KAN-57  
**Branch**: `feature/KAN-52-57-command-execution`  
**Epic**: Command Execution & Process Management  
**Story Points**: 47 total  
**Date**: December 6, 2025

---

## 📋 Issues Overview

| Issue | Title | Points | Priority | Dependencies |
|-------|-------|--------|----------|--------------|
| **KAN-52** | Pipe Creation and Management | 13 | HIGH | KAN-42 (Parser) |
| **KAN-53** | Process Creation (fork) | 8 | HIGH | - |
| **KAN-54** | Command Execution (execve) | 8 | HIGH | KAN-24, KAN-20 |
| **KAN-55** | Process Waiting & Status | 5 | HIGH | KAN-53 |
| **KAN-56** | Command Routing | 5 | HIGH | KAN-30-36, KAN-53-54 |
| **KAN-57** | Main REPL Loop | 8 | HIGH | KAN-38, KAN-42, KAN-45, KAN-56 |

**Total Story Points**: 47

---

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                     MAIN REPL LOOP                          │
│                     (KAN-57)                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ readline() → tokenize() → parse() → expand()        │   │
│  │              ↓                                       │   │
│  │         COMMAND ROUTER (KAN-56)                     │   │
│  └─────────────────────────────────────────────────────┘   │
│              ↓                           ↓                  │
│      ┌──────────────┐           ┌──────────────────┐       │
│      │   BUILTIN    │           │    EXTERNAL      │       │
│      │  (no fork)   │           │  (fork+execve)   │       │
│      └──────────────┘           └──────────────────┘       │
│                                      ↓                      │
│              ┌──────────────────────────────────┐           │
│              │  PROCESS MANAGEMENT              │           │
│              │  - fork() (KAN-53)               │           │
│              │  - execve() (KAN-54)             │           │
│              │  - waitpid() (KAN-55)            │           │
│              │  - pipe() (KAN-52)               │           │
│              └──────────────────────────────────┘           │
└─────────────────────────────────────────────────────────────┘
```

---

## 📁 Project Structure

### New Files to Create

```
src/
├── executor/
│   ├── pipes.c              # KAN-52: Pipe management
│   ├── process.c            # KAN-53: Fork wrapper
│   ├── exec.c               # KAN-54: execve + PATH search
│   ├── wait.c               # KAN-55: waitpid wrapper
│   └── command_router.c     # KAN-56: Route to builtin/external
├── main.c                   # KAN-57: Update REPL loop
└── executer/
    ├── execute_ast.c        # Already exists - integrate
    └── apply_redirections.c # Already exists
```

### Headers to Update

```
include/
└── minishell.h              # Add prototypes for new functions
```

---

## 🎯 Implementation Strategy

### Phase 1: Foundation (KAN-53, KAN-54, KAN-55)
**Goal**: Basic external command execution

1. **KAN-53: Process Creation**
   - File: `src/executor/process.c`
   - Function: `pid_t create_process(void)`
   - Simple fork() wrapper with error handling
   - Tests: fork success, fork failure

2. **KAN-54: Command Execution**
   - File: `src/executor/exec.c`
   - Functions:
     - `char *find_command_in_path(char *cmd, t_env *env)`
     - `int execute_external_cmd(char **args, t_shell *shell)`
   - PATH search algorithm
   - execve() with environment array
   - Error codes: 127 (not found), 126 (permission denied)
   - Tests: absolute paths, relative paths, PATH search

3. **KAN-55: Process Waiting**
   - File: `src/executor/wait.c`
   - Functions:
     - `int wait_for_process(pid_t pid)`
     - `int wait_for_pipeline(pid_t *pids, int count)`
   - Exit status collection (WIFEXITED, WIFSIGNALED)
   - Signal handling (128 + signal)
   - Tests: normal exit, signal termination

### Phase 2: Pipes (KAN-52)
**Goal**: Pipeline support

4. **KAN-52: Pipe Management**
   - File: `src/executor/pipes.c`
   - Functions:
     - `int *create_pipes(int num_commands)`
     - `void setup_pipe_fds(int *pipes, int index, int total)`
     - `void close_all_pipes(int *pipes, int count)`
   - Algorithm: (n-1) pipes for n commands
   - FD management: close unused pipes
   - Tests: 2-cmd pipeline, 3-cmd pipeline, 10-cmd pipeline

### Phase 3: Integration (KAN-56, KAN-57)
**Goal**: Complete working shell

5. **KAN-56: Command Router**
   - File: `src/executor/command_router.c`
   - Function: `int execute_command(t_ast *node, t_shell *shell, int in_pipe)`
   - Decision logic:
     ```c
     if (is_builtin(cmd)) {
         if (in_pipe) fork(); // Builtins in pipes need fork
         execute_builtin();
     } else {
         fork();
         execute_external();
     }
     ```
   - Tests: builtins without pipes, builtins in pipes, external commands

6. **KAN-57: Main REPL Loop**
   - File: `src/main.c`
   - Update main() function
   - Flow: Read → Parse → Expand → Execute → Update status → Free
   - Signal handling integration
   - History management (add_history)
   - Tests: interactive mode, command execution, Ctrl+C, Ctrl+D

---

## 🔧 Technical Details

### KAN-52: Pipe Structure

For `cmd1 | cmd2 | cmd3`:
```
Pipes needed: 2 (n-1)
pipe[0]: cmd1 → cmd2
pipe[1]: cmd2 → cmd3

cmd1: stdout → pipe[0][1] (write)
cmd2: stdin ← pipe[0][0] (read)
      stdout → pipe[1][1] (write)
cmd3: stdin ← pipe[1][0] (read)
```

### KAN-54: PATH Search Algorithm

```c
1. If cmd contains '/': use as-is (absolute/relative)
2. Else:
   a. Get PATH from environment
   b. Split by ':'
   c. For each directory:
      - Construct full_path = dir + "/" + cmd
      - Check access(full_path, X_OK)
      - If found: return full_path
   d. If not found: return NULL (command not found)
```

### KAN-56: Builtin vs External Decision

```c
Built-ins that MUST run in parent (no fork):
- cd (changes parent's directory)
- export (modifies parent's environment)
- unset (modifies parent's environment)
- exit (exits parent shell)

Exception: In pipes, ALL commands (including builtins) run in child
```

---

## ✅ Testing Strategy

### Unit Tests

Create `tests/executor/`:
```bash
tests/executor/
├── test_fork.sh           # KAN-53
├── test_execve.sh         # KAN-54
├── test_wait.sh           # KAN-55
├── test_pipes.sh          # KAN-52
├── test_routing.sh        # KAN-56
└── test_repl.sh           # KAN-57
```

### Test Cases

**KAN-52 (Pipes)**:
- [ ] `echo hello | cat`
- [ ] `ls | grep txt`
- [ ] `cat file | grep test | wc -l`
- [ ] 10-command pipeline
- [ ] Check FD leaks (lsof)

**KAN-53 (Fork)**:
- [ ] Fork success
- [ ] Fork failure handling
- [ ] Child process isolation

**KAN-54 (Execve)**:
- [ ] `/bin/ls` (absolute)
- [ ] `ls` (PATH search)
- [ ] `./script` (relative)
- [ ] Command not found → 127
- [ ] Permission denied → 126

**KAN-55 (Wait)**:
- [ ] Normal exit (exit 0)
- [ ] Error exit (exit 1)
- [ ] Signal termination (Ctrl+C)
- [ ] Pipeline wait for all children

**KAN-56 (Routing)**:
- [ ] `echo test` (builtin, no fork)
- [ ] `echo test | cat` (builtin in pipe, fork)
- [ ] `ls` (external, fork)
- [ ] `cd /tmp` (builtin, no fork, changes parent)

**KAN-57 (REPL)**:
- [ ] Interactive mode
- [ ] Ctrl+C → new prompt
- [ ] Ctrl+D → exit
- [ ] History navigation
- [ ] Multiple commands in session

---

## 📝 Norminette Compliance

### Rules to Follow

1. **Function length**: Max 25 lines
2. **Line length**: Max 80 characters
3. **Parameters**: Max 4 per function
4. **Variables**: Max 5 per function
5. **No for loops**: Use while
6. **Declarations**: At function start

### Strategies

- Break large functions into helpers
- Use structures to pass multiple parameters
- Extract logic into separate functions
- Use helper functions for complex conditions

---

## 🔄 Git Workflow

### Branch Strategy

```bash
# Create feature branch
git checkout -b feature/KAN-52-57-command-execution

# Work in small commits
git commit -m "feat(executor): implement pipe management (KAN-52)"
git commit -m "feat(executor): implement fork wrapper (KAN-53)"
git commit -m "feat(executor): implement execve with PATH search (KAN-54)"
git commit -m "feat(executor): implement process waiting (KAN-55)"
git commit -m "feat(executor): implement command routing (KAN-56)"
git commit -m "feat(main): update REPL loop (KAN-57)"
git commit -m "test(executor): add comprehensive tests"
git commit -m "docs: update documentation for execution module"

# Push and create PR
git push -u origin feature/KAN-52-57-command-execution
```

### Commit Convention

```
feat(scope): description (KAN-XX)
test(scope): description
docs(scope): description
fix(scope): description
refactor(scope): description
```

---

## 📊 Progress Tracking

### Milestones

- [ ] **M1**: Foundation complete (KAN-53, 54, 55) - Basic external commands work
- [ ] **M2**: Pipes complete (KAN-52) - Pipelines work
- [ ] **M3**: Integration complete (KAN-56, 57) - Full shell works
- [ ] **M4**: Tests complete - 100% coverage
- [ ] **M5**: Norminette clean - No errors
- [ ] **M6**: PR created and reviewed

### Definition of Done

✅ Code compiles without warnings  
✅ All tests pass  
✅ Norminette compliant  
✅ No memory leaks (valgrind)  
✅ Documentation updated  
✅ Code reviewed  
✅ Jira issues updated  

---

## 🎓 Learning Resources

### System Calls Reference

- `fork()`: Creates child process
- `execve()`: Replaces process with new program
- `waitpid()`: Waits for child process
- `pipe()`: Creates pipe for IPC
- `dup2()`: Duplicates file descriptor
- `access()`: Checks file permissions

### Key Concepts

1. **Process Management**: fork-exec-wait pattern
2. **File Descriptors**: stdin(0), stdout(1), stderr(2)
3. **Pipe Communication**: One-way data flow
4. **Exit Status**: WIFEXITED, WEXITSTATUS, WIFSIGNALED
5. **PATH Search**: Environment variable parsing

---

## 🚨 Common Pitfalls & CRITICAL RULES

### ⚠️ **ZERO TOLERANCE RULES**

1. **NO MEMORY LEAKS - EVER**
   - Every malloc() MUST have a corresponding free()
   - Free ALL allocated memory before exit/execve
   - Use valgrind to verify: `valgrind --leak-check=full ./minishell`
   - Check with: `valgrind --track-fds=yes` for FD leaks

2. **NO OPEN FILE DESCRIPTORS - EVER**
   - Every open/pipe/dup MUST be closed
   - Close ALL pipe ends after use
   - Close saved FDs after restoration
   - Parent MUST close all pipe FDs after forking children
   - Child MUST close unused pipe ends before exec
   - Use `lsof -p <pid>` to verify open FDs

### Common Pitfalls

3. **FD Leaks in Pipes**: 
   - ALWAYS close unused pipe ends immediately
   - Parent: close ALL pipes after all children forked
   - Child: close ALL pipes except those needed
   
4. **Zombie Processes**: 
   - ALWAYS waitpid() for ALL children
   - Track all PIDs and wait for each

5. **Memory Before Execve**: 
   - Free ALL memory before execve() (process will be replaced)
   - Don't leak in child process

6. **Signal Handling**: 
   - Parent vs child signal behavior differs

7. **PATH Search**: 
   - Free split PATH array
   - Free constructed paths
   - Handle empty PATH, missing directories

8. **Builtins in Pipes**: 
   - Must fork for builtins in pipeline

---

## 📅 Timeline Estimate

**Total**: ~3-4 days for complete implementation

- **Day 1**: KAN-53, 54, 55 (Foundation)
- **Day 2**: KAN-52 (Pipes) + Integration
- **Day 3**: KAN-56, 57 (Router + REPL)
- **Day 4**: Testing + Norminette + Documentation

---

**Status**: 📝 Planning Phase  
**Next Step**: Create feature branch and start KAN-53

