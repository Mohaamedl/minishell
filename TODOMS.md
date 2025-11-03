# Minishell Implementation TODO

**Project**: Minishell - Simple Shell Implementation  
**Estimated Total Time**: 60-80 hours  
**Difficulty**: Advanced (handles parsing, processes, signals, I/O)

---

## 📋 **PHASE 1: Foundation & Setup** 
**Estimated Time**: 12-15 hours  
**Status**: ⭕ Not Started

### 🎯 **Environment Management** (`src/env.c`)
**Priority**: 🔥 CRITICAL | **Time**: 4-5 hours

- [ ] **`init_environment()`** - Initialize env list from system environment
  - [ ] Parse `extern char **environ` into linked list
  - [ ] Create `t_env` nodes for each `key=value` pair  
  - [ ] Handle memory allocation failures
  - [ ] Set initial `last_exit_status` to 0
  - **Expected Output**: Environment variables loaded into shell context

- [ ] **`get_env_value()`** - Retrieve environment variable value
  - [ ] Iterate through environment linked list
  - [ ] Compare keys using string comparison
  - [ ] Return value or NULL if not found
  - [ ] Handle NULL inputs gracefully
  - **Test**: `echo $USER` should print current user

- [ ] **`set_env_value()`** - Set/update environment variable
  - [ ] Search for existing key in list
  - [ ] Update value if found, create new node if not
  - [ ] Handle memory allocation for key/value strings
  - [ ] Rebuild `env_array` for execve compatibility
  - **Test**: `export TEST=value; echo $TEST` should print "value"

- [ ] **`unset_env_value()`** - Remove environment variable
  - [ ] Find and remove node from linked list
  - [ ] Free allocated memory (key, value, node)
  - [ ] Rebuild `env_array` after removal
  - [ ] Handle non-existent variables gracefully
  - **Test**: `unset PATH; ls` should fail with command not found

- [ ] **`env_list_to_array()`** - Convert list to array for execve
  - [ ] Count environment variables in list
  - [ ] Allocate array of string pointers + NULL terminator
  - [ ] Create `"key=value"` strings for each variable
  - [ ] Handle memory allocation failures
  - **Validation**: `execve()` calls should work with converted array

### 🎯 **Signal Handling** (`src/signals.c`)
**Priority**: 🔥 CRITICAL | **Time**: 3-4 hours

- [ ] **Global Variable Setup** - Single allowed global variable
  - [ ] Declare `int g_signal_num` in header
  - [ ] Initialize to 0 in main
  - [ ] Use ONLY for signal number storage
  - **Validation**: Evaluator will verify only one global variable

- [ ] **`signal_handler()`** - Signal handler function  
  - [ ] Store signal number in `g_signal_num`
  - [ ] Handle SIGINT (Ctrl-C): write newline, redisplay prompt
  - [ ] Handle SIGQUIT (Ctrl-\): ignore in interactive mode
  - [ ] Use only signal-safe functions (write, rl_* functions)
  - **Test**: Ctrl-C should show new prompt, Ctrl-\ should do nothing

- [ ] **`setup_signals()`** - Configure signal handling for shell
  - [ ] Set SIGINT handler using `sigaction()`
  - [ ] Ignore SIGQUIT with `SIG_IGN`
  - [ ] Save original handlers for restoration
  - [ ] Handle signal setup failures
  - **Validation**: Interactive signals work like bash

- [ ] **`setup_child_signals()`** - Reset signals for child processes
  - [ ] Reset SIGINT and SIGQUIT to default (`SIG_DFL`)
  - [ ] Allow children to handle signals normally
  - [ ] Ensure child processes can be interrupted
  - **Test**: `cat` command should be interruptible with Ctrl-C

- [ ] **`restore_signals()`** - Cleanup function
  - [ ] Restore original signal handlers
  - [ ] Reset signal masks if modified
  - [ ] Called during shell cleanup
  - **Usage**: Called before shell exit

### 🎯 **Utility Functions** (`src/utils.c`)
**Priority**: 🟡 MEDIUM | **Time**: 2-3 hours

- [ ] **`ft_strdup()`** - Safe string duplication
  - [ ] Check for NULL input
  - [ ] Allocate memory and copy string
  - [ ] Return new string or NULL on failure
  - **Usage**: Used throughout codebase for string handling

