*This project has been created as part of the 42 curriculum by framiran and mhaddadi.*

# 🐚 Minishell - A Simple Shell Implementation

[![42 Project](https://img.shields.io/badge/42-Project-blue)](https://42.fr)
[![Norminette](https://img.shields.io/badge/Norminette-passing-brightgreen)]()
[![Language](https://img.shields.io/badge/Language-C-orange)]()

## 📖 Description

Minishell is a 42 school project that challenges students to create their own simplified version of bash. This project provides deep insights into:

- **Process Management**: fork, exec, wait operations
- **File Descriptors**: stdin, stdout, stderr manipulation
- **Signal Handling**: SIGINT, SIGQUIT management
- **Parsing**: Tokenization, syntax analysis, command interpretation
- **I/O Redirection**: Input/output stream manipulation
- **Pipe Implementation**: Inter-process communication

## 🎯 Project Goals

1. Build a functional shell that mimics bash behavior
2. Handle complex command parsing and execution
3. Implement proper memory management (no leaks except readline)
4. Follow 42 Norm coding standards
5. Create robust error handling
6. Implement all mandatory built-in commands

## ✨ Features

### Mandatory Features (Current Status)
- ✅ **Interactive prompt** with readline (command history, line editing)
- ✅ **Signal handling**: `Ctrl-C`, `Ctrl-D`, `Ctrl-\` (SIGINT, SIGQUIT, EOF)
- ✅ **Environment management**: Full environment variable support
- ✅ **Built-in commands**: All 7 mandatory builtins fully implemented and tested
  - `echo` (with `-n` option) - 24/24 tests passing
  - `cd` (relative/absolute paths, HOME, OLDPWD) - 20/20 tests passing
  - `pwd` - 12/12 tests passing
  - `export` (with validation and sorting) - 16/16 tests passing
  - `unset` - 13/13 tests passing
  - `env` - 11/11 tests passing
  - `exit` (with numeric validation) - 17/17 tests passing
- ✅ **Tokenization**: Full lexer with quote handling (`'` and `"`)
- ✅ **AST Parser**: Complete syntax tree construction
- ✅ **Execution Engine**: Command execution via AST traversal
- ✅ **Interactive/Non-interactive** mode detection
-  **Pipes**: Basic structure (sequential execution, needs fork/pipe/dup2)
-  **Redirections**: Parsed but not executed (`<`, `>`, `<<`, `>>`)
-  **External commands**: PATH resolution not implemented
-  **Quote processing**: Quotes detected but not removed/processed
- ✅ **Variable expansion**: `$VAR`, `$?` fully implemented (18 tests passing)

### Bonus Features (Planned)
-  **Logical operators**: `&&`, `||` (parsed, placeholder implementation)
-  **Parentheses**: `()` grouping (parsed, not executed)
-  **Wildcards**: `*` expansion not implemented

## 🏗️ Architecture

### Project Folder and File Structure

```
minishell/
├── 📁 include/              # Header files
│   ├── minishell.h           # Main header with function prototypes
│   └── structs.h             # Data structure definitions
│
├── 📁 src/                   # Source files (~68 files)
│   ├── main.c                # Entry point and REPL loop
│   ├── main_helpers.c        # Main helper functions
│   ├── init.c                # Shell initialization
│   ├── cleanup.c             # Resource cleanup
│   │
│   ├── 📁 environment/       # Environment management (4 files)
│   │   ├── env_init.c         # Initialize environment from envp
│   │   ├── env_get.c          # Get/find environment variables
│   │   ├── env_set.c          # Set/unset environment variables
│   │   └── env_utils.c        # Environment utilities
│   │
│   ├── 📁 signals/           # Signal handling (2 files)
│   │   ├── signals.c          # SIGINT, SIGQUIT, EOF handling
│   │   └── signals_heredoc.c  # Heredoc signal handling
│   │
│   ├── 📁 tokenizer/         # Lexical analysis (11 files)
│   │   ├── tokenizer.c                    # Main tokenizer
│   │   ├── create_token.c                 # Token creation
│   │   ├── create_token_helpers.c         # Token helpers
│   │   ├── tokenizer_helpers.c            # Tokenizer utilities
│   │   ├── token_memory.c                 # Token memory management
│   │   ├── handle_word_token.c            # Word token handling
│   │   ├── handle_redirection_token.c     # Redirection token handling
│   │   ├── handle_operator_token.c        # Operator token handling
│   │   ├── handle_special_token.c         # Special token handling
│   │   ├── validate_token_list.c          # Token validation
│   │   └── validate_token_list_helpers.c  # Validation helpers
│   │
│   ├── 📁 cmd_and_ops_list/  # Command/operator list (5 files)
│   │   ├── build_list.c                 # Build node list from tokens
│   │   ├── create_cmd_node.c            # Command node creation
│   │   ├── create_cmd_node_helpers.c    # Command node helpers
│   │   ├── create_op_node.c             # Operator node creation
│   │   └── node_memory.c                # Node memory management
│   │
│   ├── 📁 ast/               # Abstract Syntax Tree (5 files)
│   │   ├── build_tree.c               # Build binary tree from nodes
│   │   ├── build_tree_helpers.c       # Tree building helpers
│   │   ├── build_complete_ast.c       # Complete AST construction
│   │   ├── ast_memory.c               # AST memory management
│   │   └── ast_debuggers.c            # Tree visualization/debugging
│   │
│   ├── 📁 expander/          # Variable expansion (7 files)
│   │   ├── var_expand.c               # Variable expansion ($VAR, $?)
│   │   ├── get_var_value.c            # Get variable values
│   │   ├── one_var_expand_helpers.c   # Expansion helpers
│   │   ├── wildcard.c                 # Wildcard detection
│   │   ├── wildcard_match.c           # Pattern matching
│   │   ├── wildcard_expand.c          # Wildcard expansion
│   │   └── wildcard_args.c            # Wildcard argument handling
│   │
│   ├── 📁 execution/         # Command execution (10 files)
│   │   ├── execute_ast.c              # AST execution engine
│   │   ├── execute_cmd.c              # Command execution
│   │   ├── execute_cmd_helpers.c      # Command execution helpers
│   │   ├── pipeline_execution.c       # Pipeline execution
│   │   ├── pipeline_traversal.c       # Pipeline traversal
│   │   ├── pipeline_helpers.c         # Pipeline utilities
│   │   ├── apply_redirections.c       # Apply redirections
│   │   ├── handle_redirections.c      # Redirection handling
│   │   ├── heredoc.c                  # Heredoc implementation
│   │   └── heredoc_utils.c            # Heredoc utilities
│   │
│   ├── 📁 process/           # Process management (5 files)
│   │   ├── process.c          # Process creation (fork)
│   │   ├── wait.c             # Wait for processes
│   │   ├── exec.c             # Execute commands (execve)
│   │   ├── pipes.c            # Pipe creation and setup
│   │   └── pipes_utils.c      # Pipe utilities
│   │
│   ├── 📁 builtins/          # Built-in commands (8 files)
│   │   ├── builtin_utils.c    # Builtin dispatcher and utilities
│   │   ├── echo.c             # echo with -n option
│   │   ├── cd.c               # cd with HOME/OLDPWD support
│   │   ├── pwd.c              # pwd implementation
│   │   ├── export.c           # export with validation
│   │   ├── unset.c            # unset implementation
│   │   ├── env.c              # env display
│   │   └── exit.c             # exit with validation
│   │
│   ├── 📁 debuggers/         # Debug utilities (2 files)
│   │   ├── print_tokens.c     # Token visualization
│   │   └── print_nodes.c      # Node/tree visualization
│   │
│   └── 📁 utils/             # Utility functions (7 files)
│       ├── error.c            # Error handling
│       ├── string_utils.c     # String operations
│       ├── string_utils2.c    # Additional string utilities
│       ├── char_utils.c       # Character utilities
│       ├── number_utils.c     # Number parsing/validation
│       ├── memory_utils.c     # Memory operations
│       └── ft_split.c         # String splitting
│
├── 📁 Libft/                 # 42's standard library (43+ functions)
│   ├── Makefile
│   ├── libft.h
│   └── ft_*.c                 # Standard functions (strlen, strcmp, etc.)
│
├── 📁 tests/                 # Test suite
│   ├── run_all_tests.sh       # Master test runner
│   ├── phase1/                # Builtin tests (113 tests)
│   ├── tokenizer/             # Tokenizer tests
│   ├── parser/                # Parser tests
│   ├── ast/                   # AST tests
│   ├── expander/              # Expansion tests
│   ├── executor/              # Execution tests
│   ├── redirections/          # Redirection tests
│   ├── integration/           # Integration tests
│   └── bonus/                 # Bonus feature tests
│
├── 📁 docs/                  # Documentation
│   ├── ARCHITECTURE.md        # Architecture documentation
│   ├── COLLABORATION.md       # Git workflow and collaboration
│   ├── TESTING.md             # Testing guidelines
│   ├── WORKFLOW_DIAGRAM.md    # Workflow diagrams
│   ├── MODULAR_DIAGRAMS.md    # Module diagrams
│   ├── PROJECT_STATUS.md      # Project progress tracking
│   ├── EXECUTION_PLAN.md      # Execution implementation plan
│   ├── MEMORY_FD_MANAGEMENT.md # Memory and FD management
│   └── JIRA_STATUS.md         # Task tracking
│
├── 📁 .github/               # GitHub configurations
│
├── Makefile                  # Build system
├── README.md                 # This file
├── TODO.md                   # Task list
└── .gitignore                # Git ignore rules
```

### Key Statistics
- **Total Source Files**: 41 C files
- **Lines of Code**: ~3,200 (excluding Libft)
- **Test Coverage**: 113 comprehensive tests (100% passing)
- **Compilation**: Clean with `-Wall -Wextra -Werror`

## 📋 Instructions

### Prerequisites

- GCC compiler
- GNU Make
- readline library (`libreadline-dev` on Ubuntu/Debian)

### Installation & Compilation

```bash
# Clone the repository
git clone https://github.com/Mohaamedl/minishell.git
cd minishell

# Install readline library (if needed)
sudo apt-get install libreadline-dev  # Ubuntu/Debian
# or
brew install readline                 # macOS

# Build the project
make

# Run the shell
./minishell
```

### Compilation Commands

```bash
make          # Build the project
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Rebuild everything
make norm     # Check 42 norm compliance
make test     # Run test suite
```

### Execution

To start the minishell:
```bash
./minishell
```

The shell will display a prompt where you can enter commands. It supports:
- Built-in commands (echo, cd, pwd, export, unset, env, exit)
- External commands with PATH resolution
- Pipes (`|`)
- Redirections (`<`, `>`, `<<`, `>>`)
- Environment variable expansion (`$VAR`, `$?`)
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- Logical operators (`&&`, `||`)

## 🧪 Testing

### Builtin Tests (100% Passing ✅)
```bash
# Run all builtin tests (113 tests)
bash tests/phase1/run_all_tests.sh

# Or run individual test suites:
bash tests/phase1/test_echo.sh    # 24/24 passing
bash tests/phase1/test_pwd.sh     # 12/12 passing
bash tests/phase1/test_cd.sh      # 20/20 passing
bash tests/phase1/test_env.sh     # 11/11 passing
bash tests/phase1/test_export.sh  # 16/16 passing
bash tests/phase1/test_unset.sh   # 13/13 passing
bash tests/phase1/test_exit.sh    # 17/17 passing
```

### Parser Tests (Bonus Features)
```bash
# AST parser tests (expect failures - tests bonus features)
bash parser_tests/ast_tests/run_tests.sh
```

### Memory Leak Testing
```bash
# Check for memory leaks with valgrind
valgrind --leak-check=full --show-leak-kinds=all \
         --track-fds=yes ./minishell
```

## 📚 Core Concepts

### 1. Signal Handling
- **SIGINT (Ctrl-C)**: Display new prompt on new line
- **SIGQUIT (Ctrl-\)**: Ignored in interactive mode
- **EOF (Ctrl-D)**: Exit shell gracefully
- **Global variable**: Limited to storing signal number only

### 2. Execution Flow
```
User Input → Tokenization → Node List → AST Building → Execution
                                                           ↓
                                              Builtin or External Command
```

### 3. Current Data Flow
```
1. Read input (readline)
2. Tokenize into token list (quotes, operators, words)
3. Build command/operator node list
4. Construct binary AST (PIPE, AND, OR as internal nodes)
5. Execute AST recursively
   - Built-in: Execute directly
   - External: PATH search + execve
   - Pipes: fork/pipe/dup2
```


## 📊 Project Metrics

- **Lines of Code**: ~3,200 (src/) + ~1,500 (Libft) = 4,700 total
- **Test Coverage**: 113 builtin tests (100% passing)
- **Compilation**: Zero warnings with `-Wall -Wextra -Werror`
- **Memory Leaks**: None (excluding readline)
- **Files**: 41 C source files across 10 modules


## 📖 Resources

### Official Documentation
- [File Descriptors Explained](https://www.bottomupcs.com/file_descriptors.xhtml) - Deep dive into file descriptors
- [Signal Handling in C](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html) - Signal handling concepts
- [Pipes, Forks, and Dups](https://www.rozmichelle.com/pipes-forks-dups/) - Understanding pipes and redirection

### Articles & Learning Resources
- [How does a shell work?](https://github.com/tokenrove/build-your-own-shell) - Shell internals
- [Write a Shell in C](https://danishpraka.sh/posts/write-a-shell/) - Step-by-step guide
- [Unix Shells Tutorial](https://www.tutorialspoint.com/unix/unix-shell.htm) - Unix shell basics

### 42 Resources
- [42 Norm](https://github.com/42School/norminette) - Code style c
## 🙏 Acknowle
- 42 Network for the project subject
- Bash developers for inspiration
- Peers for discussions and testing

---

**Last Updated**: November 2025  
**Version**: 1.0  
**Status**: In Development

For detailed task breakdown, see [TODO.md](TODO.md)  
For architecture details, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
