# Minishell (42)

*"As beautiful as a shell"*

Implementation of a simple shell inspired by Bash, featuring command parsing, process management, I/O redirection, pipes, environment variables, and built-in commands. Focus on creating a robust, interactive command-line interface with proper signal handling and memory management.

## 🎯 Evaluation Criteria Checklist

### ✅ **MANDATORY REQUIREMENTS**
- [ ] **SINGLE GLOBAL VARIABLE** - Only for signal number storage (no additional data access)
- [ ] **NO MEMORY LEAKS** - Verified with `valgrind --leak-check=full` (readline leaks excluded)
- [ ] **NO CRASHES/SEGFAULTS** - Robust error handling throughout
- [ ] **42 NORM COMPLIANT** - All files pass norminette
- [ ] **PROPER SIGNAL HANDLING** - Ctrl-C, Ctrl-D, Ctrl-\ behave like Bash
- [ ] **WORKING HISTORY** - Command navigation with Up/Down arrows
- [ ] **EXECUTABLE SEARCH** - PATH, relative, and absolute path resolution

### ✅ **CORE FUNCTIONALITY REQUIREMENTS**
- [ ] **PROMPT DISPLAY** - Interactive prompt when waiting for commands
- [ ] **QUOTE HANDLING** - Single quotes (no interpretation) and double quotes ($ expansion)
- [ ] **REDIRECTIONS** - `<`, `>`, `<<` (heredoc), `>>` (append mode)
- [ ] **PIPES** - Command chaining with `|` character
- [ ] **ENVIRONMENT VARIABLES** - `$VAR` expansion and `$?` exit status
- [ ] **BUILT-IN COMMANDS** - echo, cd, pwd, export, unset, env, exit

### ✅ **BONUS REQUIREMENTS**
- [ ] **LOGICAL OPERATORS** - `&&` and `||` with parenthesis priorities
- [ ] **WILDCARDS** - `*` expansion in current working directory

---

## 🚀 Quick Start

### **Build & Run**
```bash
# Mandatory part
make
./minishell

# With libft (if using)
make        # Automatically compiles libft if present
./minishell

# Bonus part
make bonus
./minishell
```

### **Basic Usage**
```bash
# Simple commands
minishell$ ls -la
minishell$ /bin/echo "Hello World"
minishell$ pwd

# Built-in commands  
minishell$ cd /home/user
minishell$ export MY_VAR=value
minishell$ echo $MY_VAR
minishell$ exit

# Pipes and redirections
minishell$ ls | grep ".c" > output.txt
minishell$ cat < input.txt | wc -l
minishell$ cat << EOF
```

---

## 📋 Required Testing (Evaluation Sheet)

### **CRITICAL TESTS** ⚠️

#### **1. Basic Functionality**
```bash
# Simple commands with absolute paths
/bin/ls
/bin/echo "test"

# Commands with arguments (no quotes)
ls -la /home
grep "pattern" file.txt

# Empty commands and whitespace
[ENTER]
   [ENTER]  # spaces/tabs only
```

#### **2. Built-in Commands**
```bash
# Echo with various options
echo "Hello World"
echo -n "No newline"
echo $USER

# Exit with different arguments
exit
exit 42
exit abc  # should handle invalid args

# Directory navigation
cd /home
cd ..
cd .
cd invalid_dir  # error handling
pwd

# Environment management
env
export NEW_VAR=value
export EXISTING_VAR=new_value
unset PATH  # test impact
unset NEW_VAR
```

#### **3. Signal Handling**
```bash
# Empty prompt tests
Ctrl-C    # new line + new prompt
Ctrl-\    # no action
Ctrl-D    # exit minishell

# After typing text
minishell$ some text[Ctrl-C]  # new line, clean buffer
minishell$ some text[Ctrl-D]  # no action
minishell$ some text[Ctrl-\]  # no action

# During blocking commands
minishell$ cat[Ctrl-C]        # interrupt
minishell$ grep "text"[Ctrl-\] # no action
```

#### **4. Quotes and Parsing**
```bash
# Double quotes ($ expansion)
echo "Current user: $USER"
echo "cat lol.c | cat > lol.c"  # no execution, just echo

# Single quotes (no interpretation)
echo '$USER'        # prints "$USER"
echo '| > < && ||'  # prints literally

# Mixed quotes
echo "User: '$USER'"
echo 'Path: "$PATH"'
```

#### **5. Environment Variables**
```bash
# Variable expansion
echo $USER
echo $PATH
echo $NONEXISTENT  # empty output

# Exit status
ls /valid/path; echo $?      # 0
ls /invalid/path; echo $?    # non-zero
expr $? + $?                 # arithmetic with exit status
```