- [ ] **`ft_strjoin_sep()`** - Join strings with separator
  - [ ] Calculate total length needed
  - [ ] Allocate and concatenate strings with separator
  - [ ] Handle memory allocation failures
  - **Usage**: Building PATH strings, command concatenation

- [ ] **`ft_split()`** - Split string by delimiter
  - [ ] Count tokens first, then allocate array
  - [ ] Extract each token into separate strings
  - [ ] NULL-terminate array
  - **Usage**: Splitting PATH, parsing command arguments

- [ ] **`free_string_array()`** - Clean up string arrays
  - [ ] Free each string in array
  - [ ] Free array pointer itself
  - [ ] Handle NULL input safely
  - **Usage**: Cleanup function called throughout

- [ ] **`is_valid_number()`** - Validate numeric strings
  - [ ] Skip whitespace, check for +/- sign
  - [ ] Verify all remaining chars are digits
  - [ ] Check for integer overflow
  - **Usage**: Validating `exit` command arguments

### 🎯 **Project Structure Setup**
**Priority**: 🟡 MEDIUM | **Time**: 2-3 hours

- [ ] **Directory Structure** - Organize source files
  ```
  minishell/
  ├── Makefile
  ├── include/
  │   ├── minishell.h
  │   ├── parser.h
  │   └── builtins.h
  ├── src/
  │   ├── main.c
  │   ├── env.c
  │   ├── signals.c
  │   ├── utils.c
  │   ├── lexer.c
  │   ├── parser.c
  │   ├── executor.c
  │   ├── process.c
  │   ├── redirect.c
  │   └── builtins/
  │       ├── echo.c
  │       ├── cd_pwd.c
  │       └── env_cmds.c
  ├── src_bonus/
  │   ├── logical.c
  │   └── wildcard.c
  └── libft/ (if using)
  ```

- [ ] **Makefile Creation** - Build system setup
  - [ ] Basic rules: `NAME`, `all`, `clean`, `fclean`, `re`
  - [ ] Compiler flags: `-Wall -Wextra -Werror`
  - [ ] Link with readline: `-lreadline`
  - [ ] Libft integration if present
  - [ ] Bonus rule for advanced features
  - **Test**: `make` should compile without errors

- [ ] **Header Files** - Function declarations and structures
  - [ ] Define all data structures (`t_token`, `t_command`, etc.)
  - [ ] Function prototypes for all modules
  - [ ] Include necessary system headers
  - [ ] Include guards and external declarations
  - **Validation**: Headers should compile independently

---

## 📋 **PHASE 2: Lexical Analysis & Parsing**
**Estimated Time**: 18-22 hours  
**Status**: ⭕ Not Started

### 🎯 **Tokenizer** (`src/lexer.c`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`tokenize()`** - Main tokenization function
  - [ ] Skip leading/trailing whitespace
  - [ ] Identify word tokens (commands, arguments)
  - [ ] Recognize operators: `|`, `<`, `>`, `<<`, `>>`, `&&`, `||`
  - [ ] Handle quoted strings (preserve spacing inside quotes)
  - [ ] Handle parentheses for bonus logical grouping
  - [ ] Create linked list of tokens
  - **Test**: `echo "hello world" | grep hello` should tokenize correctly

- [ ] **`extract_word()`** - Extract command/argument tokens
  - [ ] Continue until whitespace or special character
  - [ ] Handle quoted sections within words
  - [ ] Update position pointer for caller
  - [ ] Allocate and return word string
  - **Test**: `ls -la` should extract "ls", "-la" as separate words

- [ ] **`extract_quoted_string()`** - Process quoted content
  - [ ] Handle single quotes: no interpretation of content
  - [ ] Handle double quotes: preserve content, allow $ expansion
  - [ ] Skip opening/closing quotes in result
  - [ ] Handle unclosed quotes (syntax error)
  - **Test**: `echo 'don'\''t'` should handle nested quotes

- [ ] **`identify_operator()`** - Recognize shell operators
  - [ ] Two-character operators first: `<<`, `>>`, `&&`, `||`
  - [ ] Single-character operators: `|`, `<`, `>`, `(`, `)`
  - [ ] Update position pointer appropriately
  - [ ] Return token type enum
  - **Test**: `cat file << EOF` should identify `<<` as heredoc

