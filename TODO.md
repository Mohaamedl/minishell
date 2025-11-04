# 🚀 Minishell - Task Tracking & Progress

**Project Duration**: 6-8 weeks  
**Team Members**: 2  
**Last Updated**: November 3, 2025

---

## 📊 Overall Progress

```
Progress: ▓░░░░░░░░░ 10% Complete

Phase 1 (Foundation):     ▓░░░░ 20%
Phase 2 (Parsing):        ░░░░░ 0%
Phase 3 (Built-ins):      ░░░░░ 0%
Phase 4 (Execution):      ░░░░░ 0%
Phase 5 (Integration):    ░░░░░ 0%
Phase 6 (Bonus):          ░░░░░ 0%
```

**Estimated Time Remaining**: 60-70 hours  
**Target Completion**: [Add your deadline]

---

## 🎯 Current Sprint

**Sprint 1**: Foundation & Setup  
**Duration**: Week 1 (Nov 3-10, 2025)  
**Focus**: Project structure, environment, signals, utilities

### Active Tasks
- [ ] Set up project structure (PRIORITY: HIGH) - **Assigned to: Both**
- [ ] Implement environment management - **Assigned to: Member 1**
- [ ] Implement signal handling - **Assigned to: Member 2**

---

## 📋 PHASE 1: Foundation & Setup
**Status**: 🟡 IN PROGRESS  
**Estimated Time**: 12-15 hours  
**Due Date**: November 10, 2025

### 🏗️ Project Structure Setup
**Priority**: 🔥 CRITICAL | **Time**: 2-3 hours | **Assigned**: Both

- [x] Create directory structure
  - [x] `include/` for headers
  - [x] `src/` with subdirectories
  - [x] `src_bonus/` for bonus features
  - [x] `tests/` for test scripts
  - [x] `docs/` for documentation
  - **Completed**: Setup basic structure

- [ ] **Create Makefile** ⏰ NEXT UP
  - [ ] Define compiler and flags (`-Wall -Wextra -Werror`)
  - [ ] Set up source file variables
  - [ ] Implement rules: `all`, `clean`, `fclean`, `re`
  - [ ] Add readline linking (`-lreadline`)
  - [ ] Add bonus rule
  - [ ] Test compilation
  - **Assigned to**: Member 1
  - **Dependencies**: None
  - **Deliverable**: Working Makefile that compiles cleanly

- [ ] **Create header files structure**
  - [ ] `minishell.h` - Main structures and core functions
  - [ ] `parser.h` - Parsing-related declarations
  - [ ] `builtins.h` - Built-in command functions
  - [ ] `executor.h` - Execution engine
  - [ ] Add include guards
  - [ ] Add necessary system includes
  - **Assigned to**: Member 2
  - **Dependencies**: None
  - **Deliverable**: Header files with all structure definitions

- [ ] **Create .gitignore**
  - [ ] Ignore `*.o`, `*.a` files
  - [ ] Ignore executable (`minishell`)
  - [ ] Ignore OS-specific files (`.DS_Store`, etc.)
  - [ ] Ignore editor files (`*.swp`, `.vscode/`, etc.)
  - **Assigned to**: Both
  - **Time**: 15 minutes

### 🌍 Environment Management (`src/environment/`)
**Priority**: 🔥 CRITICAL | **Time**: 4-5 hours | **Assigned**: Member 1

- [ ] **`env_init.c` - Initialize environment**
  - [ ] `init_environment()` - Parse `extern char **environ`
  - [ ] Create linked list of `t_env` nodes
  - [ ] Handle memory allocation failures
  - [ ] Test with various environment sizes
  - **Dependencies**: Header files
  - **Tests**: 
    - Verify all env vars loaded
    - Test with empty environment
    - Check memory allocation

