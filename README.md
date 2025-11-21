# 🐚 Minishell - A Simple Shell Implementation

[![42 Project](https://img.shields.io/badge/42-Project-blue)](https://42.fr)
[![Norminette](https://img.shields.io/badge/Norminette-passing-brightgreen)]()
[![Language](https://img.shields.io/badge/Language-C-orange)]()

## 📖 Overview

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
- ⚠️ **Pipes**: Basic structure (sequential execution, needs fork/pipe/dup2)
- ⚠️ **Redirections**: Parsed but not executed (`<`, `>`, `<<`, `>>`)
- ❌ **External commands**: PATH resolution not implemented
- ❌ **Quote processing**: Quotes detected but not removed/processed
- ✅ **Variable expansion**: `$VAR`, `$?` fully implemented (18 tests passing)

### Bonus Features (Planned)
- ⚠️ **Logical operators**: `&&`, `||` (parsed, placeholder implementation)
- ⚠️ **Parentheses**: `()` grouping (parsed, not executed)
- ❌ **Wildcards**: `*` expansion not implemented

## 🏗️ Architecture

### Current Project Structure (Actual)

```
minishell/
├── 📁 include/          # Header files
│   ├── minishell.h      # Main header (function prototypes)
│   └── minishell_backup.h
│
├── 📁 src/              # Source files (41 files, ~3,200 LOC)
│   ├── main.c           # ✅ Entry point and REPL loop
│   ├── main_helpers.c   # ✅ Helper functions
│   ├── init.c           # ✅ Shell initialization
│   ├── cleanup.c        # ✅ Resource cleanup
│   ├── parser_simple.c  # Temporary simple parser
│   │
│   ├── 📁 environment/  # ✅ Environment management (4 files)
│   │   ├── env_init.c   # Initialize from envp
│   │   ├── env_get.c    # Get/find variables
│   │   ├── env_set.c    # Set/unset variables
│   │   └── env_utils.c  # List conversion, utilities
│   │
│   ├── 📁 signals/      # ✅ Signal handling (1 file)
│   │   └── signals.c    # SIGINT, SIGQUIT, EOF
│   │
│   ├── 📁 parser/       # ✅ Tokenization & parsing
│   │   └── tokenizer/   # (5 files)
│   │       ├── tokenizer.c          # Main tokenizer
│   │       ├── tokenizer_helpers.c  # Helper functions
│   │       ├── type_handlers.c      # Token type handlers
│   │       ├── token_memory.c       # Memory management
│   │       └── validate_token_list.c # Validation
│   │
│   ├── 📁 ast/          # ✅ Abstract Syntax Tree (5 files)
│   │   ├── build_tree.c        # Build binary tree from nodes
│   │   ├── build_tree_helpers.c # Tree building utilities
│   │   ├── ast_memory.c        # AST cleanup
│   │   ├── ast_debuggers.c     # Tree visualization
│   │   └── execute_ast.c       # ✅ **NEW** Execution engine
│   │
│   ├── 📁 cmd_and_ops_list/ # ✅ Command/operator list (5 files)
│   │   ├── build_list.c             # Build node list
│   │   ├── create_cmd_node.c        # Command nodes
│   │   ├── create_cmd_node_helpers.c # Command helpers
│   │   ├── create_op_node.c         # Operator nodes
│   │   └── node_memory.c            # Memory management
│   │
│   ├── 📁 builtins/     # ✅ Built-in commands (8 files)
│   │   ├── builtin_utils.c  # Command dispatcher
│   │   ├── echo.c           # echo with -n
│   │   ├── cd.c             # cd with HOME/OLDPWD
│   │   ├── pwd.c            # pwd implementation
│   │   ├── export.c         # export with validation
│   │   ├── unset.c          # unset implementation
│   │   ├── env.c            # env display
│   │   └── exit.c           # exit with validation
│   │
│   ├── 📁 expander/     # ✅ **NEW** Variable expansion (1 file)
│   │   └── var_expand.c     # $VAR and $? expansion
│   │
│   ├── 📁 debuggers/    # ✅ Debug utilities (2 files)
│   │   ├── print_tokens.c   # Token visualization
│   │   └── print_nodes.c    # Node/tree visualization
│   │
│   └── 📁 utils/        # ✅ Utility functions (6 files)
│       ├── error.c          # Error handling
│       ├── string_utils.c   # String operations
│       ├── string_utils2.c  # Additional string utils
│       ├── number_utils.c   # Number parsing
│       ├── memory_utils.c   # Memory operations
│       └── ft_split.c       # String splitting
│
├── 📁 Libft/            # ✅ 42's standard library (43 functions)
│
├── 📁 tests/phase1/     # ✅ Comprehensive test suite (14 files)
│   ├── run_all_tests.sh # Master test runner
│   ├── test_echo.sh     # 24 tests
│   ├── test_pwd.sh      # 12 tests
│   ├── test_cd.sh       # 20 tests
│   ├── test_env.sh      # 11 tests
│   ├── test_export.sh   # 16 tests
│   ├── test_unset.sh    # 13 tests
│   ├── test_exit.sh     # 17 tests
│   └── test_expansion.sh # ✅ **NEW** 18 tests
│
├── 📁 parser_tests/ast_tests/ # Parser tests (bonus features)
│
├── 📁 docs/             # ✅ Documentation (5 files)
│   ├── ARCHITECTURE.md
│   ├── COLLABORATION.md
│   ├── MODULAR_DIAGRAMS.md
│   ├── TESTING.md
│   └── WORKFLOW_DIAGRAM.md
│
├── structs.h            # ✅ Core data structures
├── minishell.h          # ✅ Function prototypes
├── PROJECT_STATUS.md    # ✅ Detailed progress tracking
├── Makefile             # ✅ Build system
└── README.md            # This file
```

### Key Statistics
- **Total Source Files**: 41 C files
- **Lines of Code**: ~3,200 (excluding Libft)
- **Test Coverage**: 113 comprehensive tests (100% passing)
- **Compilation**: Clean with `-Wall -Wextra -Werror`

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- GNU Make
- readline library (`libreadline-dev` on Ubuntu/Debian)

### Installation

```bash
# Clone the repository
git clone <your-repo-url> minishell
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

### Building

```bash
make          # Build the project
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Rebuild everything
```

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
# Note: These test AND/OR operators not yet implemented
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
   - External: TODO - PATH search + execve
   - Pipes: TODO - fork/pipe/dup2
```

### 4. What Works vs What Doesn't

**✅ Fully Working:**
- All 7 builtins with comprehensive validation
- Environment variable management
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- Tokenization with quote detection
- AST construction
- Basic execution engine

**⚠️ Partially Implemented:**
- Pipes: Structure exists, sequential execution only
- Redirections: Parsed but not executed
- AND/OR operators: Parsed, placeholder execution

**❌ Not Implemented:**
- External command execution (PATH search, execve)
- Quote removal and processing
- Variable expansion ($VAR, $?)
- Actual pipe/fork/dup2 implementation
- Redirection file operations

## 🎓 Development Status

**Current Phase**: Execution Integration (80% Complete)

### ✅ Phase 1: Foundation (Complete)
- Environment management
- Signal handling
- Utility functions
- Project structure

### ✅ Phase 2: Parsing (Complete)
- Lexical analysis (tokenizer)
- Syntax tree construction (AST)
- Token validation

### ✅ Phase 3: Built-ins (Complete - 100%)
- All 7 built-in commands implemented and tested
- 113/113 tests passing
- Comprehensive error handling

### ⚠️ Phase 4: Execution (In Progress - 40%)
- ✅ Builtin command routing and execution
- ✅ AST traversal engine
- ⚠️ External command execution (TODO)
- ⚠️ Pipeline execution (placeholder)
- ❌ I/O redirections (parsed only)
- ❌ Heredoc execution

### ⏸️ Phase 5: Integration & Testing (Pending)
- Integration testing
- Memory leak fixes
- Bash compatibility testing
- Edge case handling

### ⏸️ Phase 6: Bonus (Planned)
- Logical operators (AND/OR parsed)
- Parentheses grouping (parsed)
- Wildcard expansion

## 📊 Project Metrics

- **Lines of Code**: ~3,200 (src/) + ~1,500 (Libft) = 4,700 total
- **Test Coverage**: 113 builtin tests (100% passing)
- **Compilation**: Zero warnings with `-Wall -Wextra -Werror`
- **Memory Leaks**: None (excluding readline)
- **Files**: 41 C source files across 10 modules

## 🐛 Known Issues & Next Steps

### Next Steps (Priority Order)
1. **External Command Execution** (src/ast/execute_ast.c:85)
   - Implement PATH search
   - Add execve for external commands
   - Handle command not found errors

2. **Pipeline Implementation** (src/ast/execute_ast.c:113)
   - Add fork/pipe/dup2 logic
   - Connect stdout/stdin between commands
   - Wait for all processes

3. **Redirection Execution**
   - Implement file opening (`<`, `>`, `>>`)
   - Add heredoc processing (`<<`)
   - dup2 for file descriptor manipulation

4. **Quote Processing**
   - Remove quotes from arguments
   - Handle escaped characters

5. **Variable Expansion**
   - Implement `$VAR` expansion
   - Add `$?` for exit status
   - Handle unset variables

### Known Limitations
1. **Readline Memory**: Minimal leaks ignored per subject
2. **Parser Tests**: 0/7 passing (test bonus features)
3. **Advanced Bash**: Not in scope (aliases, job control, history expansion)

## 🤝 Team Collaboration

See [COLLABORATION.md](docs/COLLABORATION.md) for Git workflow and code review guidelines.

## 📖 Resources

### Official Documentation
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)

### Useful Guides
- [Process Management in Unix](https://www.geeksforgeeks.org/process-management-in-unix/)
- [File Descriptors Explained](https://www.bottomupcs.com/file_descriptors.xhtml)
- [Signal Handling in C](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)

### 42 Resources
- [42 Norm](https://github.com/42School/norminette)
- [Testing Guide](docs/TESTING.md)

## 🔍 Current Test Results

### ✅ Builtin Tests: 113/113 (100%)
```
echo:   24/24 ✅
pwd:    12/12 ✅
cd:     20/20 ✅
env:    11/11 ✅
export: 16/16 ✅
unset:  13/13 ✅
exit:   17/17 ✅
```

### ⏸️ Parser Tests: 0/7 (Bonus Features)
```
AND operators:  Not implemented
OR operators:   Not implemented
Parentheses:    Not implemented
```

### 📋 Integration Tests: Pending
- External commands
- Pipes
- Redirections
- Complex command chains

**Note**: Bonus is only evaluated if mandatory part is perfect!

## 👥 Authors

- [Your Name]
- [Colleague Name]

## 📜 License

This project is part of the 42 school curriculum.

## 🙏 Acknowledgments

- 42 Network for the project subject
- Bash developers for inspiration
- Peers for discussions and testing

---

**Last Updated**: November 2025  
**Version**: 1.0  
**Status**: In Development

For detailed task breakdown, see [TODO.md](TODO.md)  
For architecture details, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