- [ ] **`new_token()` & Token List Management**
  - [ ] Allocate token structure
  - [ ] Set type and duplicate value string
  - [ ] Linked list operations (add, free)
  - [ ] Memory cleanup functions
  - **Memory**: All tokens must be freed after parsing

### 🎯 **Parser** (`src/parser.c`)
**Priority**: 🔥 CRITICAL | **Time**: 10-12 hours

- [ ] **`parse_command_line()`** - Top-level parsing function
  - [ ] Handle logical operators (`&&`, `||`) if bonus
  - [ ] Parse parentheses for precedence if bonus
  - [ ] Create AST for complex expressions
  - [ ] Fall back to simple pipeline parsing
  - [ ] Handle parsing errors gracefully
  - **Test**: `ls && echo success || echo fail` should parse correctly

- [ ] **`parse_pipeline()`** - Parse pipe-separated commands
  - [ ] Parse first command
  - [ ] While next token is `|`: parse additional commands
  - [ ] Create pipeline structure with command count
  - [ ] Handle pipeline parsing errors
  - **Test**: `cat file | grep pattern | sort` should create 3-command pipeline

- [ ] **`parse_command()`** - Parse single command with args/redirections
  - [ ] First word is command name
  - [ ] Additional words are arguments
  - [ ] Parse redirections when encountered
  - [ ] Create NULL-terminated args array
  - [ ] Handle empty commands
  - **Test**: `ls -la > output.txt 2>&1` should parse command and redirections

- [ ] **`parse_redirect()`** - Parse redirection operators
  - [ ] Identify redirection type: `<`, `>`, `>>`, `<<`
  - [ ] Next token must be filename/delimiter
  - [ ] Create redirection structure
  - [ ] Link multiple redirections
  - **Test**: `command < input > output >> log` should parse all redirections

- [ ] **`expand_variables()`** - Environment variable expansion
  - [ ] Find `$` characters in strings
  - [ ] Extract variable names (alphanumeric + underscore)
  - [ ] Replace `$VAR` with environment value
  - [ ] Handle `$?` (exit status) specially
  - [ ] Handle `$$` (process ID) if needed
  - [ ] Preserve literal `$` in single quotes
  - **Test**: `echo "User: $USER, Status: $?"` should expand variables

### 🎯 **Data Structure Implementation**
**Priority**: 🟡 MEDIUM | **Time**: 2-3 hours

- [ ] **AST Node Functions** - For bonus logical operators
  - [ ] `new_ast_node()` - Create AST nodes
  - [ ] `free_ast()` - Recursive cleanup
  - [ ] Tree traversal functions
  - **Usage**: Complex command expressions with `&&`, `||`

- [ ] **Memory Management** - Cleanup functions
  - [ ] `free_tokens()` - Clean up token list
  - [ ] `free_command()` - Clean up command structure
  - [ ] `free_pipeline()` - Clean up pipeline
  - [ ] Comprehensive cleanup to prevent leaks
  - **Validation**: Run with valgrind to verify no leaks

---

## 📋 **PHASE 3: Built-in Commands**
**Estimated Time**: 12-15 hours  
**Status**: ⭕ Not Started

### 🎯 **Echo Command** (`src/builtins/echo.c`)
**Priority**: 🟢 EASY | **Time**: 2-3 hours

- [ ] **`builtin_echo()`** - Echo implementation
  - [ ] Check for `-n` option (no trailing newline)
  - [ ] Print remaining arguments with space separation
  - [ ] Add newline unless `-n` specified
  - [ ] Handle multiple `-n` options correctly
  - [ ] Always return 0 (echo never fails)
  - **Test**: `echo -n hello world` should print "hello world" without newline

- [ ] **`is_n_option()`** - Validate -n flag
  - [ ] Check for exact `-n` string
  - [ ] Reject `-n` with additional characters
  - [ ] Handle multiple `-n` flags properly
  - **Test**: `echo -n -n test` should work, `echo -ntest` should not

### 🎯 **Directory Commands** (`src/builtins/cd_pwd.c`)
**Priority**: 🟡 MEDIUM | **Time**: 4-5 hours