#### **6. Redirections & Pipes**
```bash
# Input/Output redirection
echo "test" > file.txt
cat < file.txt
echo "append" >> file.txt

# Heredoc
cat << EOF
line 1
line 2
EOF

# Pipes
ls | grep ".c"
cat file.txt | grep "pattern" | wc -l
echo "test" | cat | cat | more

# Complex combinations
ls | grep ".c" > c_files.txt
cat < input.txt | sort | uniq > output.txt
```

#### **7. Error Handling & Edge Cases**
```bash
# Invalid commands
invalidcommand
/bin/invalidcommand

# PATH manipulation
unset PATH
ls  # should fail
export PATH="/bin:/usr/bin"
ls  # should work

# Complex relative paths
cd ../../..
./script.sh
../bin/executable

# Long commands with many arguments
echo arg1 arg2 arg3 ... arg100
```

### **Memory & Stability Testing**
```bash
# Memory leak check (exclude readline)
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Stress testing
# Run multiple complex commands
# Test history navigation extensively
# Try malformed input
```

---

## 🏗️ Data Structures & Headers

### **Main Header (`include/minishell.h`)**
```c
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

/*
** External functions allowed:
** readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay,
** add_history, printf, malloc, free, write, access, open, read, close,
** fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset,
** sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink,
** execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror,
** isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr,
** tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
*/

# define PROMPT "minishell$ "
# define MAX_PATH 4096
# define MAX_ARGS 1024

/* Global variable for signal handling only */
extern int	g_signal_num;

/* Token types for parsing */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF
}	t_token_type;

/* Token structure */
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

/* Redirection structure */
typedef struct s_redirect
{
	t_token_type			type;
	char					*file;
	int						fd;
	struct s_redirect		*next;
}	t_redirect;

/* Command structure */
typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
}	t_command;

/* Pipeline structure */
typedef struct s_pipeline
{
	t_command			*commands;
	int					cmd_count;
	struct s_pipeline	*next;
}	t_pipeline;

/* AST node for bonus logical operators */
typedef struct s_ast_node
{
	t_token_type			type;
	t_pipeline				*pipeline;
	struct s_ast_node		*left;
	struct s_ast_node		*right;
}	t_ast_node;

/* Environment variable structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Main shell context */
typedef struct s_shell
{
	t_env		*env_list;
	char		**env_array;
	int			last_exit_status;
	char		*current_dir;
	int			interactive;
	pid_t		*child_pids;
	int			child_count;
}	t_shell;

#endif
```

### **Parser Header (`include/parser.h`)**
```c
#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/* Lexer functions */
t_token		*tokenize(char *input);
t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new);
void		free_tokens(t_token *tokens);

/* Parser functions */
t_ast_node	*parse_command_line(t_token *tokens);
t_pipeline	*parse_pipeline(t_token **tokens);
t_command	*parse_command(t_token **tokens);
t_redirect	*parse_redirect(t_token **tokens);

/* AST functions */
t_ast_node	*new_ast_node(t_token_type type);
void		free_ast(t_ast_node *node);

/* Quote handling */
char		*handle_quotes(char *str);
char		*expand_variables(char *str, t_shell *shell);

#endif
```

### **Built-ins Header (`include/builtins.h`)**
```c
#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

/* Built-in command functions */
int		builtin_echo(char **args);
int		builtin_cd(char **args, t_shell *shell);
int		builtin_pwd(char **args);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_env(char **args, t_shell *shell);
int		builtin_exit(char **args, t_shell *shell);

/* Built-in utilities */
int		is_builtin(char *command);
int		execute_builtin(char **args, t_shell *shell);

#endif
```

---

## 📚 Implementation Roadmap & Function Documentation

### **PHASE 1: Foundation & Setup**

#### **1.1 Environment Management (`src/env.c`)**
```c
/*
** @brief: Initializes environment list from system env
** @param: shell - shell context
** @return: 0 on success, 1 on error
** TODO:
**   1. Get system environment with extern char **environ
**   2. Parse each env var into key=value pairs
**   3. Create linked list of t_env structures
**   4. Handle memory allocation failures
**   5. Set initial last_exit_status to 0
*/
int		init_environment(t_shell *shell);

/*
** @brief: Gets environment variable value
** @param: key - variable name, env_list - environment list
** @return: value string or NULL if not found
** TODO:
**   1. Iterate through environment linked list
**   2. Compare keys using strcmp
**   3. Return value if found, NULL otherwise
**   4. Handle NULL inputs gracefully
*/
char	*get_env_value(char *key, t_env *env_list);

/*
** @brief: Sets environment variable (create or update)
** @param: key - variable name, value - variable value, shell - context
** @return: 0 on success, 1 on error
** TODO:
**   1. Search for existing key in environment list
**   2. If found: free old value, set new value
**   3. If not found: create new t_env node
**   4. Handle memory allocation failures
**   5. Rebuild env_array for execve
*/
int		set_env_value(char *key, char *value, t_shell *shell);

/*
** @brief: Removes environment variable
** @param: key - variable name, shell - shell context
** @return: 0 on success, 1 on error
** TODO:
**   1. Find variable in environment list
**   2. Remove node from linked list
**   3. Free key and value memory
**   4. Free node structure
**   5. Rebuild env_array for execve
*/
int		unset_env_value(char *key, t_shell *shell);

/*
** @brief: Converts env list to string array for execve
** @param: env_list - environment linked list
** @return: NULL-terminated string array
** TODO:
**   1. Count environment variables
**   2. Allocate array of char* pointers
**   3. For each env var: create "key=value" string
**   4. Set last element to NULL
**   5. Handle memory allocation failures
*/
char	**env_list_to_array(t_env *env_list);
```