- [ ] **`env_get.c` - Retrieve environment values**
  - [ ] `get_env_value()` - Search and return value
  - [ ] Handle NULL inputs
  - [ ] Case-sensitive key comparison
  - **Dependencies**: `env_init.c`
  - **Tests**: 
    - `echo $USER` should work
    - Non-existent vars return NULL
    - Test with special vars

- [ ] **`env_set.c` - Set/update environment**
  - [ ] `set_env_value()` - Update or create variable
  - [ ] `unset_env_value()` - Remove variable
  - [ ] Update env array for execve
  - [ ] Handle memory correctly
  - **Dependencies**: `env_get.c`
  - **Tests**:
    - `export TEST=value; echo $TEST`
    - Update existing variable
    - Unset and verify removal

- [ ] **`env_utils.c` - Helper functions**
  - [ ] `env_list_to_array()` - Convert for execve
  - [ ] `free_env_list()` - Cleanup function
  - [ ] `duplicate_env_node()` - Copy function
  - **Dependencies**: Previous env files
  - **Tests**: Memory leak checks

### 📡 Signal Handling (`src/signals/`)
**Priority**: 🔥 CRITICAL | **Time**: 3-4 hours | **Assigned**: Member 2

- [ ] **Global variable setup**
  - [ ] Declare `int g_signal_num` in header
  - [ ] Document its limited purpose
  - [ ] Initialize in main
  - **Note**: ONLY global variable allowed!
  - **Validation**: Review with peer

- [ ] **`signals.c` - Signal handlers**
  - [ ] `signal_handler()` - Main handler function
    - [ ] Handle SIGINT (Ctrl-C)
    - [ ] Handle SIGQUIT (Ctrl-\)
    - [ ] Use only signal-safe functions
  - [ ] `setup_signals()` - Configure for shell
    - [ ] Set up sigaction structures
    - [ ] Save original handlers
  - [ ] `setup_child_signals()` - Reset for children
  - [ ] `restore_signals()` - Cleanup
  - **Dependencies**: None
  - **Tests**:
    - Ctrl-C shows new prompt
    - Ctrl-\ does nothing in interactive mode
    - Children can be interrupted

### 🛠️ Utility Functions (`src/utils/`)
**Priority**: 🟡 MEDIUM | **Time**: 2-3 hours | **Assigned**: Both (pair programming)

- [ ] **`string_utils.c` - String operations**
  - [ ] `ft_strdup()` - Safe string duplication
  - [ ] `ft_strjoin()` - Join two strings
  - [ ] `ft_strjoin_sep()` - Join with separator
  - [ ] `ft_split()` - Split by delimiter
  - [ ] `ft_strcmp()` / `ft_strncmp()` - Comparison
  - **Tests**: Unit test each function

- [ ] **`memory.c` - Memory management**
  - [ ] `free_string_array()` - Free string arrays
  - [ ] `safe_malloc()` - Malloc with error check
  - [ ] `safe_free()` - Safe free wrapper
  - **Tests**: Valgrind verification

- [ ] **`validation.c` - Input validation**
  - [ ] `is_valid_number()` - Numeric validation
  - [ ] `is_valid_identifier()` - Variable name check
  - [ ] `is_whitespace()` - Whitespace check
  - **Tests**: Edge cases and invalid inputs

- [ ] **`error.c` - Error handling**
  - [ ] `print_error()` - Standard error output
  - [ ] `handle_error()` - Error handler
  - [ ] Error message constants
  - **Tests**: Verify error messages match bash

### 📄 Documentation
**Priority**: 🟢 LOW | **Time**: 2 hours | **Assigned**: Split

- [ ] **`docs/ARCHITECTURE.md`**
  - [ ] System design overview
  - [ ] Data structure documentation
  - [ ] Module interactions
  - **Assigned to**: Member 1

- [ ] **`docs/API.md`**
  - [ ] Function documentation
  - [ ] Parameter descriptions
  - [ ] Return values
  - [ ] Usage examples
  - **Assigned to**: Member 2

---