- [ ] **`builtin_cd()`** - Change directory implementation
  - [ ] No arguments: change to HOME directory
  - [ ] Single argument: change to specified path
  - [ ] Handle relative and absolute paths
  - [ ] Update PWD and OLDPWD environment variables
  - [ ] Use `chdir()` system call
  - [ ] Handle permission errors, non-existent directories
  - **Test**: `cd /tmp; pwd` should show "/tmp"

- [ ] **`builtin_pwd()`** - Print working directory
  - [ ] Use `getcwd()` to get current directory
  - [ ] Print directory path to stdout
  - [ ] Handle `getcwd()` errors (buffer too small, etc.)
  - [ ] Return 0 on success, 1 on error
  - **Test**: `pwd` after `cd` should show correct directory

- [ ] **`update_pwd_env()`** - Environment variable maintenance
  - [ ] Set OLDPWD to previous PWD value
  - [ ] Set PWD to new current directory
  - [ ] Handle memory allocation for new values
  - [ ] Update shell environment consistently
  - **Test**: `echo $PWD` and `echo $OLDPWD` should be correct

### 🎯 **Environment Commands** (`src/builtins/env_cmds.c`)
**Priority**: 🟡 MEDIUM | **Time**: 6-7 hours

- [ ] **`builtin_export()`** - Export environment variables
  - [ ] No arguments: print all variables in "declare -x" format
  - [ ] With arguments: parse `key=value` pairs
  - [ ] Validate variable names (alphanumeric + underscore)
  - [ ] Handle `key` without value (create empty variable)
  - [ ] Update shell environment
  - [ ] Handle invalid variable names with error
  - **Test**: `export VAR=value; echo $VAR` should print "value"

- [ ] **`builtin_unset()`** - Remove environment variables
  - [ ] Accept multiple variable names as arguments
  - [ ] Remove variables from shell environment
  - [ ] Ignore non-existent variables (not an error)
  - [ ] Validate variable names
  - [ ] Don't unset readonly variables (if implemented)
  - **Test**: `export VAR=test; unset VAR; echo $VAR` should print nothing

- [ ] **`builtin_env()`** - Display environment variables
  - [ ] Print all environment variables in `key=value` format
  - [ ] Iterate through environment linked list
  - [ ] Output to stdout
  - [ ] Should not accept arguments (for simplicity)
  - [ ] Always return 0
  - **Test**: `env` should show all current environment variables

- [ ] **`builtin_exit()`** - Exit shell implementation
  - [ ] No arguments: exit with `last_exit_status`
  - [ ] Numeric argument: exit with that status code
  - [ ] Invalid argument: print error, exit with status 2
  - [ ] Perform shell cleanup before exit
  - [ ] Call `exit()` system call
  - **Test**: `exit 42; echo $?` in parent shell should show 42

---

## 📋 **PHASE 4: Execution Engine**
**Estimated Time**: 20-25 hours  
**Status**: ⭕ Not Started

### 🎯 **I/O Redirection** (`src/redirect.c`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`setup_redirections()`** - Main redirection handler
  - [ ] Process redirections in order (left to right)
  - [ ] Save original stdin/stdout file descriptors
  - [ ] Apply each redirection using `dup2()`
  - [ ] Handle redirection errors gracefully
  - [ ] Return saved FDs for restoration
  - **Test**: `echo test > file; cat file` should show "test"

- [ ] **`redirect_input()`** - Input redirection (`<`)
  - [ ] Open file for reading with `open()`
  - [ ] Check file exists and is readable
  - [ ] Return file descriptor for `dup2()` to stdin
  - [ ] Handle file errors (not found, permission denied)
  - **Test**: `cat < existing_file` should display file contents

- [ ] **`redirect_output()`** - Output redirection (`>`, `>>`)
  - [ ] Open file for writing (`O_WRONLY | O_CREAT`)
  - [ ] Use `O_TRUNC` for `>`, `O_APPEND` for `>>`
  - [ ] Set file permissions to 0644
  - [ ] Handle file creation/permission errors
  - **Test**: `echo hello > file` should create file with "hello"

- [ ] **`redirect_heredoc()`** - Heredoc implementation (`<<`)
  - [ ] Create pipe for heredoc content
  - [ ] Read lines until delimiter found
  - [ ] Write content to pipe, return read end
  - [ ] Handle readline errors and EOF
  - [ ] Don't add to command history
  - **Test**: `cat << EOF` should read until "EOF" line

