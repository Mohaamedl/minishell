# 🔄 Minishell Detailed Workflow Diagram

**Project**: Minishell  
**Last Updated**: November 4, 2025

---

## 📋 Table of Contents

1. [High-Level Overview](#high-level-overview)
2. [Detailed Execution Flow](#detailed-execution-flow)
3. [Module Interaction Diagram](#module-interaction-diagram)
4. [Data Flow Through Components](#data-flow-through-components)
5. [Signal Handling Flow](#signal-handling-flow)
6. [Memory Management Flow](#memory-management-flow)
7. [Example Command Flows](#example-command-flows)

---

## 🎯 High-Level Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                         MINISHELL OVERVIEW                          │
└─────────────────────────────────────────────────────────────────────┘

┌──────────┐     ┌─────────┐     ┌─────────┐     ┌──────────┐     ┌─────────┐
│  INPUT   │────▶│  LEXER  │────▶│ PARSER  │────▶│ EXPANDER │────▶│ EXECUTOR│
│ readline │     │ tokenize│     │build AST│     │ expand $ │     │  exec   │
└──────────┘     └─────────┘     └─────────┘     └──────────┘     └─────────┘
     │                                                                    │
     │                                                                    │
     └───────────────────────────────┬────────────────────────────────────┘
                                     ▼
                              ┌────────────┐
                              │   OUTPUT   │
                              │  (stdout)  │
                              └────────────┘
```

---

## 🔍 Detailed Execution Flow

### Phase 1: Initialization (Startup)

```
┌─────────────────────────────────────────────────────────────────────┐
│                    PROGRAM INITIALIZATION                            │
└─────────────────────────────────────────────────────────────────────┘

main(int ac, char **av, char **envp)
  │
  ├─▶ [1] Initialize Environment
  │    │
  │    ├─▶ Parse envp[] array
  │    ├─▶ Create linked list (t_env)
  │    │    ┌────────────────────────┐
  │    │    │ t_env *head            │
  │    │    │  ├─▶ name: "PATH"      │
  │    │    │  ├─▶ value: "/usr/bin" │
  │    │    │  └─▶ next ────────────▶│
  │    │    └────────────────────────┘
  │    └─▶ Increment SHLVL
  │
  ├─▶ [2] Setup Signal Handlers
  │    │
  │    ├─▶ SIGINT (Ctrl+C)  → display new prompt
  │    ├─▶ SIGQUIT (Ctrl+\) → ignore
  │    └─▶ EOF (Ctrl+D)     → exit shell
  │
  ├─▶ [3] Initialize Shell State
  │    │
  │    ├─▶ last_exit_status = 0
  │    └─▶ history_enabled = true
  │
  └─▶ [4] Enter Main Loop (REPL)
       │
       └─▶ Continue to Phase 2...
```

### Phase 2: Main Loop (REPL - Read-Eval-Print Loop)

```
┌─────────────────────────────────────────────────────────────────────┐
│                        MAIN REPL LOOP                                │
└─────────────────────────────────────────────────────────────────────┘

while (1)
  │
  ├─▶ [1] DISPLAY PROMPT
  │    │
  │    └─▶ print "minishell$ "
  │
  ├─▶ [2] READ INPUT
  │    │
  │    ├─▶ line = readline("minishell$ ")
  │    │
  │    ├─▶ if (line == NULL)           // Ctrl+D pressed
  │    │    └─▶ cleanup_and_exit()
  │    │
  │    ├─▶ if (line[0] == '\0')        // Empty line
  │    │    └─▶ free(line); continue;
  │    │
  │    └─▶ add_history(line)           // Add to history
  │
  ├─▶ [3] TOKENIZE (Lexer)
  │    │
  │    └─▶ tokens = tokenize(line)
  │         │
  │         └─▶ Continue to Phase 3...
  │
  ├─▶ [4] PARSE (Parser)
  │    │
  │    └─▶ ast = parse(tokens)
  │         │
  │         └─▶ Continue to Phase 4...
  │
  ├─▶ [5] EXPAND (Expander)
  │    │
  │    └─▶ expand_variables(ast)
  │         │
  │         └─▶ Continue to Phase 5...
  │
  ├─▶ [6] EXECUTE (Executor)
  │    │
  │    └─▶ last_exit_status = execute_ast(ast)
  │         │
  │         └─▶ Continue to Phase 6...
  │
  ├─▶ [7] CLEANUP
  │    │
  │    ├─▶ free_tokens(tokens)
  │    ├─▶ free_ast(ast)
  │    └─▶ free(line)
  │
  └─▶ [8] LOOP BACK
       │
       └─▶ Go to step [1]
```

---

## 📝 Phase 3: Lexical Analysis (Tokenization)

```
┌─────────────────────────────────────────────────────────────────────┐
│                    LEXER - TOKENIZATION                              │
└─────────────────────────────────────────────────────────────────────┘

INPUT: "echo hello | grep h > out"
  │
  ├─▶ [1] Initialize
  │    │
  │    └─▶ token_list = NULL
  │
  ├─▶ [2] Character-by-Character Scanning
  │    │
  │    ├─▶ Skip whitespace
  │    │
  │    ├─▶ Check for QUOTES (' or ")
  │    │    │
  │    │    ├─▶ If quote found:
  │    │    │    ├─▶ Find matching closing quote
  │    │    │    ├─▶ Extract content (preserve spaces)
  │    │    │    └─▶ Create TOKEN_WORD
  │    │    │
  │    │    └─▶ Error if unclosed quote
  │    │
  │    ├─▶ Check for OPERATORS
  │    │    │
  │    │    ├─▶ | → TOKEN_PIPE
  │    │    ├─▶ < → TOKEN_REDIR_IN
  │    │    ├─▶ > → TOKEN_REDIR_OUT
  │    │    ├─▶ << → TOKEN_REDIR_HEREDOC
  │    │    ├─▶ >> → TOKEN_REDIR_APPEND
  │    │    ├─▶ && → TOKEN_AND (bonus)
  │    │    └─▶ || → TOKEN_OR (bonus)
  │    │
  │    └─▶ Extract WORDS
  │         │
  │         ├─▶ Read until whitespace/operator
  │         └─▶ Create TOKEN_WORD
  │
  └─▶ [3] Build Token Linked List

OUTPUT: Token List
┌──────────────┬────────────────┐
│ Type         │ Value          │
├──────────────┼────────────────┤
│ TOKEN_WORD   │ "echo"         │
│ TOKEN_WORD   │ "hello"        │
│ TOKEN_PIPE   │ "|"            │
│ TOKEN_WORD   │ "grep"         │
│ TOKEN_WORD   │ "h"            │
│ TOKEN_REDIR  │ ">"            │
│ TOKEN_WORD   │ "out"          │
│ TOKEN_EOF    │ NULL           │
└──────────────┴────────────────┘
```

---

## 🌳 Phase 4: Syntax Analysis (Parsing)

```
┌─────────────────────────────────────────────────────────────────────┐
│                    PARSER - BUILD AST                                │
└─────────────────────────────────────────────────────────────────────┘

INPUT: Token List
  │
  ├─▶ [1] Syntax Validation
  │    │
  │    ├─▶ Check: Cannot start with pipe
  │    ├─▶ Check: Cannot end with operator
  │    ├─▶ Check: Redir must have filename
  │    └─▶ Check: No consecutive operators
  │
  ├─▶ [2] Build AST (Abstract Syntax Tree)
  │    │
  │    │   Grammar:
  │    │   command_line → pipeline (('&&' | '||') pipeline)*
  │    │   pipeline     → simple_cmd ('|' simple_cmd)*
  │    │   simple_cmd   → redirections* WORD args* redirections*
  │    │
  │    ├─▶ Parse Logical Operators (&&, ||)  [Lowest precedence]
  │    │
  │    ├─▶ Parse Pipes (|)                   [Medium precedence]
  │    │
  │    └─▶ Parse Simple Commands             [Highest precedence]
  │         │
  │         ├─▶ Extract command name
  │         ├─▶ Extract arguments
  │         └─▶ Attach redirections
  │
  └─▶ [3] Return AST Root Node

EXAMPLE: "echo hello | grep h > out"

AST Structure:
                    PIPE
                   /    \
                  /      \
            COMMAND      COMMAND
            /           /      \
       "echo"      "grep"    REDIR(>)
          |           |           |
      "hello"        "h"       "out"

Detailed Node Structure:
┌────────────────────────────┐
│ NODE_TYPE: PIPE            │
│ ┌─────────────────────────┐│
│ │ LEFT:                   ││
│ │   type: COMMAND         ││
│ │   args: ["echo","hello"]││
│ │   redirs: NULL          ││
│ └─────────────────────────┘│
│ ┌─────────────────────────┐│
│ │ RIGHT:                  ││
│ │   type: COMMAND         ││
│ │   args: ["grep", "h"]   ││
│ │   redirs: [>out]        ││
│ └─────────────────────────┘│
└────────────────────────────┘
```

---

## 💰 Phase 5: Variable Expansion

```
┌─────────────────────────────────────────────────────────────────────┐
│                    EXPANDER - VARIABLE EXPANSION                     │
└─────────────────────────────────────────────────────────────────────┘

INPUT: AST with unexpanded variables
  │
  ├─▶ [1] Traverse AST
  │    │
  │    └─▶ For each command node:
  │
  ├─▶ [2] Process Each Argument
  │    │
  │    ├─▶ Scan for '$' character
  │    │
  │    ├─▶ if '$' found:
  │    │    │
  │    │    ├─▶ [A] Special Variable: $?
  │    │    │    │
  │    │    │    ├─▶ Get last_exit_status
  │    │    │    ├─▶ Convert to string
  │    │    │    └─▶ Replace "$?" with status
  │    │    │
  │    │    └─▶ [B] Environment Variable: $VAR
  │    │         │
  │    │         ├─▶ Extract variable name
  │    │         │    (alphanumeric + underscore)
  │    │         │
  │    │         ├─▶ Lookup in environment
  │    │         │    │
  │    │         │    ├─▶ If found: get value
  │    │         │    └─▶ If not found: empty string
  │    │         │
  │    │         └─▶ Replace $VAR with value
  │    │
  │    └─▶ Quote Handling:
  │         │
  │         ├─▶ Single quotes ('): NO expansion
  │         │    'echo $USER' → "echo $USER"
  │         │
  │         └─▶ Double quotes ("): YES expansion
  │              "echo $USER" → "echo john"
  │
  └─▶ [3] Remove Quotes
       │
       └─▶ Strip quote characters from final strings

EXAMPLE TRANSFORMATIONS:

Before Expansion:
  args: ["echo", "$USER", "$?", "'$HOME'"]
  env: USER=john, HOME=/home/john
  last_status: 0

After Expansion:
  args: ["echo", "john", "0", "$HOME"]
                  ↑      ↑       ↑
               expanded  $?   literal (single quotes)
```

---

## ⚙️ Phase 6: Execution Engine

```
┌─────────────────────────────────────────────────────────────────────┐
│                    EXECUTOR - COMMAND EXECUTION                      │
└─────────────────────────────────────────────────────────────────────┘

INPUT: Expanded AST
  │
  └─▶ execute_ast(node)
       │
       ├─▶ [1] Determine Node Type
       │    │
       │    ├─▶ NODE_COMMAND   → Execute simple command
       │    ├─▶ NODE_PIPE      → Execute pipeline
       │    ├─▶ NODE_AND       → Execute && logic (bonus)
       │    └─▶ NODE_OR        → Execute || logic (bonus)
       │
       └─▶ [2] Route to appropriate executor
            │
            ├──────────────────────────────────────────────┐
            │                                              │
            ▼                                              ▼
    ┌──────────────────┐                        ┌──────────────────┐
    │  SIMPLE COMMAND  │                        │    PIPELINE      │
    └──────────────────┘                        └──────────────────┘
            │                                              │
            └─▶ Continue below...                         └─▶ See Pipeline section
```

### 6.1 Simple Command Execution

```
┌─────────────────────────────────────────────────────────────────────┐
│                    SIMPLE COMMAND EXECUTION                          │
└─────────────────────────────────────────────────────────────────────┘

execute_command(node)
  │
  ├─▶ [1] Check Command Type
  │    │
  │    ├─▶ Is Built-in?
  │    │    │
  │    │    ├─▶ echo, cd, pwd, export, unset, env, exit
  │    │    │
  │    │    └─▶ YES ─────────┐
  │    │                     │
  │    └─▶ External?         │
  │         │                │
  │         └─▶ ls, cat, etc.│
  │                          │
  │         ┌────────────────┘
  │         ▼
  ├─▶ [2] Decision: Fork or Not?
  │    │
  │    ├─▶ Built-in WITHOUT pipes → NO FORK (run in parent)
  │    │    │
  │    │    └─▶ Why? cd must change parent's directory
  │    │             export must modify parent's env
  │    │             exit must exit parent shell
  │    │
  │    └─▶ Built-in WITH pipes → FORK (run in child)
  │    └─▶ External command   → FORK (always)
  │
  ├─▶ [3] Setup Redirections
  │    │
  │    └─▶ For each redirection:
  │         │
  │         ├─▶ <  : open(file, O_RDONLY)
  │         │       dup2(fd, STDIN_FILENO)
  │         │
  │         ├─▶ >  : open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644)
  │         │       dup2(fd, STDOUT_FILENO)
  │         │
  │         ├─▶ >> : open(file, O_WRONLY|O_CREAT|O_APPEND, 0644)
  │         │       dup2(fd, STDOUT_FILENO)
  │         │
  │         └─▶ << : create_heredoc_pipe()
  │                  dup2(pipe_read, STDIN_FILENO)
  │
  ├─▶ [4] Execute
  │    │
  │    ├─▶ Built-in: call function directly
  │    │    │
  │    │    └─▶ return exit_status
  │    │
  │    └─▶ External: fork and exec
  │         │
  │         ├─▶ pid = fork()
  │         │
  │         ├─▶ if (child):
  │         │    │
  │         │    ├─▶ Search PATH for command
  │         │    ├─▶ Convert env list to array
  │         │    ├─▶ execve(path, args, env)
  │         │    └─▶ exit(127) if execve fails
  │         │
  │         └─▶ if (parent):
  │              │
  │              ├─▶ waitpid(pid, &status, 0)
  │              └─▶ return WEXITSTATUS(status)
  │
  └─▶ [5] Update Exit Status
       │
       └─▶ last_exit_status = result

BUILT-IN EXECUTION FLOW:

builtin_cd(args, env)
  │
  ├─▶ Get target directory
  │    ├─▶ No args? → HOME
  │    └─▶ Has arg? → use it
  │
  ├─▶ chdir(target)
  │    └─▶ if error: return 1
  │
  ├─▶ Update PWD
  │    ├─▶ OLDPWD = current PWD
  │    └─▶ PWD = getcwd()
  │
  └─▶ return 0

EXTERNAL COMMAND FLOW:

execute_external(["ls", "-la"])
  │
  ├─▶ Search in PATH
  │    │
  │    ├─▶ Get PATH from env
  │    ├─▶ Split by ':'
  │    │    PATH="/usr/bin:/bin"
  │    │    → ["/usr/bin", "/bin"]
  │    │
  │    └─▶ For each directory:
  │         │
  │         ├─▶ Try: /usr/bin/ls
  │         ├─▶ access(path, X_OK)
  │         └─▶ if accessible: found!
  │
  ├─▶ Fork process
  │    │
  │    └─▶ pid = fork()
  │
  ├─▶ Child process:
  │    │
  │    ├─▶ execve("/usr/bin/ls", ["-la"], env)
  │    └─▶ exit(127)  // if execve fails
  │
  └─▶ Parent process:
       │
       ├─▶ waitpid(pid, &status, 0)
       └─▶ return exit_status
```

### 6.2 Pipeline Execution

```
┌─────────────────────────────────────────────────────────────────────┐
│                    PIPELINE EXECUTION                                │
└─────────────────────────────────────────────────────────────────────┘

EXAMPLE: "ls -l | grep txt | wc -l"

execute_pipeline(node)
  │
  ├─▶ [1] Count Commands
  │    │
  │    └─▶ num_commands = 3  (ls, grep, wc)
  │
  ├─▶ [2] Create Pipes
  │    │
  │    │   Need: num_commands - 1 = 2 pipes
  │    │
  │    ├─▶ pipe(pipe1)  // for ls → grep
  │    └─▶ pipe(pipe2)  // for grep → wc
  │
  │         ┌──────┐  pipe1  ┌──────┐  pipe2  ┌──────┐
  │         │  ls  │ ──────→ │ grep │ ──────→ │  wc  │
  │         └──────┘         └──────┘         └──────┘
  │
  ├─▶ [3] Fork Each Command
  │    │
  │    ├─▶ Command 1: ls -l
  │    │    │
  │    │    ├─▶ pid1 = fork()
  │    │    │
  │    │    └─▶ if (child):
  │    │         │
  │    │         ├─▶ Close unused pipe ends
  │    │         ├─▶ dup2(pipe1[WRITE], STDOUT)
  │    │         ├─▶ close all pipes
  │    │         └─▶ execve("ls", ["-l"], env)
  │    │
  │    ├─▶ Command 2: grep txt
  │    │    │
  │    │    ├─▶ pid2 = fork()
  │    │    │
  │    │    └─▶ if (child):
  │    │         │
  │    │         ├─▶ dup2(pipe1[READ], STDIN)
  │    │         ├─▶ dup2(pipe2[WRITE], STDOUT)
  │    │         ├─▶ close all pipes
  │    │         └─▶ execve("grep", ["txt"], env)
  │    │
  │    └─▶ Command 3: wc -l
  │         │
  │         ├─▶ pid3 = fork()
  │         │
  │         └─▶ if (child):
  │              │
  │              ├─▶ dup2(pipe2[READ], STDIN)
  │              ├─▶ close all pipes
  │              └─▶ execve("wc", ["-l"], env)
  │
  ├─▶ [4] Parent Closes All Pipes
  │    │
  │    ├─▶ close(pipe1[READ])
  │    ├─▶ close(pipe1[WRITE])
  │    ├─▶ close(pipe2[READ])
  │    └─▶ close(pipe2[WRITE])
  │
  ├─▶ [5] Wait for All Children
  │    │
  │    ├─▶ waitpid(pid1, &status1, 0)
  │    ├─▶ waitpid(pid2, &status2, 0)
  │    └─▶ waitpid(pid3, &status3, 0)
  │
  └─▶ [6] Return Status of Last Command
       │
       └─▶ return WEXITSTATUS(status3)

FILE DESCRIPTOR TABLE:

Initial state:
┌─────┬─────────┐
│ FD  │ Points  │
├─────┼─────────┤
│  0  │ stdin   │
│  1  │ stdout  │
│  2  │ stderr  │
└─────┴─────────┘

After pipe():
┌─────┬──────────┐
│ FD  │ Points   │
├─────┼──────────┤
│  0  │ stdin    │
│  1  │ stdout   │
│  2  │ stderr   │
│  3  │ pipe1[R] │
│  4  │ pipe1[W] │
│  5  │ pipe2[R] │
│  6  │ pipe2[W] │
└─────┴──────────┘

After dup2 in cmd1 (ls):
┌─────┬──────────┐
│ FD  │ Points   │
├─────┼──────────┤
│  0  │ stdin    │
│  1  │ pipe1[W] │ ← redirected
│  2  │ stderr   │
└─────┴──────────┘
```

### 6.3 Redirection Details

```
┌─────────────────────────────────────────────────────────────────────┐
│                    REDIRECTION HANDLING                              │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────┐
│  INPUT (<)           │
└──────────────────────┘

cat < input.txt
  │
  ├─▶ fd = open("input.txt", O_RDONLY)
  ├─▶ if (fd < 0): error
  ├─▶ dup2(fd, STDIN_FILENO)    // stdin now reads from file
  ├─▶ close(fd)
  └─▶ execute cat

┌──────────────────────┐
│  OUTPUT (>)          │
└──────────────────────┘

echo hello > output.txt
  │
  ├─▶ fd = open("output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)
  ├─▶ if (fd < 0): error
  ├─▶ dup2(fd, STDOUT_FILENO)   // stdout now writes to file
  ├─▶ close(fd)
  └─▶ execute echo

┌──────────────────────┐
│  APPEND (>>)         │
└──────────────────────┘

echo world >> output.txt
  │
  ├─▶ fd = open("output.txt", O_WRONLY|O_CREAT|O_APPEND, 0644)
  ├─▶ if (fd < 0): error
  ├─▶ dup2(fd, STDOUT_FILENO)
  ├─▶ close(fd)
  └─▶ execute echo

┌──────────────────────┐
│  HEREDOC (<<)        │
└──────────────────────┘

cat << EOF
  │
  ├─▶ Create pipe: pipe(pipefd)
  │
  ├─▶ Read lines until delimiter:
  │    │
  │    ├─▶ Display "> " prompt
  │    ├─▶ line = readline("> ")
  │    ├─▶ if (line == "EOF"): break
  │    ├─▶ Expand variables (if not quoted)
  │    └─▶ write(pipefd[WRITE], line)
  │
  ├─▶ close(pipefd[WRITE])
  ├─▶ dup2(pipefd[READ], STDIN_FILENO)
  ├─▶ close(pipefd[READ])
  └─▶ execute cat

MULTIPLE REDIRECTIONS:

cat < in1 < in2 > out1 > out2
  │
  │   Rule: Last one wins!
  │
  ├─▶ Process left to right:
  │    ├─▶ < in1  (stdin = in1)
  │    ├─▶ < in2  (stdin = in2)  ← overwrites
  │    ├─▶ > out1 (stdout = out1)
  │    └─▶ > out2 (stdout = out2) ← overwrites
  │
  └─▶ Final: stdin=in2, stdout=out2
```

---

## 🚦 Signal Handling Flow

```
┌─────────────────────────────────────────────────────────────────────┐
│                    SIGNAL HANDLING                                   │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────┐
│  SIGINT (Ctrl+C)     │
└──────────────────────┘

Context: Interactive prompt
  │
  User presses Ctrl+C
  │
  ├─▶ Signal handler triggered
  │    │
  │    ├─▶ Write "\n" to stdout
  │    ├─▶ rl_on_new_line()        // Move to new line
  │    ├─▶ rl_replace_line("", 0)  // Clear input
  │    └─▶ rl_redisplay()          // Show new prompt
  │
  └─▶ Return to prompt (don't exit)

Context: Command executing
  │
  User presses Ctrl+C
  │
  ├─▶ Child process receives SIGINT
  │    └─▶ Child terminates
  │
  ├─▶ Parent process waits
  │    │
  │    └─▶ waitpid() returns with signal status
  │         │
  │         └─▶ exit_status = 128 + SIGINT (130)
  │
  └─▶ Return to prompt

┌──────────────────────┐
│  SIGQUIT (Ctrl+\)    │
└──────────────────────┘

Interactive mode:
  │
  └─▶ Ignore (do nothing)

Command executing:
  │
  └─▶ Child may terminate with SIGQUIT

┌──────────────────────┐
│  EOF (Ctrl+D)        │
└──────────────────────┘

readline() returns NULL
  │
  ├─▶ Check if line is NULL
  │
  ├─▶ Print "exit\n"
  │
  ├─▶ Cleanup:
  │    ├─▶ free_environment()
  │    ├─▶ rl_clear_history()
  │    └─▶ close file descriptors
  │
  └─▶ exit(last_exit_status)

┌──────────────────────────────────────────┐
│  Signal Setup (at initialization)       │
└──────────────────────────────────────────┘

setup_signals()
  │
  ├─▶ struct sigaction sa
  │
  ├─▶ SIGINT:
  │    ├─▶ sa.sa_handler = handle_sigint
  │    ├─▶ sa.sa_flags = SA_RESTART
  │    └─▶ sigaction(SIGINT, &sa, NULL)
  │
  ├─▶ SIGQUIT:
  │    ├─▶ sa.sa_handler = SIG_IGN
  │    └─▶ sigaction(SIGQUIT, &sa, NULL)
  │
  └─▶ Signals ready
```

---

## 💾 Memory Management Flow

```
┌─────────────────────────────────────────────────────────────────────┐
│                    MEMORY MANAGEMENT                                 │
└─────────────────────────────────────────────────────────────────────┘

┌────────────────────────┐
│  ALLOCATION POINTS     │
└────────────────────────┘

[1] Environment List
    │
    ├─▶ malloc(sizeof(t_env)) for each env var
    └─▶ malloc for name and value strings

[2] Input Line
    │
    └─▶ readline() allocates (must free!)

[3] Tokens
    │
    ├─▶ malloc(sizeof(t_token)) for each token
    └─▶ malloc for token value strings

[4] AST Nodes
    │
    ├─▶ malloc(sizeof(t_ast_node)) for each node
    ├─▶ malloc for args array
    └─▶ malloc for redirection list

[5] Expanded Strings
    │
    └─▶ malloc during variable expansion

┌────────────────────────┐
│  CLEANUP SEQUENCE      │
└────────────────────────┘

Per Command Cycle:
  │
  ├─▶ [1] Free line
  │    └─▶ free(line)
  │
  ├─▶ [2] Free tokens
  │    │
  │    └─▶ free_tokens(tokens)
  │         │
  │         ├─▶ while (token):
  │         │    ├─▶ free(token->value)
  │         │    ├─▶ temp = token
  │         │    ├─▶ token = token->next
  │         │    └─▶ free(temp)
  │         │
  │         └─▶ tokens = NULL
  │
  ├─▶ [3] Free AST
  │    │
  │    └─▶ free_ast(ast)
  │         │
  │         ├─▶ if (node->left): free_ast(left)
  │         ├─▶ if (node->right): free_ast(right)
  │         ├─▶ free_array(node->args)
  │         ├─▶ free_redirections(node->redirs)
  │         └─▶ free(node)
  │
  └─▶ Ready for next command

On Exit:
  │
  ├─▶ [1] Free environment
  │    │
  │    └─▶ free_environment(env)
  │         │
  │         ├─▶ while (env):
  │         │    ├─▶ free(env->name)
  │         │    ├─▶ free(env->value)
  │         │    ├─▶ temp = env
  │         │    ├─▶ env = env->next
  │         │    └─▶ free(temp)
  │         │
  │         └─▶ env = NULL
  │
  ├─▶ [2] Clear history
  │    └─▶ rl_clear_history()
  │
  ├─▶ [3] Close file descriptors
  │    └─▶ close any open fds
  │
  └─▶ exit(status)

┌────────────────────────┐
│  LEAK PREVENTION       │
└────────────────────────┘

[1] Always pair malloc with free
[2] Free in reverse order of allocation
[3] Set pointers to NULL after free
[4] Use valgrind to detect leaks
[5] Free on error paths too!

Error Handling Pattern:
  │
  ├─▶ ptr = malloc(size)
  ├─▶ if (!ptr):
  │    ├─▶ cleanup_partial()
  │    └─▶ return ERROR
  │
  └─▶ Use ptr...
```

---

## 📊 Example Command Flows

### Example 1: Simple Command

```
┌─────────────────────────────────────────────────────────────────────┐
│  COMMAND: echo hello world                                          │
└─────────────────────────────────────────────────────────────────────┘

minishell$ echo hello world
     │
     ├─▶ readline() → "echo hello world"
     │
     ├─▶ tokenize()
     │    └─▶ [WORD:"echo"] [WORD:"hello"] [WORD:"world"] [EOF]
     │
     ├─▶ parse()
     │    └─▶ AST: COMMAND { args: ["echo", "hello", "world"] }
     │
     ├─▶ expand()
     │    └─▶ No $ variables → no change
     │
     ├─▶ execute()
     │    │
     │    ├─▶ Is builtin? YES (echo)
     │    ├─▶ Has pipes? NO
     │    ├─▶ → Execute in parent (no fork)
     │    │
     │    └─▶ builtin_echo(["hello", "world"])
     │         │
     │         ├─▶ print "hello world\n"
     │         └─▶ return 0
     │
     ├─▶ last_exit_status = 0
     │
     └─▶ Display new prompt

OUTPUT: hello world
```

### Example 2: Pipeline with Redirection

```
┌─────────────────────────────────────────────────────────────────────┐
│  COMMAND: cat file.txt | grep hello | wc -l > count.txt            │
└─────────────────────────────────────────────────────────────────────┘

minishell$ cat file.txt | grep hello | wc -l > count.txt
     │
     ├─▶ readline() → "cat file.txt | grep hello | wc -l > count.txt"
     │
     ├─▶ tokenize()
     │    └─▶ [WORD:"cat"] [WORD:"file.txt"] [PIPE]
     │        [WORD:"grep"] [WORD:"hello"] [PIPE]
     │        [WORD:"wc"] [WORD:"-l"] [REDIR_OUT] [WORD:"count.txt"] [EOF]
     │
     ├─▶ parse()
     │    └─▶ AST:
     │              PIPE
     │             /    \
     │          PIPE    COMMAND(wc)
     │         /    \          \
     │    CMD(cat) CMD(grep)   REDIR(>count.txt)
     │
     ├─▶ expand()
     │    └─▶ No variables → no change
     │
     ├─▶ execute_pipeline()
     │    │
     │    ├─▶ Create 2 pipes (for 3 commands)
     │    │
     │    ├─▶ Fork Command 1: cat file.txt
     │    │    │
     │    │    ├─▶ dup2(pipe1[WRITE], STDOUT)
     │    │    └─▶ execve("cat", ["file.txt"])
     │    │
     │    ├─▶ Fork Command 2: grep hello
     │    │    │
     │    │    ├─▶ dup2(pipe1[READ], STDIN)
     │    │    ├─▶ dup2(pipe2[WRITE], STDOUT)
     │    │    └─▶ execve("grep", ["hello"])
     │    │
     │    ├─▶ Fork Command 3: wc -l
     │    │    │
     │    │    ├─▶ dup2(pipe2[READ], STDIN)
     │    │    ├─▶ Setup redirection:
     │    │    │    └─▶ fd = open("count.txt", O_WRONLY|O_CREAT|O_TRUNC)
     │    │    │    └─▶ dup2(fd, STDOUT)
     │    │    └─▶ execve("wc", ["-l"])
     │    │
     │    ├─▶ Close all pipes in parent
     │    │
     │    └─▶ Wait for all children
     │         └─▶ return status of wc
     │
     ├─▶ last_exit_status = 0
     │
     └─▶ Display new prompt

DATA FLOW:
file.txt → cat → pipe1 → grep → pipe2 → wc → count.txt
```

### Example 3: Variable Expansion

```
┌─────────────────────────────────────────────────────────────────────┐
│  COMMAND: echo "User: $USER, Status: $?"                            │
└─────────────────────────────────────────────────────────────────────┘

minishell$ echo "User: $USER, Status: $?"
     │
     │  Context: USER=john, last_exit_status=0
     │
     ├─▶ readline() → 'echo "User: $USER, Status: $?"'
     │
     ├─▶ tokenize()
     │    └─▶ [WORD:"echo"] [WORD:"User: $USER, Status: $?"] [EOF]
     │         Note: Quotes preserved in token value
     │
     ├─▶ parse()
     │    └─▶ AST: COMMAND { args: ["echo", "\"User: $USER, Status: $?\""] }
     │
     ├─▶ expand()
     │    │
     │    ├─▶ Process arg: "User: $USER, Status: $?"
     │    │
     │    ├─▶ Find $USER:
     │    │    ├─▶ In double quotes → expand
     │    │    ├─▶ Lookup USER in env → "john"
     │    │    └─▶ Replace: "User: john, Status: $?"
     │    │
     │    ├─▶ Find $?:
     │    │    ├─▶ Get last_exit_status → 0
     │    │    ├─▶ Convert to string → "0"
     │    │    └─▶ Replace: "User: john, Status: 0"
     │    │
     │    └─▶ Remove quotes: User: john, Status: 0
     │
     ├─▶ execute()
     │    │
     │    └─▶ builtin_echo(["User: john, Status: 0"])
     │         │
     │         ├─▶ print "User: john, Status: 0\n"
     │         └─▶ return 0
     │
     └─▶ Display new prompt

OUTPUT: User: john, Status: 0
```

### Example 4: Heredoc

```
┌─────────────────────────────────────────────────────────────────────┐
│  COMMAND: cat << EOF                                                 │
└─────────────────────────────────────────────────────────────────────┘

minishell$ cat << EOF
     │
     ├─▶ tokenize()
     │    └─▶ [WORD:"cat"] [HEREDOC:"<<"] [WORD:"EOF"] [EOF]
     │
     ├─▶ parse()
     │    └─▶ AST: COMMAND { cmd: "cat", redir: HEREDOC(delimiter="EOF") }
     │
     ├─▶ execute()
     │    │
     │    ├─▶ Setup heredoc:
     │    │    │
     │    │    ├─▶ pipe(pipefd)
     │    │    │
     │    │    ├─▶ prompt "> "
     │    │    ├─▶ line = readline("> ")
     │    │    │    User types: "Hello $USER"
     │    │    │
     │    │    ├─▶ while (line != "EOF"):
     │    │    │    │
     │    │    │    ├─▶ Expand variables (USER=john)
     │    │    │    │    "Hello $USER" → "Hello john"
     │    │    │    │
     │    │    │    ├─▶ write(pipefd[WRITE], "Hello john\n")
     │    │    │    │
     │    │    │    ├─▶ prompt "> "
     │    │    │    └─▶ line = readline("> ")
     │    │    │         User types: "EOF"
     │    │    │
     │    │    ├─▶ close(pipefd[WRITE])
     │    │    │
     │    │    ├─▶ dup2(pipefd[READ], STDIN)
     │    │    │
     │    │    └─▶ close(pipefd[READ])
     │    │
     │    └─▶ execve("cat", [])
     │         │
     │         └─▶ cat reads from redirected stdin (pipe)
     │
     └─▶ Display new prompt

INTERACTION:
minishell$ cat << EOF
> Hello $USER
> This is a test
> EOF
Hello john
This is a test
minishell$
```

---

## 🎨 Complete System Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         MINISHELL ARCHITECTURE                          │
└─────────────────────────────────────────────────────────────────────────┘

                              START
                                │
                                ▼
                         ┌─────────────┐
                         │  Initialize │
                         │  - env list │
                         │  - signals  │
                         └──────┬──────┘
                                │
                                ▼
                    ┌──────────────────────┐
                    │   Display Prompt     │◀────────────────┐
                    │  "minishell$ "       │                 │
                    └──────────┬───────────┘                 │
                               │                             │
                               ▼                             │
                    ┌──────────────────────┐                 │
                    │   Read Input         │                 │
                    │   readline()         │                 │
                    └──────────┬───────────┘                 │
                               │                             │
                          Empty/NULL?                        │
                          │         │                        │
                        YES         NO                       │
                          │         │                        │
                       Exit         │                        │
                                    ▼                        │
                         ┌────────────────────┐              │
                         │   LEXER            │              │
                         │   Tokenize input   │              │
                         │   Handle quotes    │              │
                         │   Identify ops     │              │
                         └─────────┬──────────┘              │
                                   │                         │
                                   ▼                         │
                         ┌────────────────────┐              │
                         │   PARSER           │              │
                         │   Syntax check     │              │
                         │   Build AST        │              │
                         │   Handle priority  │              │
                         └─────────┬──────────┘              │
                                   │                         │
                            Syntax Error?                    │
                              │       │                      │
                             YES      NO                     │
                              │       │                      │
                          Print err   │                      │
                              │       ▼                      │
                              │  ┌────────────────────┐      │
                              │  │   EXPANDER         │      │
                              │  │   Expand $VAR      │      │
                              │  │   Expand $?        │      │
                              │  │   Remove quotes    │      │
                              │  └─────────┬──────────┘      │
                              │            │                 │
                              │            ▼                 │
                              │  ┌────────────────────┐      │
                              │  │   EXECUTOR         │      │
                              │  │   Route command    │      │
                              │  │   Setup redirs     │      │
                              │  │   Fork/exec        │      │
                              │  │   Wait/collect     │      │
                              │  └─────────┬──────────┘      │
                              │            │                 │
                              │            ▼                 │
                              │  ┌────────────────────┐      │
                              │  │   Update Status    │      │
                              │  │   last_exit_status │      │
                              │  └─────────┬──────────┘      │
                              │            │                 │
                              └────────────┤                 │
                                           │                 │
                                           ▼                 │
                                    ┌─────────────┐          │
                                    │   Cleanup   │          │
                                    │   Free mem  │          │
                                    └──────┬──────┘          │
                                           │                 │
                                           └─────────────────┘
                                              Loop back
```

---

## 📝 Summary Checklist

### Execution Path for Any Command

- [ ] **Input**: User types command
- [ ] **Read**: readline() gets the line
- [ ] **History**: add_history() if not empty
- [ ] **Tokenize**: Break into tokens (WORD, PIPE, REDIR, etc.)
- [ ] **Parse**: Build AST from tokens
- [ ] **Validate**: Check syntax rules
- [ ] **Expand**: Replace $VAR and $?
- [ ] **Route**: Built-in or external?
- [ ] **Redirect**: Setup < > >> <<
- [ ] **Fork**: Create child process (if needed)
- [ ] **Execute**: Run command
- [ ] **Wait**: Collect exit status
- [ ] **Update**: Set last_exit_status
- [ ] **Cleanup**: Free all allocated memory
- [ ] **Loop**: Back to prompt

---

**Document Version**: 1.0  
**Last Updated**: November 4, 2025  
**For**: Minishell Implementation Team