#### **1.2 Signal Handling (`src/signals.c`)**
```c
/*
** @brief: Signal handler function
** @param: sig - signal number
** @return: void
** TODO:
**   1. Store signal number in global variable
**   2. Handle SIGINT (Ctrl-C): print newline, redisplay prompt
**   3. Handle SIGQUIT (Ctrl-\): ignore in interactive mode
**   4. Handle SIGTERM: graceful shutdown
**   5. Use only signal-safe functions
*/
void	signal_handler(int sig);

/*
** @brief: Sets up signal handlers for interactive mode
** @param: void
** @return: 0 on success, 1 on error
** TODO:
**   1. Set up SIGINT handler for Ctrl-C
**   2. Ignore SIGQUIT for Ctrl-\
**   3. Use sigaction for reliable signal handling
**   4. Save old signal handlers for restoration
**   5. Handle signal setup failures
*/
int		setup_signals(void);

/*
** @brief: Sets up signals for child processes
** @param: void
** @return: 0 on success, 1 on error
** TODO:
**   1. Reset SIGINT to default behavior
**   2. Reset SIGQUIT to default behavior
**   3. Allow child processes to handle signals normally
**   4. Use sigaction for reliable setup
*/
int		setup_child_signals(void);

/*
** @brief: Restores original signal handlers
** @param: void
** @return: void
** TODO:
**   1. Restore saved signal handlers
**   2. Reset signal masks if modified
**   3. Clean up signal-related resources
*/
void	restore_signals(void);
```

### **PHASE 2: Lexical Analysis & Parsing**

#### **2.1 Tokenizer (`src/lexer.c`)**
```c
/*
** @brief: Main tokenization function
** @param: input - command line string
** @return: linked list of tokens
** TODO:
**   1. Skip leading whitespace
**   2. Identify token types: words, operators, quotes
**   3. Handle special characters: |, <, >, <<, >>, &&, ||
**   4. Process quoted strings (preserve content)
**   5. Create token linked list
**   6. Handle tokenization errors gracefully
*/
t_token	*tokenize(char *input);

/*
** @brief: Extracts word token (command/argument)
** @param: input - string pointer, pos - current position pointer
** @return: word string (malloc'd)
** TODO:
**   1. Start from current position
**   2. Continue until whitespace or special character
**   3. Handle quoted sections within words
**   4. Update position pointer
**   5. Return allocated string
*/
char	*extract_word(char *input, int *pos);

/*
** @brief: Extracts quoted string token
** @param: input - string pointer, pos - current position, quote - quote type
** @return: quoted content string (malloc'd)
** TODO:
**   1. Skip opening quote
**   2. Extract content until closing quote
**   3. Handle escaped characters in double quotes
**   4. Update position pointer past closing quote
**   5. Return content without quotes
*/
char	*extract_quoted_string(char *input, int *pos, char quote);

/*
** @brief: Identifies operator tokens
** @param: input - string pointer, pos - current position pointer
** @return: token type enum
** TODO:
**   1. Check for two-character operators first (<<, >>, &&, ||)
**   2. Check for single-character operators (|, <, >)
**   3. Update position pointer appropriately
**   4. Return corresponding token type
**   5. Handle invalid operator combinations
*/
t_token_type	identify_operator(char *input, int *pos);

/*
** @brief: Creates new token structure
** @param: type - token type, value - token value string
** @return: allocated token structure
** TODO:
**   1. Allocate memory for t_token
**   2. Set type and duplicate value string
**   3. Initialize next pointer to NULL
**   4. Handle memory allocation failures
**   5. Return new token
*/
t_token	*new_token(t_token_type type, char *value);
```