- [ ] **`restore_fds()`** - Restore original file descriptors
  - [ ] Use `dup2()` to restore stdin/stdout
  - [ ] Close saved file descriptor copies
  - [ ] Handle restoration errors gracefully
  - **Usage**: Called after command execution

### 🎯 **Process Management** (`src/process.c`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`fork_and_execute()`** - Fork and exec external commands
  - [ ] Fork new process with `fork()`
  - [ ] Child: setup signals, apply redirections, exec
  - [ ] Parent: add PID to tracking list
  - [ ] Handle fork failures
  - [ ] Clean up on errors
  - **Test**: `/bin/ls` should execute and display directory

- [ ] **`wait_for_children()`** - Process synchronization
  - [ ] Wait for each child with `waitpid()`
  - [ ] Collect exit statuses
  - [ ] Handle signal termination of children
  - [ ] Update shell's `last_exit_status`
  - [ ] Clean up child PID tracking
  - **Test**: Exit status should be available in `$?`

- [ ] **`setup_pipes()`** - Pipeline pipe creation
  - [ ] Calculate number of pipes needed (n-1 for n commands)
  - [ ] Create pipe FDs with `pipe()`
  - [ ] Store pipe information for command setup
  - [ ] Handle pipe creation failures
  - [ ] Clean up pipes on error
  - **Usage**: Called before pipeline execution

- [ ] **`terminate_children()`** - Signal management
  - [ ] Send SIGTERM to all tracked children
  - [ ] Wait briefly for graceful termination
  - [ ] Send SIGKILL to remaining processes
  - [ ] Clean up child tracking list
  - **Usage**: Called on shell exit or interrupt

### 🎯 **Command Execution** (`src/executor.c`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`execute_ast()`** - Main execution coordinator
  - [ ] Determine execution type (simple, pipeline, logical)
  - [ ] Route to appropriate execution function
  - [ ] Handle execution errors
  - [ ] Return final exit status
  - [ ] Clean up execution resources
  - **Test**: All command types should execute correctly

- [ ] **`execute_pipeline()`** - Pipeline execution
  - [ ] Create pipes between commands
  - [ ] Fork process for each command in pipeline
  - [ ] Set up stdin/stdout connections via pipes
  - [ ] Close unused pipe ends
  - [ ] Wait for all processes
  - [ ] Return exit status of last command
  - **Test**: `ls | grep file | wc -l` should count matching lines

- [ ] **`execute_command()`** - Single command execution
  - [ ] Check if command is built-in
  - [ ] Built-ins: execute directly in shell
  - [ ] External: fork and exec
  - [ ] Apply redirections before execution
  - [ ] Restore file descriptors after
  - **Test**: Both built-ins and external commands work

- [ ] **`find_executable()`** - Command path resolution
  - [ ] If command contains `/`: check if file exists and executable
  - [ ] Otherwise: search PATH directories in order
  - [ ] Use `access()` to check execute permission
  - [ ] Return full path or NULL if not found
  - **Test**: `ls` should find `/bin/ls`, `./script` should work

---

## 📋 **PHASE 5: Main Program & Integration**
**Estimated Time**: 8-12 hours  
**Status**: ⭕ Not Started

### 🎯 **Main Shell Loop** (`src/main.c`)
**Priority**: 🔥 CRITICAL | **Time**: 6-8 hours

- [ ] **`main()`** - Program entry point
  - [ ] Initialize shell context
  - [ ] Set up signal handlers
  - [ ] Enter main interactive loop
  - [ ] Handle cleanup on exit
  - [ ] Return final exit status
  - **Validation**: Shell should start and accept commands

- [ ] **`shell_loop()`** - Interactive command loop
  - [ ] Display prompt with `readline()`
  - [ ] Handle EOF (Ctrl-D) to exit
  - [ ] Add non-empty input to history
  - [ ] Process signals between commands
  - [ ] Tokenize, parse, and execute input
  - [ ] Update exit status and continue
  - **Test**: Shell should be interactive and responsive

- [ ] **`init_shell()`** - Shell initialization
  - [ ] Initialize environment from system
  - [ ] Set current working directory
  - [ ] Set interactive flag based on `isatty()`
  - [ ] Initialize child process tracking
  - [ ] Set initial exit status to 0
  - **Validation**: Shell starts with correct initial state

