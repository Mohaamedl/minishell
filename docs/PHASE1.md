# 📚 Phase 1: Foundation - Documentation

**Status**: ✅ Complete  
**Version**: 1.0  
**Last Updated**: November 8, 2025

---

## 🎯 Overview

Phase 1 establishes the foundational components of minishell, including environment management, signal handling, utility functions, and shell initialization/cleanup.

### Components Implemented

1. **Environment Management** - Linked list for environment variables
2. **Signal Handling** - SIGINT, SIGQUIT, EOF handlers
3. **Utility Functions** - String, memory, number, error handling
4. **Shell Initialization** - State setup and cleanup
5. **Testing Framework** - Comprehensive test scripts

---

## 📁 Project Structure

```
minishell/
├── include/
│   └── minishell.h              # Main header with all prototypes
├── src/
│   ├── main.c                   # Entry point with REPL loop
│   ├── init.c                   # Shell initialization
│   ├── cleanup.c                # Resource cleanup
│   ├── environment/
│   │   ├── env_init.c          # Environment list initialization
│   │   ├── env_get.c           # Get environment variables
│   │   ├── env_set.c           # Set/unset environment variables
│   │   └── env_utils.c         # Environment utilities
│   ├── signals/
│   │   └── signals.c           # Signal handler setup
│   └── utils/
│       ├── string_utils.c      # String manipulation functions
│       ├── string_utils2.c     # Additional string functions
│       ├── ft_split.c          # String splitting
│       ├── memory_utils.c      # Memory allocation/deallocation
│       ├── number_utils.c      # Number conversion functions
│       └── error.c             # Error reporting
├── tests/phase1/
│   ├── test_all.sh             # Master test runner
│   ├── test_environment.sh     # Environment tests
│   ├── test_signals.sh         # Signal handling tests
│   └── test_utils.sh           # Utility function tests
└── Makefile                     # Build system
```

---

## 🔧 Environment Management

### Data Structure

```c
typedef struct s_env
{
    char         *key;      // Environment variable name
    char         *value;    // Environment variable value
    struct s_env *next;     // Next node in linked list
} t_env;
```

### Functions

#### `t_env *init_env(char **envp)`
Initializes environment linked list from `envp` array.

**Parameters**:
- `envp`: Array of environment strings from main()

**Returns**: Head of environment linked list, or NULL on error

**Example**:
```c
t_env *env = init_env(envp);
if (!env)
    return (ERROR);
```

#### `char *get_env_value(t_env *env, const char *key)`
Retrieves value of environment variable.

**Parameters**:
- `env`: Environment linked list
- `key`: Variable name to look up

**Returns**: Value string or NULL if not found

**Example**:
```c
char *home = get_env_value(env, "HOME");
if (home)
    printf("HOME=%s\n", home);
```

#### `int set_env_value(t_env **env, const char *key, const char *value)`
Sets or updates environment variable.

**Parameters**:
- `env`: Pointer to environment list head
- `key`: Variable name
- `value`: Variable value

**Returns**: SUCCESS (0) or ERROR (1)

**Example**:
```c
set_env_value(&env, "MY_VAR", "my_value");
```

#### `int unset_env_value(t_env **env, const char *key)`
Removes environment variable from list.

**Parameters**:
- `env`: Pointer to environment list head
- `key`: Variable name to remove

**Returns**: SUCCESS (0)

**Example**:
```c
unset_env_value(&env, "MY_VAR");
```

#### `char **env_list_to_array(t_env *env)`
Converts linked list to array format for execve().

**Parameters**:
- `env`: Environment linked list

**Returns**: NULL-terminated array of "KEY=VALUE" strings

**Example**:
```c
char **env_array = env_list_to_array(env);
execve(path, args, env_array);
free_env_array(env_array);
```

#### `void free_env(t_env *env)`
Frees entire environment linked list.

**Parameters**:
- `env`: Environment list to free

---

## ⚡ Signal Handling

### Global Variable