#### **2.2 Parser (`src/parser.c`)**
```c
/*
** @brief: Main parsing function (command line to AST)
** @param: tokens - tokenized input
** @return: abstract syntax tree root
** TODO:
**   1. Parse logical expressions (&&, ||) if bonus
**   2. Handle parentheses for precedence if bonus
**   3. Parse pipelines and commands
**   4. Build AST or pipeline structure
**   5. Handle parsing errors gracefully
*/
t_ast_node	*parse_command_line(t_token *tokens);

/*
** @brief: Parses pipeline (commands connected by pipes)
** @param: tokens - token pointer (modified during parsing)
** @return: pipeline structure
** TODO:
**   1. Parse first command
**   2. While next token is PIPE:
**   3.   Parse next command
**   4.   Add to pipeline
**   5. Set command count
**   6. Handle pipeline parsing errors
*/
t_pipeline	*parse_pipeline(t_token **tokens);

/*
** @brief: Parses single command with arguments and redirections
** @param: tokens - token pointer (modified during parsing)
** @return: command structure
** TODO:
**   1. Parse command name (first word)
**   2. Parse arguments until non-word token
**   3. Parse redirections if present
**   4. Allocate args array with NULL terminator
**   5. Handle command parsing errors
*/
t_command	*parse_command(t_token **tokens);

/*
** @brief: Parses redirection (< > << >>)
** @param: tokens - token pointer (modified during parsing)
** @return: redirection structure
** TODO:
**   1. Identify redirection type
**   2. Consume redirection token
**   3. Parse target filename
**   4. Create redirection structure
**   5. Handle redirection parsing errors
*/
t_redirect	*parse_redirect(t_token **tokens);

/*
** @brief: Expands environment variables in string
** @param: str - string with $variables, shell - shell context
** @return: expanded string (malloc'd)
** TODO:
**   1. Find $ characters in string
**   2. Extract variable name (alphanumeric + _)
**   3. Look up variable value in environment
**   4. Replace $var with value
**   5. Handle $? (exit status) specially
**   6. Handle $$ (process ID) if needed
*/
char	*expand_variables(char *str, t_shell *shell);
```

### **PHASE 3: Built-in Commands**

#### **3.1 Echo Command (`src/builtins/echo.c`)**
```c
/*
** @brief: Implements echo built-in command
** @param: args - argument array (args[0] = "echo")
** @return: exit status (always 0 for echo)
** TODO:
**   1. Check for -n option (no trailing newline)
**   2. Print remaining arguments separated by spaces
**   3. Add newline unless -n was specified
**   4. Handle empty arguments
**   5. Return 0 (echo never fails)
*/
int		builtin_echo(char **args);

/*
** @brief: Checks if argument is -n option
** @param: arg - argument string
** @return: 1 if -n option, 0 otherwise
** TODO:
**   1. Check if arg starts with "-n"
**   2. Ensure no additional characters after -n
**   3. Return appropriate boolean value
*/
int		is_n_option(char *arg);
```

#### **3.2 Directory Commands (`src/builtins/cd_pwd.c`)**
```c
/*
** @brief: Implements cd built-in command
** @param: args - argument array, shell - shell context
** @return: exit status (0 success, 1 error)
** TODO:
**   1. Handle no arguments (cd to HOME)
**   2. Handle single argument (relative or absolute path)
**   3. Update PWD and OLDPWD environment variables
**   4. Use chdir() system call
**   5. Handle errors (permission, non-existent directory)
**   6. Update shell->current_dir
*/
int		builtin_cd(char **args, t_shell *shell);

/*
** @brief: Implements pwd built-in command
** @param: args - argument array (unused)
** @return: exit status (0 success, 1 error)
** TODO:
**   1. Get current working directory with getcwd()
**   2. Print directory path to stdout
**   3. Handle getcwd() errors
**   4. Return appropriate exit status
*/
int		builtin_pwd(char **args);

/*
** @brief: Updates PWD and OLDPWD environment variables
** @param: old_pwd - previous directory, new_pwd - current directory, shell
** @return: void
** TODO:
**   1. Set OLDPWD to old_pwd value
**   2. Set PWD to new_pwd value
**   3. Handle memory allocation for env values
**   4. Update shell environment
*/
void	update_pwd_env(char *old_pwd, char *new_pwd, t_shell *shell);
```

#### **3.3 Environment Commands (`src/builtins/env_cmds.c`)**
```c
/*
** @brief: Implements export built-in command
** @param: args - argument array, shell - shell context
** @return: exit status (0 success, 1 error)
** TODO:
**   1. Handle no arguments: print all env vars in "declare -x" format
**   2. Handle arguments: parse key=value pairs
**   3. Validate variable names (alphanumeric + underscore)
**   4. Set environment variables using set_env_value()
**   5. Handle invalid variable names
*/
int		builtin_export(char **args, t_shell *shell);

/*
** @brief: Implements unset built-in command
** @param: args - argument array, shell - shell context
** @return: exit status (0 success, 1 error)
** TODO:
**   1. Handle multiple arguments (variable names)
**   2. Validate variable names
**   3. Remove variables using unset_env_value()
**   4. Ignore non-existent variables (not an error)
**   5. Handle invalid variable names
*/
int		builtin_unset(char **args, t_shell *shell);

/*
** @brief: Implements env built-in command
** @param: args - argument array, shell - shell context
** @return: exit status (0 success, 1 error)
** TODO:
**   1. Print all environment variables in "key=value" format
**   2. Iterate through environment linked list
**   3. Output to stdout
**   4. Return 0 (env command doesn't fail)
*/
int		builtin_env(char **args, t_shell *shell);

/*
** @brief: Implements exit built-in command
** @param: args - argument array, shell - shell context
** @return: doesn't return (exits process)
** TODO:
**   1. Handle no arguments: exit with last_exit_status
**   2. Handle numeric argument: exit with that status
**   3. Handle invalid arguments: print error, exit with 2
**   4. Perform cleanup before exit
**   5. Call exit() system call
*/
int		builtin_exit(char **args, t_shell *shell);
```