- [ ] **`cleanup_shell()`** - Resource cleanup
  - [ ] Free environment list and arrays
  - [ ] Free current directory string
  - [ ] Terminate remaining child processes
  - [ ] Clear readline history
  - [ ] Clean up all allocated memory
  - **Validation**: No memory leaks on exit

### 🎯 **Integration & Testing**
**Priority**: 🟡 MEDIUM | **Time**: 2-4 hours

- [ ] **Built-in Integration** - Connect built-ins to executor
  - [ ] `is_builtin()` - Check if command is built-in
  - [ ] `execute_builtin()` - Route to appropriate built-in
  - [ ] Handle built-in errors and return codes
  - [ ] Built-ins should work in pipelines
  - **Test**: `echo hello | cat` should work

- [ ] **Error Handling** - Comprehensive error management
  - [ ] Command not found errors
  - [ ] Permission denied errors
  - [ ] Syntax error handling
  - [ ] Memory allocation failures
  - [ ] File I/O errors
  - **Test**: Invalid commands should show appropriate errors

- [ ] **History Integration** - Command history functionality
  - [ ] Add commands to history with `add_history()`
  - [ ] Navigate history with Up/Down arrows
  - [ ] Don't add empty commands to history
  - [ ] Don't add heredoc content to history
  - **Test**: Up arrow should recall previous commands

---

## 📋 **PHASE 6: Bonus Features** (Optional)
**Estimated Time**: 10-15 hours  
**Status**: ⭕ Not Started  
**⚠️ Only if mandatory part is perfect**

### 🎯 **Logical Operators** (`src_bonus/logical.c`)
**Priority**: 🟡 BONUS | **Time**: 6-8 hours

- [ ] **`parse_logical_expression()`** - Parse `&&` and `||`
  - [ ] Handle operator precedence (`&&` before `||`)
  - [ ] Parse parentheses for grouping
  - [ ] Build binary AST for logical expressions
  - [ ] Handle complex nested expressions
  - **Test**: `(ls && echo success) || echo fail` should work

- [ ] **`execute_logical_and()`** - Short-circuit AND execution
  - [ ] Execute left side first
  - [ ] If left succeeds (exit status 0): execute right
  - [ ] If left fails: skip right, return left's status
  - [ ] Return final exit status
  - **Test**: `false && echo skip` should not print "skip"

- [ ] **`execute_logical_or()`** - Short-circuit OR execution
  - [ ] Execute left side first
  - [ ] If left fails (exit status != 0): execute right
  - [ ] If left succeeds: skip right, return left's status
  - [ ] Return final exit status
  - **Test**: `true || echo skip` should not print "skip"

### 🎯 **Wildcard Expansion** (`src_bonus/wildcard.c`)
**Priority**: 🟡 BONUS | **Time**: 4-7 hours

- [ ] **`expand_wildcards()`** - Main wildcard expansion
  - [ ] Find arguments containing `*`
  - [ ] Match against files in current directory
  - [ ] Replace wildcard args with matched filenames
  - [ ] Sort results alphabetically
  - [ ] Handle no matches (return literal `*`)
  - **Test**: `ls *.c` should list all C files

- [ ] **`match_pattern()`** - Pattern matching algorithm
  - [ ] Implement glob pattern matching
  - [ ] Handle multiple `*` in pattern
  - [ ] Match literal characters exactly
  - [ ] `*` matches any sequence of characters
  - **Test**: `test*file` should match "test123file"

- [ ] **`get_matching_files()`** - Directory scanning
  - [ ] Open current directory with `opendir()`
  - [ ] Read all entries with `readdir()`
  - [ ] Filter entries matching wildcard pattern
  - [ ] Exclude hidden files (starting with `.`)
  - [ ] Return sorted array of matches
  - **Test**: `echo *` should list all non-hidden files

---

## 📋 **PHASE 7: Final Testing & Validation**
**Estimated Time**: 8-12 hours  
**Status**: ⭕ Not Started

### 🎯 **Comprehensive Testing**
**Priority**: 🔥 CRITICAL | **Time**: 6-8 hours

