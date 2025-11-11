# 🏗️ Minishell Architecture Documentation

**Version**: 1.0  
**Last Updated**: November 3, 2025

---

## 📋 Table of Contents

1. [System Overview](#system-overview)
2. [High-Level Architecture](#high-level-architecture)
3. [Data Structures](#data-structures)
4. [Module Descriptions](#module-descriptions)
5. [Execution Flow](#execution-flow)
6. [Signal Handling](#signal-handling)
7. [Memory Management](#memory-management)
8. [Error Handling](#error-handling)

---

## 🎯 System Overview

Minishell is a simplified Unix shell implementation that provides:
- Command line interpretation and execution
- Process creation and management
- I/O redirection and piping
- Environment variable management
- Signal handling
- Built-in command execution

### Design Principles

1. **Modularity**: Each component is self-contained and has clear responsibilities
2. **Robustness**: Comprehensive error handling and memory management
3. **Maintainability**: Clean code structure following 42 Norm
4. **Testability**: Components can be tested independently
5. **Bash Compatibility**: Behavior matches bash where specified

---

## 🏛️ High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         USER INPUT                           │
│                      (via readline)                          │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                       MAIN LOOP                              │
│  • Display prompt                                            │
│  • Read input                                                │
│  • Add to history                                            │
│  • Check for empty/whitespace                                │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    LEXER (Tokenizer)                         │
│  • Identify operators: | < > << >> && || ( )                │
│  • Extract words (commands, arguments)                       │
│  • Handle quotes (single and double)                         │
│  • Create token linked list                                  │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    PARSER                                    │
│  • Build Abstract Syntax Tree (AST)                          │
│  • Parse logical operators (&& ||) [BONUS]                   │
│  • Parse pipelines (|)                                       │
│  • Parse redirections (< > << >>)                            │
│  • Parse simple commands                                     │
│  • Validate syntax                                           │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    EXPANDER                                  │
│  • Expand environment variables ($VAR)                       │
│  • Expand exit status ($?)                                   │
│  • Handle quote context                                      │
│  • Wildcard expansion (*) [BONUS]                            │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    EXECUTOR                                  │
│  • Determine execution type:                                 │
│    - Built-in command                                        │
│    - Simple external command                                 │
│    - Pipeline                                                │
│    - Logical expression [BONUS]                              │
│  • Set up redirections                                       │
│  • Execute command(s)                                        │
│  • Wait for processes                                        │
│  • Update exit status                                        │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    OUTPUT / RESULT                           │
│  • Command output to stdout/stderr                           │
│  • Exit status stored in shell context                       │
│  • Return to main loop                                       │
└─────────────────────────────────────────────────────────────┘
```

---

## 📦 Data Structures

### Core Shell Context

```c
typedef struct s_shell
{
    t_env       *env_list;          // Environment variables (linked list)
    char        **env_array;        // Environment array for execve
    char        *cwd;               // Current working directory
    int         last_exit_status;   // Exit status of last command ($?)
    int         is_interactive;     // Interactive mode flag
    t_process   *child_processes;   // Tracked child processes
}   t_shell;
```

### Environment Variable

```c
typedef struct s_env
{
    char        *key;               // Variable name
    char        *value;             // Variable value
    struct s_env *next;             // Next in linked list
}   t_env;
```

### Token Structure

```c
typedef enum e_token_type
{
    TOKEN_WORD,                     // Command, argument, or filename
    TOKEN_PIPE,                     // |
    TOKEN_REDIRECT_IN,              // <
    TOKEN_REDIRECT_OUT,             // >
    TOKEN_REDIRECT_APPEND,          // >>
    TOKEN_HEREDOC,                  // <<
    TOKEN_AND,                      // && [BONUS]
    TOKEN_OR,                       // || [BONUS]
    TOKEN_LPAREN,                   // ( [BONUS]
    TOKEN_RPAREN,                   // ) [BONUS]
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;           // Token type
    char            *value;         // Token string value
    struct s_token  *next;          // Next token in list
}   t_token;
```

### Command Structure

```c
typedef struct s_redirect
{
    t_token_type    type;           // Redirection type
    char            *file;          // Filename or heredoc delimiter
    int             fd;             // File descriptor (for heredoc)
    struct s_redirect *next;        // Next redirection
}   t_redirect;

typedef struct s_command
{
    char        **args;             // NULL-terminated argument array
    t_redirect  *redirects;         // List of redirections
    int         is_builtin;         // Built-in command flag
}   t_command;
```

### Pipeline Structure

```c
typedef struct s_pipeline
{
    t_command   **commands;         // Array of commands
    int         cmd_count;          // Number of commands
    int         **pipes;            // Pipe file descriptors
}   t_pipeline;
```

### AST Node (for bonus logical operators)

```c
typedef enum e_ast_type
{
    AST_COMMAND,                    // Simple command
    AST_PIPELINE,                   // Pipeline
    AST_LOGICAL_AND,                // &&
    AST_LOGICAL_OR,                 // ||
}   t_ast_type;

typedef struct s_ast_node
{
    t_ast_type          type;       // Node type
    union {
        t_command       *command;   // For AST_COMMAND
        t_pipeline      *pipeline;  // For AST_PIPELINE
        struct {
            struct s_ast_node *left;
            struct s_ast_node *right;
        } logical;                  // For AND/OR
    } data;
}   t_ast_node;
```

### Process Tracking

```c
typedef struct s_process
{
    pid_t               pid;        // Process ID
    struct s_process    *next;      // Next process
}   t_process;
```

---

## 🔧 Module Descriptions

### 1. Main Program (`src/main.c`, `src/init.c`, `src/cleanup.c`)

**Responsibility**: Program lifecycle management

**Functions**:
- `main()`: Entry point, initialize and run shell loop
- `init_shell()`: Initialize shell context, environment, signals
- `shell_loop()`: Main interactive loop (prompt → execute → repeat)
- `cleanup_shell()`: Free all resources before exit

**Key Operations**:
1. Parse command line arguments (if any)
2. Initialize shell context
3. Set up signal handlers
4. Enter interactive loop:
   - Display prompt
   - Read line with readline
   - Process input
   - Execute command
5. Clean up on exit (Ctrl-D or exit command)

---

### 2. Environment Management (`src/environment/`)

**Responsibility**: Environment variable storage and manipulation

**Files**:
- `env_init.c`: Initialize from system environment
- `env_get.c`: Retrieve variable values
- `env_set.c`: Set/update variables
- `env_utils.c`: Helper functions

**Key Functions**:
- `init_environment(char **environ)`: Load system environment
- `get_env_value(t_env *env, char *key)`: Get variable value
- `set_env_value(t_env **env, char *key, char *value)`: Set variable
- `unset_env_value(t_env **env, char *key)`: Remove variable
- `env_list_to_array(t_env *env)`: Convert to array for execve

**Data Flow**:
```
System environ → Linked list → Shell operations → Array for execve
```

---

### 3. Signal Handling (`src/signals/`)

**Responsibility**: Manage Unix signals for shell and child processes

**Files**:
- `signals.c`: Signal handlers and setup
- `signal_utils.c`: Helper functions

**Global Variable**:
```c
int g_signal_num;  // ONLY global variable (stores signal number)
```

**Key Functions**:
- `signal_handler(int signum)`: Handle SIGINT and SIGQUIT
- `setup_signals()`: Configure signal handling for interactive mode
- `setup_child_signals()`: Reset signals for child processes
- `restore_signals()`: Restore original handlers

**Signal Behavior**:
- **SIGINT (Ctrl-C)**: New prompt on new line
- **SIGQUIT (Ctrl-\)**: Ignored in interactive mode
- **Child processes**: Can be interrupted with Ctrl-C

---

### 4. Lexer/Tokenizer (`src/lexer/`)

**Responsibility**: Convert input string into tokens

**Files**:
- `tokenizer.c`: Main tokenization logic
- `token_utils.c`: Token list management
- `quote_handler.c`: Quote processing

**Key Functions**:
- `tokenize(char *input)`: Main entry point
- `extract_word()`: Extract command/argument tokens
- `extract_quoted_string()`: Handle quoted content
- `identify_operator()`: Recognize shell operators
- `new_token()`: Create token node

**Tokenization Rules**:
1. **Whitespace**: Token separator (except in quotes)
2. **Operators**: `|`, `<`, `>`, `<<`, `>>`, `&&`, `||`, `(`, `)`
3. **Single quotes**: Preserve everything literally
4. **Double quotes**: Preserve content, allow variable expansion
5. **Words**: Everything else

**Example**:
```
Input:  echo "hello world" | grep hello > output.txt
Tokens: [WORD:echo] [WORD:"hello world"] [PIPE] [WORD:grep] 
        [WORD:hello] [REDIRECT_OUT] [WORD:output.txt]
```

---

### 5. Parser (`src/parser/`)

**Responsibility**: Build Abstract Syntax Tree from tokens

**Files**:
- `parser.c`: Main parsing logic
- `parse_command.c`: Command parsing
- `parse_redirect.c`: Redirection parsing
- `parse_pipeline.c`: Pipeline parsing

**Key Functions**:
- `parse_command_line(t_token *tokens)`: Entry point, returns AST
- `parse_logical_expression()`: Handle && and || [BONUS]
- `parse_pipeline()`: Handle pipe operators
- `parse_simple_command()`: Parse single command with redirections
- `parse_redirection()`: Parse redirection operators

**Parsing Precedence** (highest to lowest):
1. Parentheses `()` [BONUS]
2. Pipes `|`
3. Logical AND `&&` [BONUS]
4. Logical OR `||` [BONUS]

**Grammar** (simplified):
```
command_line    : logical_expr
logical_expr    : pipeline (('&&' | '||') pipeline)*
pipeline        : command ('|' command)*
command         : word* redirection*
redirection     : ('<' | '>' | '>>' | '<<') word
```

---

### 6. Expander (`src/expander/`)

**Responsibility**: Expand variables and wildcards

**Files**:
- `expander.c`: Main expansion logic
- `expand_var.c`: Variable substitution
- `expand_utils.c`: Helper functions

**Key Functions**:
- `expand_variables(char *str, t_shell *shell)`: Expand $VAR
- `expand_exit_status(char *str, int status)`: Expand $?
- `extract_var_name()`: Get variable name from $...
- `expand_wildcards(char **args)`: Wildcard expansion [BONUS]

**Expansion Rules**:
1. **`$VAR`**: Replace with environment variable value
2. **`$?`**: Replace with last exit status
3. **`$$`**: Replace with shell PID (optional)
4. **Single quotes**: No expansion
5. **Double quotes**: Expand variables, not wildcards
6. **No quotes**: Expand everything

**Example**:
```bash
USER=john
echo "$USER is here"  →  "john is here"
echo '$USER is here'  →  "$USER is here"
echo $?               →  "0" (if last command succeeded)
```

---

### 7. Executor (`src/executor/`)

**Responsibility**: Execute commands and manage processes

**Files**:
- `executor.c`: Main execution coordinator
- `exec_simple.c`: Simple command execution
- `exec_pipeline.c`: Pipeline execution
- `exec_utils.c`: Helper functions

**Key Functions**:
- `execute_ast(t_ast_node *ast, t_shell *shell)`: Entry point
- `execute_simple_command()`: Execute single command
- `execute_pipeline()`: Execute piped commands
- `execute_logical_and/or()`: Execute && and || [BONUS]
- `find_executable()`: Resolve command path

**Execution Decision Tree**:
```
Is built-in?
├─ YES → Execute in current process
│         (with redirections)
└─ NO  → Fork child process
          ├─ Setup redirections
          ├─ Reset signals
          ├─ Resolve PATH
          └─ execve()
```

---

### 8. Redirections (`src/redirections/`)

**Responsibility**: Handle I/O redirection

**Files**:
- `redirect.c`: Main redirection handler
- `redirect_input.c`: Input redirection
- `redirect_output.c`: Output redirection
- `heredoc.c`: Heredoc implementation

**Key Functions**:
- `setup_redirections(t_redirect *redirects)`: Apply all redirections
- `redirect_input(char *file)`: Handle `<`
- `redirect_output(char *file, int append)`: Handle `>` and `>>`
- `redirect_heredoc(char *delimiter)`: Handle `<<`
- `restore_fds(int saved_stdin, int saved_stdout)`: Restore original FDs

**Redirection Types**:

| Operator | Description | Implementation |
|----------|-------------|----------------|
| `<` | Input from file | `open()` + `dup2()` to stdin |
| `>` | Output to file (truncate) | `open(O_TRUNC)` + `dup2()` to stdout |
| `>>` | Output to file (append) | `open(O_APPEND)` + `dup2()` to stdout |
| `<<` | Heredoc (read until delimiter) | Create pipe, write content, `dup2()` to stdin |

**File Descriptor Management**:
1. Save original stdin/stdout before redirections
2. Open files and apply redirections with `dup2()`
3. Execute command
4. Restore original stdin/stdout
5. Close all opened file descriptors

---

### 9. Built-in Commands (`src/builtins/`)

**Responsibility**: Implement shell built-in commands

**Files** (one per built-in):
- `builtin_handler.c`: Dispatcher
- `echo.c`: echo command
- `cd.c`: cd command
- `pwd.c`: pwd command
- `export.c`: export command
- `unset.c`: unset command
- `env.c`: env command
- `exit.c`: exit command

**Built-in Characteristics**:
- Execute in current shell process (don't fork)
- Can modify shell state (environment, cwd, etc.)
- Return exit status (0 = success, 1+ = error)

**Implementation Details**:

| Command | Purpose | Key Operations |
|---------|---------|----------------|
| `echo` | Print arguments | Parse `-n` flag, print with spaces |
| `cd` | Change directory | `chdir()`, update PWD/OLDPWD |
| `pwd` | Print working dir | `getcwd()`, print to stdout |
| `export` | Set env variables | Parse key=value, update environment |
| `unset` | Remove env variables | Remove from environment list |
| `env` | Display environment | Print all key=value pairs |
| `exit` | Exit shell | Validate arg, cleanup, `exit()` |

---

### 10. Process Management (`src/process/`)

**Responsibility**: Fork, execute, and wait for child processes

**Files**:
- `process.c`: Process coordination
- `fork_exec.c`: Fork and execute
- `wait.c`: Wait for processes

**Key Functions**:
- `fork_and_execute()`: Fork and exec external command
- `wait_for_children()`: Wait for all child processes
- `setup_pipes()`: Create pipes for pipeline
- `terminate_children()`: Send signals to all children

**Pipeline Execution Flow**:
```
Command: cmd1 | cmd2 | cmd3

1. Create 2 pipes: pipe1[0,1], pipe2[0,1]

2. Fork cmd1:
   - Close: pipe1[0], pipe2[0], pipe2[1]
   - Redirect: stdout → pipe1[1]
   - Exec cmd1
   
3. Fork cmd2:
   - Close: pipe1[1], pipe2[0]
   - Redirect: stdin → pipe1[0], stdout → pipe2[1]
   - Exec cmd2
   
4. Fork cmd3:
   - Close: pipe1[0], pipe1[1], pipe2[1]
   - Redirect: stdin → pipe2[0]
   - Exec cmd3

5. Parent closes all pipes
6. Wait for all children
7. Return exit status of cmd3
```

---

### 11. Utilities (`src/utils/`)

**Responsibility**: Shared utility functions

**Files**:
- `string_utils.c`: String operations
- `memory.c`: Memory management
- `validation.c`: Input validation
- `error.c`: Error handling

**Key Functions**:
- String: `ft_strdup()`, `ft_strjoin()`, `ft_split()`
- Memory: `free_string_array()`, `safe_malloc()`
- Validation: `is_valid_number()`, `is_valid_identifier()`
- Error: `print_error()`, `handle_error()`

---

## 🔄 Execution Flow Examples

### Example 1: Simple Command

```bash
Input: ls -la
```

**Flow**:
1. **Lexer**: `[WORD:ls] [WORD:-la]`
2. **Parser**: `AST_COMMAND { args: ["ls", "-la", NULL] }`
3. **Expander**: No variables to expand
4. **Executor**:
   - Not a built-in
   - Fork child process
   - Find executable: `/bin/ls`
   - `execve("/bin/ls", ["ls", "-la", NULL], env)`
5. **Wait**: Parent waits for child, gets exit status
6. **Result**: Directory listing displayed

### Example 2: Built-in with Redirection

```bash
Input: echo "hello" > output.txt
```

**Flow**:
1. **Lexer**: `[WORD:echo] [WORD:"hello"] [REDIRECT_OUT] [WORD:output.txt]`
2. **Parser**: 
   ```
   AST_COMMAND {
       args: ["echo", "hello", NULL],
       redirects: [{ type: REDIRECT_OUT, file: "output.txt" }]
   }
   ```
3. **Expander**: Remove quotes: `hello`
4. **Executor**:
   - Is built-in: `echo`
   - Save stdout: `saved_fd = dup(STDOUT_FILENO)`
   - Open file: `fd = open("output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)`
   - Redirect: `dup2(fd, STDOUT_FILENO)`
   - Execute built-in echo: prints "hello" to file
   - Restore: `dup2(saved_fd, STDOUT_FILENO)`
   - Close: `close(fd)`, `close(saved_fd)`
5. **Result**: "hello" written to `output.txt`, exit status 0

### Example 3: Pipeline

```bash
Input: cat file.txt | grep "pattern" | wc -l
```

**Flow**:
1. **Lexer**: 
   ```
   [WORD:cat] [WORD:file.txt] [PIPE] [WORD:grep] [WORD:"pattern"] 
   [PIPE] [WORD:wc] [WORD:-l]
   ```
2. **Parser**:
   ```
   AST_PIPELINE {
       commands: [
           { args: ["cat", "file.txt", NULL] },
           { args: ["grep", "pattern", NULL] },
           { args: ["wc", "-l", NULL] }
       ],
       cmd_count: 3
   }
   ```
3. **Executor**:
   - Create 2 pipes: `pipe1`, `pipe2`
   - Fork 3 processes:
     - P1: cat → pipe1 write
     - P2: pipe1 read → grep → pipe2 write
     - P3: pipe2 read → wc
   - Parent: Close all pipes, wait for all
4. **Result**: Count of matching lines displayed

### Example 4: Variable Expansion

```bash
Input: echo "User: $USER, Exit: $?"
```

**Flow**:
1. **Lexer**: `[WORD:echo] [WORD:"User: $USER, Exit: $?"]`
2. **Parser**: `AST_COMMAND { args: ["echo", "User: $USER, Exit: $?", NULL] }`
3. **Expander**:
   - Find `$USER` → lookup in environment → `"john"`
   - Find `$?` → get last exit status → `"0"`
   - Result: `"User: john, Exit: 0"`
4. **Executor**: Built-in echo prints expanded string
5. **Result**: "User: john, Exit: 0"

### Example 5: Heredoc

```bash
Input: cat << EOF
line 1
line 2
EOF
```

**Flow**:
1. **Lexer**: `[WORD:cat] [HEREDOC] [WORD:EOF]`
2. **Parser**: 
   ```
   AST_COMMAND {
       args: ["cat", NULL],
       redirects: [{ type: HEREDOC, file: "EOF" }]
   }
   ```
3. **Executor**:
   - Create pipe: `pipe(pipefd)`
   - Read lines until "EOF":
     - `readline()` for each line
     - Write to `pipefd[1]`
   - Close write end: `close(pipefd[1])`
   - Redirect stdin: `dup2(pipefd[0], STDIN_FILENO)`
   - Fork and exec cat
   - Close read end: `close(pipefd[0])`
4. **Result**: cat reads from pipe, prints lines

---

## 🚦 Signal Handling Details

### Signal States

| Mode | SIGINT (Ctrl-C) | SIGQUIT (Ctrl-\) |
|------|-----------------|------------------|
| **Interactive shell** | New prompt on new line | Ignored |
| **Executing command** | Send to child, display prompt after | Send to child |
| **Heredoc input** | Interrupt input, new prompt | Ignored |

### Implementation

```c
// Global variable (ONLY one allowed)
int g_signal_num = 0;

void signal_handler(int signum)
{
    g_signal_num = signum;  // Store signal number
    
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();       // Move to new line
        rl_replace_line("", 0); // Clear current line
        rl_redisplay();         // Redisplay prompt
    }
}

void setup_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);
    
    // Ignore SIGQUIT in interactive mode
    signal(SIGQUIT, SIG_IGN);
}

void setup_child_signals(void)
{
    // Reset to default for child processes
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
```

---

## 🧠 Memory Management Strategy

### Principles

1. **Every malloc has a corresponding free**
2. **Free in reverse order of allocation**
3. **Set pointers to NULL after freeing**
4. **Use cleanup functions for complex structures**
5. **Free on error paths too**

### Memory Ownership Rules

| Structure | Owner | Cleanup Responsibility |
|-----------|-------|------------------------|
| Token list | Parser | Parser frees after building AST |
| AST | Executor | Executor frees after execution |
| Command args | Command | Command cleanup function |
| Environment list | Shell context | Shell cleanup on exit |
| Pipe FDs | Executor | Close immediately after use |

### Cleanup Functions

```c
void free_tokens(t_token *tokens);
void free_command(t_command *cmd);
void free_pipeline(t_pipeline *pipeline);
void free_ast(t_ast_node *ast);
void free_env_list(t_env *env);
void cleanup_shell(t_shell *shell);
```

### Valgrind Testing

```bash
# Basic leak check
valgrind --leak-check=full ./minishell

# Show all leak types
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Track file descriptors
valgrind --track-fds=yes ./minishell

# With readline suppression file
valgrind --suppressions=readline.supp --leak-check=full ./minishell
```

---

## ⚠️ Error Handling

### Error Categories

1. **Syntax Errors**: Invalid command syntax
2. **Runtime Errors**: Command not found, permission denied
3. **System Errors**: Fork failure, pipe failure
4. **Resource Errors**: Memory allocation failure

### Error Handling Strategy

```c
// Return error codes
#define SUCCESS         0
#define ERROR_SYNTAX    2
#define ERROR_COMMAND   127
#define ERROR_NOEXEC    126
#define ERROR_GENERIC   1

// Error message format
void print_error(char *context, char *message, char *detail)
{
    write(STDERR_FILENO, "minishell: ", 11);
    if (context)
        write(STDERR_FILENO, context, strlen(context));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, message, strlen(message));
    if (detail)
    {
        write(STDERR_FILENO, ": ", 2);
        write(STDERR_FILENO, detail, strlen(detail));
    }
    write(STDERR_FILENO, "\n", 1);
}
```

### Error Examples

```bash
# Command not found
$ invalid_command
minishell: invalid_command: command not found

# Permission denied
$ ./no_exec_perm
minishell: ./no_exec_perm: Permission denied

# Syntax error
$ echo "unclosed quote
minishell: syntax error: unclosed quote

# File not found (redirection)
$ cat < nonexistent
minishell: nonexistent: No such file or directory
```

---

## 🎯 Design Decisions

### Why Linked Lists for Tokens?

- **Dynamic size**: Don't know token count in advance
- **Easy insertion**: Can insert tokens during parsing
- **Simple traversal**: One-pass parsing
- **Memory efficient**: Allocate only what's needed

### Why AST for Commands?

- **Flexibility**: Can represent complex expressions
- **Precedence**: Natural handling of operator precedence
- **Extensibility**: Easy to add new node types (bonus features)
- **Clarity**: Clear representation of command structure

### Why Separate Lexer and Parser?

- **Separation of concerns**: Tokenization vs. syntax analysis
- **Modularity**: Can test independently
- **Maintainability**: Easier to modify/debug
- **Standard practice**: Follows compiler design principles

### Why Fork for External Commands?

- **Isolation**: Child process can't corrupt shell state
- **Safety**: Shell continues even if command crashes
- **Standard**: Unix standard for process execution
- **Required**: execve() replaces process image

---

## 📊 Performance Considerations

### Optimization Strategies

1. **Environment Lookup**: Consider hash table for large environments
2. **PATH Search**: Cache last found executable path
3. **Token Reuse**: Reuse token structures where possible
4. **Minimize Syscalls**: Batch operations when possible
5. **Efficient String Ops**: Use optimal string functions

### Time Complexity

| Operation | Complexity | Notes |
|-----------|------------|-------|
| Tokenize | O(n) | n = input length |
| Parse | O(t) | t = number of tokens |
| Env lookup | O(e) | e = number of env vars |
| PATH search | O(d) | d = directories in PATH |
| Execute | O(1) | + command execution time |

---

## 🔍 Testing Strategy

### Unit Testing

- Test each module independently
- Mock dependencies
- Cover edge cases
- Test error conditions

### Integration Testing

- Test module interactions
- Test complete command execution
- Compare with bash behavior
- Test with various inputs

### Stress Testing

- Long input lines (1000+ chars)
- Many arguments (100+)
- Deep pipelines (10+ commands)
- Large environment (100+ variables)
- Rapid command execution

---

## 📚 References

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/)
- [The Linux Programming Interface](https://man7.org/tlpi/)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)

---

**Document Version**: 1.0  
**Last Updated**: November 3, 2025  
**Maintained By**: Minishell Team