### **PHASE 4: Execution Engine**

#### **4.1 Command Execution (`src/executor.c`)**
```c
/*
** @brief: Main execution function
** @param: ast - abstract syntax tree or pipeline, shell - shell context
** @return: exit status of executed commands
** TODO:
**   1. Determine execution type (pipeline, logical operators)
**   2. Set up pipes for pipeline if needed
**   3. Fork processes for each command
**   4. Handle built-ins vs external commands
**   5. Wait for all processes and collect exit status
*/
int		execute_ast(t_ast_node *ast, t_shell *shell);

/*
** @brief: Executes pipeline of commands
** @param: pipeline - pipeline structure, shell - shell context
** @return: exit status of last command
** TODO:
**   1. Create pipes between commands
**   2. Fork process for each command
**   3. Set up file descriptors (stdin/stdout)
**   4. Execute each command in its process
**   5. Close pipe file descriptors
**   6. Wait for all processes
*/
int		execute_pipeline(t_pipeline *pipeline, t_shell *shell);

/*
** @brief: Executes single command
** @param: cmd - command structure, shell - shell context
** @return: exit status
** TODO:
**   1. Check if command is built-in
**   2. Handle redirections before execution
**   3. For built-ins: execute directly
**   4. For external: fork and execve
**   5. Restore file descriptors after redirections
*/
int		execute_command(t_command *cmd, t_shell *shell);

/*
** @brief: Finds executable in PATH or validates path
** @param: command - command name, env_list - environment
** @return: full path to executable (malloc'd) or NULL
** TODO:
**   1. If command contains '/', check if file exists and is executable
**   2. Otherwise, search in PATH directories
**   3. For each PATH directory, check for executable file
**   4. Return first valid path found
**   5. Return NULL if not found
*/
char	*find_executable(char *command, t_env *env_list);
```

#### **4.2 Process Management (`src/process.c`)**
```c
/*
** @brief: Forks and executes external command
** @param: cmd - command structure, shell - shell context
** @return: child process ID
** TODO:
**   1. Fork new process
**   2. In child: set up signals, execute command
**   3. In parent: add PID to child list, return PID
**   4. Handle fork failures
**   5. Clean up resources on error
*/
pid_t	fork_and_execute(t_command *cmd, t_shell *shell);

/*
** @brief: Waits for all child processes
** @param: shell - shell context
** @return: exit status of last process
** TODO:
**   1. Wait for each child process with waitpid()
**   2. Collect exit statuses
**   3. Handle process termination by signals
**   4. Update last_exit_status
**   5. Clean up child PID list
*/
int		wait_for_children(t_shell *shell);

/*
** @brief: Sets up pipes between commands
** @param: pipeline - pipeline structure
** @return: 0 on success, 1 on error
** TODO:
**   1. Calculate number of pipes needed
**   2. Create pipe file descriptors for each connection
**   3. Store pipe FDs in pipeline structure
**   4. Handle pipe creation failures
**   5. Clean up on error
*/
int		setup_pipes(t_pipeline *pipeline);

/*
** @brief: Terminates all child processes
** @param: shell - shell context
** @return: void
** TODO:
**   1. Send SIGTERM to all child processes
**   2. Wait briefly for graceful termination
**   3. Send SIGKILL to remaining processes
**   4. Clean up process list
*/
void	terminate_children(t_shell *shell);
```