- [ ] **Basic Functionality Tests**
  - [ ] All built-in commands with various arguments
  - [ ] External command execution with PATH resolution
  - [ ] Signal handling in all scenarios
  - [ ] Quote parsing with complex nested quotes
  - [ ] Environment variable expansion
  - **Validation**: All basic tests pass

- [ ] **Advanced Functionality Tests**
  - [ ] Complex pipeline combinations
  - [ ] Multiple redirection combinations
  - [ ] Heredoc with various delimiters
  - [ ] Mixed built-ins and external commands in pipelines
  - [ ] Error handling for edge cases
  - **Validation**: Advanced features work correctly

- [ ] **Stress Testing**
  - [ ] Very long command lines
  - [ ] Many arguments (100+ arguments)
  - [ ] Deep pipeline chains (10+ commands)
  - [ ] Rapid command execution
  - [ ] Memory leak testing with valgrind
  - **Validation**: Shell remains stable under stress

- [ ] **Bash Compatibility Testing**
  - [ ] Compare behavior with bash for edge cases
  - [ ] Test same commands in both shells
  - [ ] Verify error messages match bash style
  - [ ] Check exit status codes match bash
  - **Validation**: Behavior matches bash reference

### 🎯 **Code Quality & Documentation**
**Priority**: 🟡 MEDIUM | **Time**: 2-4 hours

- [ ] **Code Review & Cleanup**
  - [ ] Remove debug code and print statements
  - [ ] Ensure consistent coding style
  - [ ] Add necessary comments
  - [ ] Optimize performance where possible
  - [ ] Check for potential security issues
  - **Validation**: Code is clean and professional

- [ ] **Memory Management Validation**
  - [ ] Run extensive valgrind testing
  - [ ] Fix any memory leaks (except readline)
  - [ ] Test with various valgrind tools
  - [ ] Ensure clean shutdown
  - **Validation**: Zero memory leaks in your code

- [ ] **Norm Compliance**
  - [ ] Run norminette on all files
  - [ ] Fix any norm violations
  - [ ] Check line length, function complexity
  - [ ] Verify proper header guards
  - **Validation**: 100% norm compliant

---

## 🎯 **Implementation Tips & Strategies**

### **Development Order Priorities**
1. **Start with environment and signals** - Core shell functionality
2. **Build parser incrementally** - Test with simple commands first
3. **Implement built-ins early** - Easier to test than external commands
4. **Add redirections carefully** - Test each type individually
5. **Implement pipes last** - Most complex feature

### **Testing Strategy**
- **Test each feature in isolation** before combining
- **Use simple commands first** (echo, pwd) before complex ones
- **Compare with bash** for expected behavior
- **Test error cases** extensively
- **Use valgrind early and often**

### **Common Pitfalls to Avoid**
- **Signal handling complexity** - Keep handler minimal
- **Memory management** - Free everything you allocate
- **File descriptor leaks** - Close all opened FDs
- **PATH resolution** - Check each directory in order
- **Quote parsing edge cases** - Test nested and escaped quotes

### **Performance Considerations**
- **Environment variable lookup** - Consider hash table for large environments
- **Command history** - Limit history size if needed
- **Pattern matching** - Optimize for common wildcard patterns
- **Process creation** - Minimize fork/exec overhead

---

## ✅ **Completion Checklist**

### **Mandatory Requirements** ✅
- [ ] Single global variable (signal number only)
- [ ] No memory leaks (excluding readline)
- [ ] All built-in commands working
- [ ] Signal handling matches bash
- [ ] Quote parsing (single and double)
- [ ] Environment variable expansion
- [ ] Redirections (all 4 types)
- [ ] Pipes working correctly
- [ ] PATH resolution working
- [ ] Error handling comprehensive

### **Bonus Requirements** ✅
- [ ] Logical operators (`&&`, `||`)
- [ ] Parentheses for precedence
- [ ] Wildcard expansion (`*`)
- [ ] Advanced command combinations

### **Quality Assurance** ✅
- [ ] Norm compliant (norminette)
- [ ] Memory leak free (valgrind)
- [ ] No crashes or undefined behavior
- [ ] Bash compatibility verified
- [ ] Comprehensive testing completed

---

**🎉 Project Completion Estimate: 60-80 hours**  
**🎯 Success Criteria: Pass all evaluation tests + bonus features working**  
**🚀 Next Steps: Begin Phase 1 - Foundation & Setup**