## 📋 PHASE 2: Lexical Analysis & Parsing
**Status**: ⭕ NOT STARTED  
**Estimated Time**: 18-22 hours  
**Planned Start**: November 10, 2025

### 🔤 Tokenizer (`src/lexer/`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`tokenizer.c` - Main tokenization**
  - [ ] `tokenize()` - Entry point
  - [ ] `skip_whitespace()` - Skip spaces/tabs
  - [ ] `identify_token_type()` - Determine token type
  - [ ] Create token linked list
  - **Key Features**:
    - Recognize operators: `|`, `<`, `>`, `<<`, `>>`, `&&`, `||`, `(`, `)`
    - Handle quoted strings
    - Extract words (commands/arguments)
  - **Tests**:
    - Simple: `echo hello`
    - Quotes: `echo "hello world"`
    - Pipes: `cat file | grep test`
    - Operators: `ls && echo success`

- [ ] **`token_utils.c` - Token management**
  - [ ] `new_token()` - Create token
  - [ ] `add_token()` - Add to list
  - [ ] `free_tokens()` - Cleanup
  - [ ] `print_tokens()` - Debug function

- [ ] **`quote_handler.c` - Quote processing**
  - [ ] `extract_single_quoted()` - Handle `'...'`
  - [ ] `extract_double_quoted()` - Handle `"..."`
  - [ ] `handle_unclosed_quote()` - Error handling
  - [ ] Preserve spaces in quotes
  - **Tests**:
    - `echo 'don'\''t'`
    - `echo "User: $USER"`
    - Unclosed quote detection

### 🌳 Parser (`src/parser/`)
**Priority**: 🔥 CRITICAL | **Time**: 10-12 hours

- [ ] **`parser.c` - Main parser**
  - [ ] `parse_command_line()` - Top-level parsing
  - [ ] `parse_logical_expression()` - Handle &&, ||
  - [ ] `create_ast()` - Build abstract syntax tree
  - [ ] Syntax error detection
  - **Tests**: Complex command combinations

- [ ] **`parse_command.c` - Command parsing**
  - [ ] `parse_simple_command()` - Single command
  - [ ] `extract_command_name()` - First word
  - [ ] `extract_arguments()` - Remaining words
  - [ ] NULL-terminate args array

- [ ] **`parse_redirect.c` - Redirection parsing**
  - [ ] `parse_redirection()` - Main function
  - [ ] Handle `<`, `>`, `>>`, `<<`
  - [ ] Extract filename/delimiter
  - [ ] Create redirection structures
  - [ ] Link multiple redirections

- [ ] **`parse_pipeline.c` - Pipeline parsing**
  - [ ] `parse_pipeline()` - Handle `|`
  - [ ] Create pipeline structure
  - [ ] Count commands in pipeline
  - [ ] Error handling

### 🔄 Variable Expansion (`src/expander/`)
**Priority**: 🔥 CRITICAL | **Time**: 4-5 hours

- [ ] **`expander.c` - Main expansion**
  - [ ] `expand_variables()` - Entry point
  - [ ] Find `$` in strings
  - [ ] Handle `$?` (exit status)
  - [ ] Handle `$$` (process ID)
  - [ ] Respect quote context

- [ ] **`expand_var.c` - Variable substitution**
  - [ ] Extract variable name
  - [ ] Lookup in environment
  - [ ] Replace in string
  - [ ] Handle non-existent vars

- [ ] **`expand_utils.c` - Helper functions**
  - [ ] `is_expandable()` - Check context
  - [ ] `extract_var_name()` - Get var name
  - [ ] `build_expanded_string()` - Construct result

---

## 📋 PHASE 3: Built-in Commands
**Status**: ⭕ NOT STARTED  
**Estimated Time**: 12-15 hours  
**Planned Start**: November 17, 2025

### 🔧 Built-in Handler (`src/builtins/`)
**Priority**: 🔥 CRITICAL