```c
extern volatile sig_atomic_t g_signal_received;
```

Only global variable allowed by subject. Stores last received signal number.

### Functions

#### `void setup_signals_interactive(void)`
Configures signals for interactive mode (at prompt).

**Behavior**:
- **SIGINT (Ctrl+C)**: Display new prompt on new line
- **SIGQUIT (Ctrl+\\)**: Ignored

**Example**:
```c
setup_signals_interactive();
// Now at prompt, Ctrl+C shows new prompt
```

#### `void setup_signals_executing(void)`
Configures signals during command execution.

**Behavior**:
- **SIGINT**: Ignored in parent (child handles it)
- **SIGQUIT**: Ignored in parent

**Example**:
```c
setup_signals_executing();
// Execute command...
setup_signals_interactive();
```

#### `void setup_signals_default(void)`
Restores default signal handlers (for child processes).

**Example**:
```c
pid = fork();
if (pid == 0)
{
    setup_signals_default();
    execve(path, args, env);
}
```

---

## 🛠️ Utility Functions

### String Utilities

#### `char *ft_strdup(const char *s)`
Duplicates a string.

#### `char *ft_strjoin(const char *s1, const char *s2)`
Concatenates two strings.

#### `int ft_strcmp(const char *s1, const char *s2)`
Compares two strings.

#### `int ft_strncmp(const char *s1, const char *s2, size_t n)`
Compares n characters of two strings.

#### `char **ft_split(const char *s, char c)`
Splits string by delimiter into array.

#### `size_t ft_strlen(const char *s)`
Returns string length.

#### `char *ft_strchr(const char *s, int c)`
Locates character in string.

#### `char *ft_substr(const char *s, unsigned int start, size_t len)`
Extracts substring.

### Character Type Checks

#### `int ft_isdigit(int c)`
Checks if character is digit (0-9).

#### `int ft_isalpha(int c)`
Checks if character is alphabetic (a-z, A-Z).

#### `int ft_isalnum(int c)`
Checks if character is alphanumeric.

### Memory Utilities

#### `void *ft_calloc(size_t count, size_t size)`
Allocates and zeroes memory.

#### `void *ft_memset(void *b, int c, size_t len)`
Sets memory bytes to value.

#### `void free_array(char **arr)`
Frees NULL-terminated array of strings.

### Number Utilities

#### `int ft_atoi(const char *str)`
Converts string to integer.

#### `char *ft_itoa(int n)`
Converts integer to string.

#### `int is_numeric_string(const char *str)`
Checks if string represents valid number.

### Error Handling

#### `void print_error(const char *cmd, const char *arg, const char *msg)`
Prints formatted error message to stderr.

**Format**: `minishell: cmd: arg: msg\n`

**Example**:
```c
print_error("cd", "/invalid/path", "No such file or directory");
// Output: minishell: cd: /invalid/path: No such file or directory
```

#### `void print_error_prefix(const char *cmd)`
Prints error prefix only.

---

## 🚀 Shell Initialization

### Data Structure

```c
typedef struct s_shell
{
    t_env *env_list;         // Environment variables
    int   last_exit_status;  // Last command exit status
    int   running;           // Shell running flag
} t_shell;
```

### Functions

#### `t_shell *init_shell(char **envp)`
Initializes shell state.

**Actions**:
1. Allocates shell structure
2. Initializes environment from envp
3. Increments SHLVL
4. Sets up signal handlers
5. Sets initial exit status to 0

**Returns**: Initialized shell or NULL on error

#### `void cleanup_shell(t_shell *shell)`
Cleans up all shell resources.

**Actions**:
1. Frees environment list
2. Clears readline history
3. Frees shell structure

---

## 🧪 Testing

### Running Tests

```bash
# Run all Phase 1 tests
make test

# Or manually:
bash tests/phase1/test_all.sh

# Run specific test suites
bash tests/phase1/test_environment.sh
bash tests/phase1/test_signals.sh
bash tests/phase1/test_utils.sh
```

