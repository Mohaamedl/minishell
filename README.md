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

### Mandatory Features
- ✅ Interactive prompt with command history
- ✅ Executable search via PATH or absolute/relative paths
- ✅ Quote handling (single `'` and double `"`)
- ✅ Environment variable expansion (`$VAR`, `$?`)
- ✅ Redirections: `<`, `>`, `<<`, `>>`
- ✅ Pipes: `|` for command chaining
- ✅ Signal handling: `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- ✅ Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

### Bonus Features
- ⭐ Logical operators: `&&`, `||`
- ⭐ Parentheses for command grouping
- ⭐ Wildcard expansion: `*`

## 🏗️ Architecture

```
minishell/
├── 📁 include/           # Header files
│   ├── minishell.h      # Main header with structures and prototypes
│   ├── parser.h         # Parsing-related declarations
│   ├── builtins.h       # Built-in command declarations
│   └── executor.h       # Execution engine declarations
│
├── 📁 src/              # Source files
│   ├── main.c           # Entry point and main loop
│   ├── init.c           # Shell initialization
│   ├── cleanup.c        # Resource cleanup
│   │
│   ├── 📁 environment/  # Environment management
│   │   ├── env_init.c
│   │   ├── env_get.c
│   │   ├── env_set.c
│   │   └── env_utils.c
│   │
│   ├── 📁 signals/      # Signal handling
│   │   ├── signals.c
│   │   └── signal_utils.c
│   │
│   ├── 📁 lexer/        # Tokenization
│   │   ├── tokenizer.c
│   │   ├── token_utils.c
│   │   └── quote_handler.c
│   │
│   ├── 📁 parser/       # Command parsing
│   │   ├── parser.c
│   │   ├── parse_command.c
│   │   ├── parse_redirect.c
│   │   └── parse_pipeline.c
│   │
│   ├── 📁 expander/     # Variable expansion
│   │   ├── expander.c
│   │   ├── expand_var.c
│   │   └── expand_utils.c
│   │
│   ├── 📁 executor/     # Command execution
│   │   ├── executor.c
│   │   ├── exec_simple.c
│   │   ├── exec_pipeline.c
│   │   └── exec_utils.c
│   │
│   ├── 📁 redirections/ # I/O redirection
│   │   ├── redirect.c
│   │   ├── redirect_input.c
│   │   ├── redirect_output.c
│   │   └── heredoc.c
│   │
│   ├── 📁 builtins/     # Built-in commands
│   │   ├── builtin_handler.c
│   │   ├── echo.c
│   │   ├── cd.c
│   │   ├── pwd.c
│   │   ├── export.c
│   │   ├── unset.c
│   │   ├── env.c
│   │   └── exit.c
│   │
│   ├── 📁 process/      # Process management
│   │   ├── process.c
│   │   ├── fork_exec.c
│   │   └── wait.c
│   │
│   └── 📁 utils/        # Utility functions
│       ├── error.c
│       ├── string_utils.c
│       ├── memory.c
│       └── validation.c
│
├── 📁 src_bonus/        # Bonus features
│   ├── logical_ops.c    # && and || operators
│   ├── parentheses.c    # Command grouping
│   └── wildcard.c       # * expansion
│
├── 📁 tests/            # Test suite
│   ├── test_lexer.sh
│   ├── test_parser.sh
│   ├── test_builtins.sh
│   ├── test_redirections.sh
│   ├── test_pipes.sh
│   └── test_all.sh
│
├── 📁 docs/             # Documentation
│   ├── ARCHITECTURE.md  # System design
│   ├── API.md          # Function documentation
│   ├── TESTING.md      # Testing guide
│   └── CONTRIBUTION.md # Development guidelines
│
├── Makefile            # Build system
├── README.md           # This file
├── TODO.md             # Task tracking
└── .gitignore          # Git ignore rules
```

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
make bonus    # Build with bonus features
```

## 🧪 Testing

```bash
# Run all tests
bash tests/test_all.sh

# Run specific test suites
bash tests/test_builtins.sh
bash tests/test_pipes.sh

# Check for memory leaks
valgrind --leak-check=full --show-leak-kinds=all \
         --track-fds=yes --suppressions=readline.supp ./minishell

# Compare with bash
./tests/compare_with_bash.sh
```

## 📚 Core Concepts

### 1. Signal Handling
- **SIGINT (Ctrl-C)**: Display new prompt on new line
- **SIGQUIT (Ctrl-\)**: Ignored in interactive mode
- **SIGTERM**: Graceful shutdown
- **Global variable**: Limited to storing signal number only

### 2. Parsing Pipeline
```
Input → Lexer (Tokenization) → Parser (Syntax Analysis) 
     → Expander (Variable Substitution) → Executor (Command Execution)
```

### 3. Command Execution Flow
```
1. Parse command line
2. Check if built-in command
3. If built-in: execute directly
4. If external: fork → search PATH → execve
5. Wait for child processes
6. Update exit status
```

### 4. Pipe Implementation
```
cmd1 | cmd2 | cmd3
  ↓      ↓      ↓
fork   fork   fork
  ↓      ↓      ↓
pipe connects stdout of cmd1 to stdin of cmd2
pipe connects stdout of cmd2 to stdin of cmd3
```

## 🎓 Development Phases

### Phase 1: Foundation (Week 1)
- Environment management
- Signal handling
- Utility functions
- Project structure

### Phase 2: Parsing (Week 2)
- Lexical analysis (tokenizer)
- Syntax parsing
- Variable expansion
- Quote handling

### Phase 3: Built-ins (Week 3)
- Implement all 7 built-in commands
- Test each thoroughly
- Error handling

### Phase 4: Execution (Week 4-5)
- Simple command execution
- I/O redirections
- Heredoc implementation
- Pipeline execution
- Process management

### Phase 5: Integration & Testing (Week 6)
- Integration testing
- Memory leak fixes
- Bash compatibility testing
- Edge case handling

### Phase 6: Bonus (Optional)
- Logical operators
- Wildcard expansion
- Advanced features

## 📋 Task Management

See [TODO.md](TODO.md) for detailed task breakdown and progress tracking.

## 🐛 Known Issues & Limitations

1. **Readline Memory Leaks**: Ignored as per subject requirements
2. **Advanced Bash Features**: Not implemented (aliases, job control, etc.)
3. **Signal Handling**: Simplified compared to bash

## 🤝 Team Collaboration

### Git Workflow
```bash
# Create feature branch
git checkout -b feature/parser

# Make changes and commit
git add .
git commit -m "feat: implement basic tokenizer"

# Push to remote
git push origin feature/parser

# Create pull request for review
```

### Code Review Guidelines
1. Check norm compliance: `norminette`
2. Test functionality thoroughly
3. Verify no memory leaks: `valgrind`
4. Review for edge cases
5. Ensure proper error handling

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
- Peer evaluations and discussions
- Previous student implementations (reference only)

## 🔍 Evaluation Criteria

### Mandatory (100 points)
- ✅ Norm compliance
- ✅ No crashes or undefined behavior
- ✅ No memory leaks (except readline)
- ✅ Proper error handling
- ✅ All built-ins working correctly
- ✅ Quote and variable expansion
- ✅ All redirection types
- ✅ Pipes working correctly
- ✅ Signal handling like bash
- ✅ History functionality

### Bonus (25 points)
- ⭐ Logical operators (&&, ||)
- ⭐ Parentheses for priorities
- ⭐ Wildcard expansion (*)

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