#### **4.3 I/O Redirection (`src/redirect.c`)**
```c
/*
** @brief: Sets up all redirections for a command
** @param: redirects - redirection list, shell - shell context
** @return: 0 on success, 1 on error
** TODO:
**   1. Process each redirection in order
**   2. Open files with appropriate modes
**   3. Duplicate file descriptors to stdin/stdout
**   4. Store original FDs for restoration
**   5. Handle redirection errors
*/
int		setup_redirections(t_redirect *redirects, t_shell *shell);

/*
** @brief: Handles input redirection (<)
** @param: filename - input file name
** @return: file descriptor or -1 on error
** TODO:
**   1. Open file for reading
**   2. Check file permissions and existence
**   3. Return file descriptor
**   4. Handle open errors appropriately
*/
int		redirect_input(char *filename);

/*
** @brief: Handles output redirection (> and >>)
** @param: filename - output file name, append - append mode flag
** @return: file descriptor or -1 on error
** TODO:
**   1. Open file with appropriate flags (O_WRONLY | O_CREAT)
**   2. Use O_APPEND for append mode, O_TRUNC for overwrite
**   3. Set file permissions (0644)
**   4. Return file descriptor
**   5. Handle open errors
*/
int		redirect_output(char *filename, int append);

/*
** @brief: Handles heredoc redirection (<<)
** @param: delimiter - heredoc delimiter string
** @return: file descriptor to read from
** TODO:
**   1. Create temporary pipe
**   2. Read lines from stdin until delimiter
**   3. Write lines to pipe write end
**   4. Close write end, return read end
**   5. Handle readline and pipe errors
*/
int		redirect_heredoc(char *delimiter);

/*
** @brief: Restores original file descriptors
** @param: original_stdin - saved stdin FD, original_stdout - saved stdout FD
** @return: void
** TODO:
**   1. Duplicate original FDs back to stdin/stdout
**   2. Close saved FD copies
**   3. Handle dup2 errors gracefully
*/
void	restore_fds(int original_stdin, int original_stdout);
```

### **PHASE 5: Main Program & Shell Loop**

#### **5.1 Main Shell Loop (`src/main.c`)**
```c
/*
** @brief: Main program entry point
** @param: argc - argument count, argv - argument array
** @return: exit status
** TODO:
**   1. Initialize shell context
**   2. Set up signal handlers
**   3. Enter main shell loop
**   4. Handle cleanup on exit
**   5. Return final exit status
*/
int		main(int argc, char **argv);

/*
** @brief: Main interactive shell loop
** @param: shell - shell context
** @return: exit status
** TODO:
**   1. Display prompt with readline()
**   2. Add non-empty input to history
**   3. Tokenize and parse input
**   4. Execute parsed commands
**   5. Update exit status
**   6. Handle Ctrl-D (EOF)
**   7. Free allocated memory
*/
int		shell_loop(t_shell *shell);

/*
** @brief: Initializes shell context
** @param: shell - shell context to initialize
** @return: 0 on success, 1 on error
** TODO:
**   1. Initialize environment from system
**   2. Set initial working directory
**   3. Set interactive flag based on isatty()
**   4. Initialize child process list
**   5. Set initial exit status to 0
*/
int		init_shell(t_shell *shell);

/*
** @brief: Cleans up shell resources
** @param: shell - shell context
** @return: void
** TODO:
**   1. Free environment list and array
**   2. Free current directory string
**   3. Terminate any remaining child processes
**   4. Free child PID list
**   5. Clear readline history
*/
void	cleanup_shell(t_shell *shell);
```

#### **5.2 Utilities (`src/utils.c`)**
```c
/*
** @brief: Safely duplicates a string
** @param: str - string to duplicate
** @return: duplicated string or NULL on error
** TODO:
**   1. Check for NULL input
**   2. Allocate memory for new string
**   3. Copy string content
**   4. Return new string or NULL on failure
*/
char	*ft_strdup(const char *str);

/*
** @brief: Joins two strings with separator
** @param: s1 - first string, s2 - second string, sep - separator
** @return: joined string (malloc'd)
** TODO:
**   1. Calculate total length needed
**   2. Allocate memory for result
**   3. Copy first string, separator, second string
**   4. Null-terminate result
**   5. Handle memory allocation failures
*/
char	*ft_strjoin_sep(char *s1, char *s2, char sep);

/*
** @brief: Splits string by delimiter
** @param: str - string to split, delimiter - split character
** @return: NULL-terminated array of strings
** TODO:
**   1. Count number of tokens
**   2. Allocate array of string pointers
**   3. Extract each token and allocate string
**   4. Set last element to NULL
**   5. Handle memory allocation failures
*/
char	**ft_split(char const *str, char delimiter);

/*
** @brief: Frees array of strings
** @param: array - string array to free
** @return: void
** TODO:
**   1. Free each string in array
**   2. Free array pointer itself
**   3. Handle NULL input gracefully
*/
void	free_string_array(char **array);

/*
** @brief: Checks if string represents a valid number
** @param: str - string to check
** @return: 1 if valid number, 0 otherwise
** TODO:
**   1. Skip leading whitespace
**   2. Check for optional +/- sign
**   3. Verify all remaining characters are digits
**   4. Check for integer overflow
**   5. Return boolean result
*/
int		is_valid_number(char *str);
```

### **BONUS PART: Advanced Features**