### Test Coverage

#### Environment Tests
- ✅ Environment initialization from envp
- ✅ SHLVL increment
- ✅ Environment variable access (HOME, PATH, USER)
- ✅ Get/set/unset operations
- ✅ List to array conversion

#### Signal Tests
- ✅ SIGINT handler configuration
- ✅ SIGQUIT handler configuration
- ✅ EOF handler
- ✅ Signal context switching (interactive ↔ executing)

#### Utility Tests
- ✅ String length, comparison, duplication
- ✅ String join and split
- ✅ Number conversion (atoi, itoa)
- ✅ Character type checks
- ✅ Memory allocation and freeing

---

## 📝 Usage Examples

### Basic Shell Initialization

```c
int main(int argc, char **argv, char **envp)
{
    t_shell *shell;
    
    shell = init_shell(envp);
    if (!shell)
    {
        print_error("minishell", NULL, "initialization failed");
        return (ERROR);
    }
    
    // Main loop here...
    
    cleanup_shell(shell);
    return (SUCCESS);
}
```

### Environment Operations

```c
// Get environment variable
char *path = get_env_value(shell->env_list, "PATH");

// Set environment variable
set_env_value(&shell->env_list, "MY_VAR", "value");

// Remove environment variable
unset_env_value(&shell->env_list, "MY_VAR");

// Print all environment
print_env_list(shell->env_list);
```

### Signal Handling

```c
// At prompt
setup_signals_interactive();

// Before executing command
setup_signals_executing();

// In child process
if (fork() == 0)
{
    setup_signals_default();
    // Execute command...
}
```

---

## 🔍 Norminette Compliance

All files comply with 42 Norm:

```bash
make norm
```

**Standards**:
- ✅ Maximum 25 lines per function
- ✅ Maximum 5 functions per file
- ✅ No forbidden functions
- ✅ Proper 42 header
- ✅ Tab indentation
- ✅ Max 80 characters per line

---

## 🐛 Known Issues & Edge Cases

### Environment
- Empty environment (`envp[0] == NULL`) handled gracefully
- Variables without `=` sign are skipped
- SHLVL wraps at INT_MAX

### Signals
- Readline requires special handling for Ctrl+C
- Signal handlers must be async-signal-safe

### Memory
- All allocations checked for NULL
- Proper cleanup on error paths
- No leaks except readline (as per subject)

---

## 📊 Performance

- **Environment lookup**: O(n) - linear search through list
- **Environment set**: O(n) - may need to traverse entire list
- **Environment to array**: O(n) - single pass conversion
- **String functions**: O(n) where n is string length

**Optimization opportunities**:
- Use hash table for environment (bonus)
- Cache frequently accessed variables

---

## 🎓 Learning Outcomes

### Concepts Mastered
1. **Linked Lists**: Dynamic environment storage
2. **Signal Handling**: Async-safe programming
3. **Memory Management**: Proper allocation/deallocation
4. **String Manipulation**: Custom string library
5. **Error Handling**: Robust error reporting

### Skills Developed
- Writing clean, norminette-compliant code
- Comprehensive testing strategies
- Documentation practices
- Build system configuration (Makefile)

---

## 📚 References

- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Signal Handling (GNU C Library)](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)
- [POSIX Environment Variables](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [42 Norminette](https://github.com/42School/norminette)

---

## ✅ Checklist

- [x] Environment initialization from envp
- [x] Environment get/set/unset operations
- [x] Environment list to array conversion
- [x] SHLVL increment on init
- [x] Signal handling (SIGINT, SIGQUIT, EOF)
- [x] All utility functions implemented
- [x] Error reporting system
- [x] Shell init and cleanup
- [x] Comprehensive test suite
- [x] Norminette compliance
- [x] Documentation complete
- [x] Makefile with all rules

---

**Next Phase**: [Phase 2: Parsing](PHASE2.md)

**Status**: Ready for integration and Phase 2 development