- [ ] **`builtin_handler.c` - Dispatcher**
  - [ ] `is_builtin()` - Check if command is built-in
  - [ ] `execute_builtin()` - Route to correct built-in
  - [ ] Return exit status
  - **Time**: 1 hour

### Individual Built-ins
**Each built-in should be thoroughly tested**

- [ ] **`echo.c`** - Echo command
  - **Priority**: 🟢 EASY | **Time**: 2-3 hours
  - [ ] Handle `-n` option
  - [ ] Print arguments with spaces
  - [ ] Handle multiple `-n` flags
  - **Tests**:
    - `echo hello world`
    - `echo -n test`
    - `echo -n -n test`
    - `echo "$USER"`

- [ ] **`cd.c`** - Change directory
  - **Priority**: 🟡 MEDIUM | **Time**: 3-4 hours
  - [ ] No args: go to HOME
  - [ ] Relative paths
  - [ ] Absolute paths
  - [ ] Update PWD and OLDPWD
  - [ ] Error handling (permission, not found)
  - **Tests**:
    - `cd /tmp; pwd`
    - `cd ..; pwd`
    - `cd` (go home)
    - `cd nonexistent` (error)

- [ ] **`pwd.c`** - Print working directory
  - **Priority**: 🟢 EASY | **Time**: 1 hour
  - [ ] Use `getcwd()`
  - [ ] Print to stdout
  - [ ] Error handling
  - **Tests**: Verify after cd commands

- [ ] **`export.c`** - Export variables
  - **Priority**: 🟡 MEDIUM | **Time**: 3-4 hours
  - [ ] No args: print all vars
  - [ ] Parse `key=value`
  - [ ] Validate variable names
  - [ ] Handle `key` without value
  - [ ] Update environment
  - **Tests**:
    - `export VAR=value`
    - `export INVALID-NAME=test` (error)
    - `export` (list all)

- [ ] **`unset.c`** - Unset variables
  - **Priority**: 🟢 EASY | **Time**: 1-2 hours
  - [ ] Remove variables
  - [ ] Handle multiple args
  - [ ] Ignore non-existent vars
  - **Tests**: `export VAR=test; unset VAR; echo $VAR`

- [ ] **`env.c`** - Display environment
  - **Priority**: 🟢 EASY | **Time**: 1 hour
  - [ ] Print all vars in `key=value` format
  - [ ] No arguments
  - **Tests**: Verify output format

- [ ] **`exit.c`** - Exit shell
  - **Priority**: 🟢 EASY | **Time**: 1-2 hours
  - [ ] No args: exit with last status
  - [ ] Numeric arg: exit with that status
  - [ ] Invalid arg: error and exit(2)
  - [ ] Cleanup before exit
  - **Tests**:
    - `exit`
    - `exit 42`
    - `exit abc` (error)

---

## 📋 PHASE 4: Execution Engine
**Status**: ⭕ NOT STARTED  
**Estimated Time**: 20-25 hours  
**Planned Start**: November 24, 2025

### 🔀 I/O Redirection (`src/redirections/`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`redirect.c` - Main redirection handler**
  - [ ] `setup_redirections()` - Process all redirections
  - [ ] Save original stdin/stdout
  - [ ] Apply redirections in order
  - [ ] Error handling
  - **Time**: 2 hours

- [ ] **`redirect_input.c` - Input redirection**
  - [ ] Handle `<` operator
  - [ ] Open file for reading
  - [ ] `dup2()` to stdin
  - [ ] Error: file not found, permission
  - **Time**: 1-2 hours
  - **Tests**: `cat < file`

- [ ] **`redirect_output.c` - Output redirection**
  - [ ] Handle `>` operator (truncate)
  - [ ] Handle `>>` operator (append)
  - [ ] Create file if not exists (0644)
  - [ ] `dup2()` to stdout
  - **Time**: 2-3 hours
  - **Tests**:
    - `echo test > file`
    - `echo more >> file`