#### **B.1 Logical Operators (`src_bonus/logical.c`)**
```c
/*
** @brief: Parses logical expressions with && and ||
** @param: tokens - token list
** @return: AST node with logical operators
** TODO:
**   1. Parse logical OR expressions (lowest precedence)
**   2. Parse logical AND expressions (higher precedence)
**   3. Handle parentheses for precedence override
**   4. Build binary AST with logical operators
**   5. Handle parsing errors
*/
t_ast_node	*parse_logical_expression(t_token **tokens);

/*
** @brief: Executes logical AND expression
** @param: left - left AST node, right - right AST node, shell - context
** @return: exit status
** TODO:
**   1. Execute left side
**   2. If left side succeeds (exit status 0):
**   3.   Execute right side
**   4.   Return right side's exit status
**   5. Else: return left side's exit status
*/
int		execute_logical_and(t_ast_node *left, t_ast_node *right, t_shell *shell);

/*
** @brief: Executes logical OR expression
** @param: left - left AST node, right - right AST node, shell - context
** @return: exit status
** TODO:
**   1. Execute left side
**   2. If left side fails (exit status != 0):
**   3.   Execute right side
**   4.   Return right side's exit status
**   5. Else: return left side's exit status
*/
int		execute_logical_or(t_ast_node *left, t_ast_node *right, t_shell *shell);
```

#### **B.2 Wildcard Expansion (`src_bonus/wildcard.c`)**
```c
/*
** @brief: Expands wildcards in arguments
** @param: args - argument array with potential wildcards
** @return: expanded argument array
** TODO:
**   1. For each argument containing '*':
**   2.   Open current directory
**   3.   Match filenames against pattern
**   4.   Replace wildcard arg with matched files
**   5. Return expanded argument list
**   6. Handle no matches (return literal '*')
*/
char	**expand_wildcards(char **args);

/*
** @brief: Matches filename against wildcard pattern
** @param: filename - file to test, pattern - pattern with *
** @return: 1 if matches, 0 otherwise
** TODO:
**   1. Implement pattern matching algorithm
**   2. Handle multiple * in pattern
**   3. Match literal characters exactly
**   4. * matches any sequence of characters
**   5. Return boolean result
*/
int		match_pattern(char *filename, char *pattern);

/*
** @brief: Gets list of files in current directory
** @param: pattern - wildcard pattern
** @return: array of matching filenames
** TODO:
**   1. Open current directory with opendir()
**   2. Read all directory entries
**   3. Filter entries matching pattern
**   4. Sort results alphabetically
**   5. Return NULL-terminated array
*/
char	**get_matching_files(char *pattern);
```

---

## 🎯 Implementation Order & Dependencies

### **Priority 1: Foundation (Build First)**
1. **Environment management** (`env.c`) - Core shell state
2. **Signal handling** (`signals.c`) - Interactive behavior  
3. **Utility functions** (`utils.c`) - String manipulation helpers

### **Priority 2: Parsing Engine (Build Second)**
4. **Tokenizer** (`lexer.c`) - Input analysis
5. **Parser** (`parser.c`) - Command structure building
6. **Variable expansion** (`expand.c`) - $VAR processing

### **Priority 3: Built-in Commands (Build Third)**
7. **Echo command** (`builtins/echo.c`) - Simple output
8. **Directory commands** (`builtins/cd_pwd.c`) - Navigation
9. **Environment commands** (`builtins/env_cmds.c`) - Variable management

### **Priority 4: Execution Engine (Build Fourth)**
10. **Redirection handling** (`redirect.c`) - I/O management
11. **Process management** (`process.c`) - Command execution
12. **Main executor** (`executor.c`) - Command coordination

### **Priority 5: Integration (Build Last)**
13. **Main shell loop** (`main.c`) - User interface
14. **Bonus features** (`src_bonus/`) - Advanced functionality

---

## 📝 Output Format & Behavior

### **Interactive Prompt**
```bash
minishell$ 
```

### **Error Messages** 
```bash
minishell: command_name: command not found
minishell: file.txt: No such file or directory
minishell: file.txt: Permission denied
bash: syntax error near unexpected token 'token'
```

### **Signal Behavior**
- **Ctrl-C**: Display new prompt on new line
- **Ctrl-D**: Exit shell
- **Ctrl-\**: No action (ignore)

---

## 🛠️ Makefile Structure

### **Mandatory Part (`Makefile`)**
```makefile
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRCDIR = src
INCDIR = include
LIBFTDIR = libft

SOURCES = main.c env.c signals.c lexer.c parser.c \
          executor.c process.c redirect.c utils.c \
          builtins/echo.c builtins/cd_pwd.c builtins/env_cmds.c

OBJECTS = $(SOURCES:.c=.o)

# Include libft if present
ifeq ($(shell test -d $(LIBFTDIR) && echo yes), yes)
    LIBFT = $(LIBFTDIR)/libft.a
    LDFLAGS += -L$(LIBFTDIR) -lft
    CFLAGS += -I$(LIBFTDIR)
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -f $(OBJECTS)
ifneq ($(LIBFT),)
	make -C $(LIBFTDIR) clean
endif

fclean: clean
	rm -f $(NAME)
ifneq ($(LIBFT),)
	make -C $(LIBFTDIR) fclean
endif

re: fclean all

bonus: $(NAME)

.PHONY: all clean fclean re bonus
```

