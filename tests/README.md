# 🧪 Minishell Test Suite

Comprehensive testing infrastructure for the minishell project.

## 📁 Directory Structure

```
tests/
├── run_all_tests.sh          # Master test runner (runs all suites)
├── tokenizer/                # Tokenizer/Lexer tests
│   └── test_tokenizer.sh     # Token parsing and classification
├── parser/                   # Parser tests
│   └── test_parser.sh        # Syntax validation
├── ast/                      # Abstract Syntax Tree tests
│   └── test_ast.sh           # AST construction and execution
├── expander/                 # Variable expansion tests
│   └── test_expansion.sh     # $VAR and $? expansion
├── builtins/                 # Built-in command tests
│   ├── test_echo.sh          # echo command (24 tests)
│   ├── test_cd.sh            # cd command (20 tests)
│   ├── test_pwd.sh           # pwd command (12 tests)
│   ├── test_env.sh           # env command (11 tests)
│   ├── test_export.sh        # export command (16 tests)
│   ├── test_unset.sh         # unset command (13 tests)
│   └── test_exit.sh          # exit command (17 tests)
├── integration/              # Integration tests (future)
└── unit/                     # Unit tests (future)
```

## 🚀 Quick Start

### Run All Tests
```bash
# From project root
bash tests/run_all_tests.sh
```

### Run Specific Test Suite
```bash
# Tokenizer tests
bash tests/tokenizer/test_tokenizer.sh

# Parser tests
bash tests/parser/test_parser.sh

# AST tests
bash tests/ast/test_ast.sh

# Variable expansion tests
bash tests/expander/test_expansion.sh

# Builtin command tests
bash tests/builtins/test_echo.sh
bash tests/builtins/test_cd.sh
# ... etc
```

## 📊 Test Coverage

### Current Test Statistics

| Module | Test Suite | Tests | Status |
|--------|-----------|-------|--------|
| **Tokenizer** | `test_tokenizer.sh` | 25+ | ✅ |
| **Parser** | `test_parser.sh` | 25+ | ✅ |
| **AST** | `test_ast.sh` | 10+ | ✅ |
| **Expander** | `test_expansion.sh` | 30+ | ✅ |
| **Builtins** | 7 test files | 113 | ✅ |
| **Total** | - | **200+** | ✅ |

### Coverage by Component

- ✅ **Tokenization**: Quote handling, operators, special characters
- ✅ **Parsing**: Syntax validation, error detection
- ✅ **AST**: Tree construction, execution order
- ✅ **Variable Expansion**: $VAR, $?, quotes, edge cases
- ✅ **Built-in Commands**: All 7 mandatory builtins
- ⏳ **Redirections**: Planned (future)
- ⏳ **Pipes**: Planned (future)
- ⏳ **External Commands**: Planned (future)

## 🔧 Test Framework

### Test Structure

Each test file follows this pattern:

```bash
#!/bin/bash

# Setup
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

TOTAL=0
PASSED=0
FAILED=0

MINISHELL="./minishell"

# Test function
test_feature() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    output=$(echo "$input" | $MINISHELL 2>&1)
    
    if [ "$output" == "$expected" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        FAILED=$((FAILED + 1))
    fi
}

# Run tests
test_feature "Simple test" "echo hello" "hello"

# Summary
echo "Total: $TOTAL | Passed: $PASSED | Failed: $FAILED"
exit $FAILED
```

### Exit Codes

- `0` - All tests passed
- `1` - At least one test failed

## 🎯 Writing New Tests

### 1. Create Test File

```bash
# Create in appropriate directory
touch tests/<module>/test_<feature>.sh
chmod +x tests/<module>/test_<feature>.sh
```

### 2. Follow Template

Use the structure from existing test files:
- Color-coded output (GREEN/RED)
- Test counters (TOTAL/PASSED/FAILED)
- Descriptive test names
- Clear expected vs actual output

### 3. Add to Master Runner

Edit `tests/run_all_tests.sh` to include your new test:

```bash
if [ -f "tests/<module>/test_<feature>.sh" ]; then
    run_suite "tests/<module>/test_<feature>.sh" "<Feature> Tests"
fi
```

## 🔄 CI/CD Integration

Tests are automatically run via GitHub Actions on:
- ✅ Push to `main`, `develop`, `feature/**` branches
- ✅ Pull requests to `main`, `develop`, `feature/**`

### Pipeline Stages

1. **Build** - Compile minishell
2. **Norminette** - Code style check
3. **Unit Tests** - Tokenizer, Parser, AST, Expander (parallel)
4. **Builtin Tests** - All 7 builtins (parallel)
5. **Integration Tests** - Full test suite
6. **Memory Check** - Valgrind leak detection
7. **Coverage Report** - Generate summary

See `.github/workflows/ci.yml` for details.

## 📝 Test Categories

### Unit Tests
- **Purpose**: Test individual components in isolation
- **Scope**: Single function/module behavior
- **Examples**: Tokenizer, Parser, AST, Expander

### Integration Tests
- **Purpose**: Test component interactions
- **Scope**: Multiple modules working together
- **Examples**: Full command execution, pipe chains

### Builtin Tests
- **Purpose**: Verify built-in command behavior
- **Scope**: Each builtin command
- **Examples**: echo, cd, pwd, env, export, unset, exit

## 🐛 Debugging Failed Tests

### View Detailed Output

```bash
# Run with verbose output
bash -x tests/<module>/test_<feature>.sh

# Run single test manually
echo "test command" | ./minishell
```

### Common Issues

1. **Binary not found**
   ```bash
   make clean && make
   ```

2. **Permission denied**
   ```bash
   chmod +x tests/**/*.sh
   ```

3. **Unexpected output**
   - Check for extra newlines
   - Verify environment variables
   - Compare with bash behavior

## 📈 Test Metrics

### Success Criteria
- ✅ All unit tests pass (100%)
- ✅ All builtin tests pass (100%)
- ✅ Integration tests pass (100%)
- ✅ No memory leaks (Valgrind clean)
- ✅ Norminette compliant

### Performance
- Average test suite execution: ~30 seconds
- Full test suite: ~2 minutes
- CI/CD pipeline: ~5 minutes

## 🤝 Contributing

### Adding Tests

1. Identify the component to test
2. Create test file in appropriate directory
3. Follow existing test patterns
4. Document test cases
5. Update this README
6. Ensure tests pass before committing

### Test Guidelines

- ✅ Test both success and failure cases
- ✅ Test edge cases and boundaries
- ✅ Use descriptive test names
- ✅ Keep tests independent
- ✅ Clean up test artifacts
- ✅ Document expected behavior

## 📚 Resources

- [Bash Test Framework](https://github.com/sstephenson/bats)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)

## 📞 Support

For issues or questions about tests:
1. Check existing test files for examples
2. Review this README
3. Open an issue on GitHub
4. Contact the development team

---

**Last Updated**: November 21, 2025  
**Test Suite Version**: 1.0.0  
**Total Tests**: 200+