- [ ] **`heredoc.c` - Heredoc implementation**
  - [ ] Handle `<<` operator
  - [ ] Create pipe
  - [ ] Read until delimiter
  - [ ] Don't add to history
  - [ ] Handle Ctrl-C during input
  - **Time**: 3-4 hours
  - **Tests**:
    - `cat << EOF`
    - `cat << "EOF"` (no expansion)

- [ ] **`restore_fds()` - Restore file descriptors**
  - [ ] Restore stdin/stdout
  - [ ] Close saved FDs
  - **Time**: 30 minutes

### ⚙️ Process Management (`src/process/`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`process.c` - Process coordination**
  - [ ] `create_child_process()` - Fork wrapper
  - [ ] Track child PIDs
  - [ ] `terminate_children()` - Signal all children
  - **Time**: 2-3 hours

- [ ] **`fork_exec.c` - Fork and execute**
  - [ ] `fork_and_execute()` - Main function
  - [ ] Child: setup signals, redirections, exec
  - [ ] Parent: track PID
  - [ ] Handle fork failures
  - [ ] `find_executable()` - PATH resolution
  - **Time**: 3-4 hours
  - **Tests**: External command execution

- [ ] **`wait.c` - Process synchronization**
  - [ ] `wait_for_children()` - Wait for all
  - [ ] Collect exit statuses
  - [ ] Handle signals (SIGINT, SIGTERM)
  - [ ] Update `$?`
  - **Time**: 2-3 hours

- [ ] **Pipeline support**
  - [ ] `setup_pipes()` - Create pipes
  - [ ] Connect stdin/stdout via pipes
  - [ ] Close unused pipe ends
  - **Time**: 3-4 hours

### 🎬 Command Execution (`src/executor/`)
**Priority**: 🔥 CRITICAL | **Time**: 8-10 hours

- [ ] **`executor.c` - Main execution coordinator**
  - [ ] `execute_ast()` - Entry point
  - [ ] Route to correct execution type
  - [ ] Return final exit status
  - **Time**: 2 hours

- [ ] **`exec_simple.c` - Simple command execution**
  - [ ] `execute_simple_command()` - Single command
  - [ ] Check if built-in
  - [ ] Apply redirections
  - [ ] Execute (built-in or external)
  - [ ] Restore FDs
  - **Time**: 3-4 hours

- [ ] **`exec_pipeline.c` - Pipeline execution**
  - [ ] `execute_pipeline()` - Main function
  - [ ] Create pipes between commands
  - [ ] Fork for each command
  - [ ] Set up stdin/stdout connections
  - [ ] Wait for all processes
  - [ ] Return last command status
  - **Time**: 4-5 hours
  - **Tests**:
    - `ls | grep file`
    - `cat file | grep test | wc -l`
    - `echo test | cat | cat | cat`

- [ ] **`exec_utils.c` - Helper functions**
  - [ ] `is_executable()` - Check exec permission
  - [ ] `build_exec_args()` - Prepare execve args
  - [ ] PATH resolution logic
  - **Time**: 1-2 hours

---

## 📋 PHASE 5: Integration & Testing
**Status**: ⭕ NOT STARTED  
**Estimated Time**: 8-12 hours  
**Planned Start**: December 1, 2025

### 🔗 Integration (`src/`)
**Priority**: 🔥 CRITICAL | **Time**: 4-6 hours

- [ ] **`main.c` - Main program**
  - [ ] Program entry point
  - [ ] Parse command line arguments (if any)
  - [ ] Initialize shell
  - [ ] Enter shell loop
  - [ ] Cleanup on exit
  - **Time**: 2 hours

- [ ] **`init.c` - Shell initialization**
  - [ ] `init_shell()` - Initialize context
  - [ ] Load environment
  - [ ] Setup signals
  - [ ] Set current directory
  - [ ] Initialize history
  - **Time**: 1-2 hours