---

## 🔄 Critical Implementation Strategies

### **1. Signal Handling Strategy**
```c
// Global variable (only one allowed)
int g_signal_num = 0;

void signal_handler(int sig)
{
    g_signal_num = sig;
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}
```

### **2. Quote Parsing Strategy**
```c
char *handle_quotes(char *str)
{
    int in_single = 0, in_double = 0;
    
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        // Process character based on quote state
    }
}
```

### **3. Pipe Implementation Strategy**
```c
int execute_pipeline(t_pipeline *pipeline, t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid;
    
    for (int i = 0; i < pipeline->cmd_count; i++)
    {
        if (i < pipeline->cmd_count - 1)
            pipe(pipe_fd);
            
        pid = fork();
        if (pid == 0)  // Child
        {
            // Setup input from previous command
            // Setup output to next command
            // Execute command
        }
        // Close pipe FDs in parent
    }
    // Wait for all children
}
```

---

## ⚠️ Common Pitfalls & Solutions

### **1. Memory Leaks (Excluding Readline)**
❌ **Wrong**: Not freeing parser structures
```c
// Forgot to free AST nodes and token lists
```
✅ **Correct**: Systematic cleanup
```c
void free_ast(t_ast_node *node)
{
    if (!node) return;
    if (node->pipeline) free_pipeline(node->pipeline);
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}
```

### **2. Signal Race Conditions**
❌ **Wrong**: Accessing data structures in signal handler
```c
void signal_handler(int sig)
{
    shell->should_exit = 1;  // WRONG: accessing complex data
}
```
✅ **Correct**: Minimal signal handler
```c
int g_signal_num = 0;  // Only global variable allowed

void signal_handler(int sig)
{
    g_signal_num = sig;  // Only store signal number
}
```

### **3. File Descriptor Leaks**
❌ **Wrong**: Not closing pipe FDs
```c
pipe(pipe_fd);
// Forgot to close pipe_fd[0] and pipe_fd[1] in parent
```
✅ **Correct**: Systematic FD management
```c
// Always close unused FDs immediately after setup
pipe(pipe_fd);
if (setup_child_fd(pipe_fd))
{
    close(pipe_fd[0]);  // Close in parent
    close(pipe_fd[1]);
}
```

### **4. PATH Resolution Issues**
❌ **Wrong**: Not checking PATH order
```c
// Wrong: only checking if file exists, not PATH order
```
✅ **Correct**: Proper PATH search
```c
char *find_executable(char *cmd, t_env *env)
{
    if (strchr(cmd, '/'))  // Contains path
        return access(cmd, X_OK) == 0 ? ft_strdup(cmd) : NULL;
    
    char *path = get_env_value("PATH", env);
    char **dirs = ft_split(path, ':');
    // Search each directory in order
}
```

---

## ✅ Evaluation Preparation Checklist

### **Before Defense**
- [ ] Run `norminette` on all files - zero errors
- [ ] Test with `valgrind --leak-check=full` - zero leaks (exclude readline)
- [ ] Verify single global variable rule
- [ ] Test all built-in commands thoroughly
- [ ] Test signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- [ ] Test quote handling (single and double)
- [ ] Test pipe and redirection combinations
- [ ] Test environment variable expansion
- [ ] Test error handling for edge cases
- [ ] Verify clean compilation with required flags

### **Defense Strategy**
1. **Explain parsing approach**: Tokenization → AST/Pipeline → Execution
2. **Show signal handling**: Demonstrate global variable usage
3. **Walkthrough built-ins**: Explain each command implementation
4. **Process management**: Explain fork/exec/wait cycle
5. **Memory management**: Show cleanup functions

### **Key Points to Emphasize**
- **Single global variable** - only for signal number storage
- **Readline leak exclusion** - your code must be leak-free
- **Bash compatibility** - behavior matches reference shell
- **Error handling** - graceful handling of all error conditions
- **Signal safety** - signal handlers only modify global variable

---

## 📚 Essential References

- **GNU Readline**: https://tiswww.case.edu/php/chet/readline/rltop.html
- **POSIX Shell**: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html
- **Process Management**: https://man7.org/linux/man-pages/man2/fork.2.html
- **Signal Handling**: https://man7.org/linux/man-pages/man7/signal.7.html
- **File Descriptors**: https://man7.org/linux/man-pages/man2/dup.2.html

**SUCCESS FORMULA**: Proper parsing + Robust execution + Signal safety + Memory management + Bash compatibility = Outstanding evaluation

---

## 📁 Project Files

- **[TODOMS.md](./TODOMS.md)** - Detailed implementation roadmap with checkboxes and time estimates
- **readmeMS.md** - This documentation with technical specifications and evaluation criteria