- [ ] **`cleanup.c` - Resource cleanup**
  - [ ] `cleanup_shell()` - Free all resources
  - [ ] Free environment
  - [ ] Free command structures
  - [ ] Clear history
  - [ ] Close file descriptors
  - **Time**: 1-2 hours

- [ ] **Shell loop**
  - [ ] Display prompt (readline)
  - [ ] Handle EOF (Ctrl-D)
  - [ ] Add to history
  - [ ] Process input
  - [ ] Update exit status
  - **Time**: 1 hour

### 🧪 Comprehensive Testing
**Priority**: 🔥 CRITICAL | **Time**: 6-8 hours

- [ ] **Basic functionality tests**
  - [ ] All built-ins with various inputs
  - [ ] External commands
  - [ ] Quote parsing
  - [ ] Variable expansion
  - [ ] Signal handling
  - **Time**: 2 hours
  - **Create**: `tests/test_builtins.sh`

- [ ] **Advanced functionality tests**
  - [ ] Pipelines (simple and complex)
  - [ ] All redirection types
  - [ ] Heredoc with different delimiters
  - [ ] Mixed commands
  - [ ] Error cases
  - **Time**: 2 hours
  - **Create**: `tests/test_advanced.sh`

- [ ] **Edge cases**
  - [ ] Empty input
  - [ ] Very long commands
  - [ ] Many arguments (100+)
  - [ ] Deep pipelines (10+)
  - [ ] Invalid syntax
  - [ ] Permission errors
  - **Time**: 1-2 hours
  - **Create**: `tests/test_edge_cases.sh`

- [ ] **Memory leak testing**
  - [ ] Run all tests with valgrind
  - [ ] Fix any leaks in your code
  - [ ] Create suppression file for readline
  - [ ] Document known readline leaks
  - **Time**: 2-3 hours
  - **Commands**:
    ```bash
    valgrind --leak-check=full ./minishell
    valgrind --show-leak-kinds=all ./minishell
    ```

- [ ] **Bash compatibility**
  - [ ] Compare outputs with bash
  - [ ] Match error messages
  - [ ] Verify exit statuses
  - [ ] Test same commands in both
  - **Time**: 1 hour
  - **Create**: `tests/compare_with_bash.sh`

### 📝 Documentation Completion
**Priority**: 🟡 MEDIUM | **Time**: 2 hours

- [ ] **Complete all code comments**
  - [ ] Function descriptions
  - [ ] Complex logic explanations
  - [ ] Edge case handling notes

- [ ] **Update documentation**
  - [ ] `docs/ARCHITECTURE.md`
  - [ ] `docs/API.md`
  - [ ] `docs/TESTING.md`
  - [ ] `README.md`

- [ ] **Create user guide**
  - [ ] Usage examples
  - [ ] Common issues
  - [ ] Troubleshooting

---

## 📋 PHASE 6: Bonus Features (Optional)
**Status**: ⭕ NOT STARTED  
**Estimated Time**: 10-15 hours  
**⚠️ ONLY if mandatory part is PERFECT**

### ⚡ Logical Operators (`src_bonus/`)
**Priority**: 🟡 BONUS | **Time**: 6-8 hours

- [ ] **`logical_ops.c` - && and || operators**
  - [ ] `execute_logical_and()` - Short-circuit AND
  - [ ] `execute_logical_or()` - Short-circuit OR
  - [ ] Operator precedence handling
  - [ ] **Tests**:
    - `true && echo success` (prints)
    - `false && echo skip` (doesn't print)
    - `false || echo fallback` (prints)
    - `true || echo skip` (doesn't print)

- [ ] **`parentheses.c` - Command grouping**
  - [ ] Parse parentheses
  - [ ] Handle nested groups
  - [ ] Priority/precedence
  - [ ] **Tests**:
    - `(ls && echo ok) || echo fail`
    - `(false || true) && echo success`

### 🌟 Wildcard Expansion (`src_bonus/`)
**Priority**: 🟡 BONUS | **Time**: 4-7 hours

- [ ] **`wildcard.c` - * expansion**
  - [ ] `expand_wildcards()` - Main function
  - [ ] `match_pattern()` - Pattern matching
  - [ ] `get_matching_files()` - Directory scan
  - [ ] Sort results alphabetically
  - [ ] Handle no matches (return literal `*`)
  - [ ] Exclude hidden files
  - [ ] **Tests**:
    - `ls *.c`
    - `echo test*`
    - `cat *.txt`

---

## 🎯 Quality Assurance Checklist

### Before Each Commit
- [ ] Code compiles without warnings
- [ ] Norminette passes (no errors)
- [ ] No obvious memory leaks
- [ ] Functionality tested manually
- [ ] Comments added where needed

### Before Push
- [ ] All tests pass
- [ ] Valgrind check performed
- [ ] Code reviewed by peer
- [ ] Documentation updated
- [ ] TODO.md updated

### Before Evaluation
- [ ] ✅ Complete functionality test
- [ ] ✅ Memory leak verification (valgrind)
- [ ] ✅ Norm compliance (norminette)
- [ ] ✅ Compare with bash behavior
- [ ] ✅ Test all edge cases
- [ ] ✅ Check error handling
- [ ] ✅ Verify signal handling
- [ ] ✅ Test with evaluator commands
- [ ] ✅ Documentation complete
- [ ] ✅ Code clean (no debug prints)

---

## 📊 Team Task Assignment

### Member 1 Focus Areas
- Environment management
- Parser implementation
- Built-ins: echo, export, env
- Redirection implementation
- Documentation

### Member 2 Focus Areas
- Signal handling
- Lexer/Tokenizer
- Built-ins: cd, pwd, unset, exit
- Process management
- Pipeline implementation

### Shared Responsibilities
- Utility functions (pair programming)
- Integration and testing
- Code review
- Bug fixing
- Bonus features

---

## 🐛 Known Issues & Bugs

### Active Issues
*No active issues yet*

### Resolved Issues
*None yet*

---

## 📈 Time Tracking

### Week 1 (Nov 3-10)
- **Planned**: Foundation & Setup (12-15 hours)
- **Actual**: [To be filled]
- **Variance**: [To be calculated]

### Week 2 (Nov 10-17)
- **Planned**: Parsing (18-22 hours)
- **Actual**: [To be filled]

### Week 3 (Nov 17-24)
- **Planned**: Built-ins (12-15 hours)
- **Actual**: [To be filled]

### Week 4-5 (Nov 24-Dec 1)
- **Planned**: Execution (20-25 hours)
- **Actual**: [To be filled]

### Week 6 (Dec 1-8)
- **Planned**: Integration & Testing (8-12 hours)
- **Actual**: [To be filled]

---

## 💡 Notes & Reminders

### Important Points
- ⚠️ **Only ONE global variable allowed** (signal number)
- ⚠️ **Readline leaks are OK**, but your code must be leak-free
- ⚠️ **Bonus only if mandatory is perfect**
- 📝 **Compare with bash** for expected behavior
- 🧪 **Test early and often**
- 👥 **Code review is mandatory** before merging

### Common Pitfalls
- Signal handler complexity (keep it minimal)
- File descriptor leaks (close all FDs)
- Quote parsing edge cases
- PATH resolution order
- Pipeline execution order
- Memory management in error cases

### Best Practices
- Write tests before/during implementation
- Use valgrind frequently
- Commit small, logical changes
- Document as you go
- Ask for help early
- Review each other's code daily

---

**Last Updated**: November 3, 2025  
**Next Review**: November 10, 2025  
**Status**: Project setup in progress

For architectural details, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)  
For API documentation, see [docs/API.md](docs/API.md)
