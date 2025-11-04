# 🧪 Minishell Testing Guide

**Version**: 1.0  
**Last Updated**: November 3, 2025

---

## 📋 Table of Contents

1. [Testing Strategy](#testing-strategy)
2. [Test Categories](#test-categories)
3. [Manual Testing](#manual-testing)
4. [Memory Leak Testing](#memory-leak-testing)
5. [Bash Comparison](#bash-comparison)
6. [Test Scripts](#test-scripts)
7. [Common Test Cases](#common-test-cases)

---

## 🎯 Testing Strategy

### Testing Pyramid

```
         /\
        /  \      Unit Tests (60%)
       /────\     - Test individual functions
      /      \    - Mock dependencies
     /────────\   
    /          \  Integration Tests (30%)
   /────────────\ - Test module interactions
  /              \
 /────────────────\ End-to-End Tests (10%)
                    - Test complete commands
```

### Testing Principles

1. **Test Early, Test Often**: Don't wait until everything is done
2. **Test While Developing**: Write tests alongside code
3. **Automate When Possible**: Create scripts for repetitive tests
4. **Compare with Bash**: Use bash as reference for expected behavior
5. **Test Edge Cases**: Empty input, very long strings, special characters
6. **Test Error Handling**: Invalid input, permission errors, etc.

---

## 📂 Test Categories

### 1. Unit Tests (Individual Functions)

**Purpose**: Test functions in isolation

**Example**:
```c
// Testing ft_split function
char **result = ft_split("hello world test", ' ');
assert(result[0] equals "hello");
assert(result[1] equals "world");
assert(result[2] equals "test");
assert(result[3] equals NULL);
free_string_array(result);
```

**Modules to Unit Test**:
- String utilities (ft_strdup, ft_split, ft_strjoin)
- Environment functions (get_env_value, set_env_value)
- Validation functions (is_valid_number, is_valid_identifier)
- Token creation and management
- Memory management functions

### 2. Integration Tests (Module Interactions)

**Purpose**: Test how modules work together

**Example**:
```bash
# Test lexer → parser integration
Input: echo "hello world" | grep hello
Expected: 
  - Lexer produces correct tokens
  - Parser builds correct AST
  - No crashes or memory leaks
```

**Integration Points to Test**:
- Lexer → Parser
- Parser → Expander
- Expander → Executor
- Executor → Process Manager
- All modules → Environment Manager

### 3. End-to-End Tests (Complete Commands)

**Purpose**: Test complete command execution

**Example**:
```bash
# Test complete pipeline with redirections
./minishell << EOF
cat file.txt | grep "pattern" | wc -l > output.txt
cat output.txt
exit
EOF
```

### 4. Regression Tests

**Purpose**: Ensure bug fixes don't break again

**Process**:
1. When you fix a bug, create a test for it
2. Add test to regression suite
3. Run regression tests before each commit

---

## 🔧 Manual Testing

### Basic Command Testing

```bash
# Start minishell
./minishell

# Test 1: Simple command
ls

# Test 2: Command with arguments
ls -la

# Test 3: Absolute path
/bin/echo hello

# Test 4: Relative path (if you have a test executable)
./test_program

# Test 5: Command not found
invalid_command

# Test 6: Empty input (just press Enter)


# Test 7: Whitespace only
   

# Exit
exit
```

### Built-in Testing Checklist

#### Echo
```bash
echo hello
echo hello world
echo "hello world"
echo 'hello world'
echo -n hello
echo -n -n test
echo $USER
echo "$USER"
echo '$USER'
echo $?
```

#### CD & PWD
```bash
pwd
cd /tmp
pwd
cd ..
pwd
cd
pwd
cd /nonexistent
cd /root
```

#### Export & Env
```bash
env | grep USER
export TEST=value
echo $TEST
export MYVAR="hello world"
echo $MYVAR
export INVALID-NAME=test
export
unset TEST
echo $TEST
env | grep TEST
```

#### Exit
```bash
exit
exit 0
exit 42
exit 256
exit abc
exit 42 extra
```

### Quote Testing

```bash
echo "hello world"
echo 'hello world'
echo "it's working"
echo 'he said "hello"'
echo "user: $USER"
echo 'user: $USER'
echo "quote: \"test\""
echo "unclosed quote
```

### Redirection Testing

```bash
# Output redirection
echo hello > file.txt
cat file.txt

# Append
echo world >> file.txt
cat file.txt

# Input redirection
cat < file.txt

# Multiple redirections
cat < file.txt > output.txt
cat output.txt

# Heredoc
cat << EOF
line 1
line 2
line 3
EOF

# Heredoc to file
cat << EOF > heredoc.txt
test line 1
test line 2
EOF
cat heredoc.txt
```

### Pipeline Testing

```bash
# Simple pipe
ls | grep minishell

# Multiple pipes
cat file.txt | grep test | wc -l

# With redirections
cat file.txt | grep test > results.txt

# Built-in in pipeline
echo hello | cat
pwd | cat

# Many commands
echo test | cat | cat | cat | cat
```

### Variable Expansion Testing

```bash
echo $USER
echo $PATH
echo $HOME
echo $?
ls
echo $?
invalid_command
echo $?
echo "User: $USER, Home: $HOME"
echo 'User: $USER'
```

### Signal Testing

```bash
# Start minishell
./minishell

# Test Ctrl-C (should show new prompt)
sleep 100
^C

# Test Ctrl-\ (should do nothing)
^\

# Test Ctrl-D (should exit)
^D

# Test Ctrl-C during command
cat
^C

# Test Ctrl-C during heredoc
cat << EOF
^C
```

---

## 🔍 Memory Leak Testing

### Valgrind Basics

```bash
# Basic leak check
valgrind --leak-check=full ./minishell

# Detailed leak information
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./minishell

# Track file descriptors
valgrind --track-fds=yes ./minishell

# Save output to file
valgrind --leak-check=full \
         --log-file=valgrind-out.txt \
         ./minishell
```

### Creating Readline Suppression File

**File**: `readline.supp`
```
{
   readline_leak_1
   Memcheck:Leak
   ...
   fun:readline
}
{
   readline_leak_2
   Memcheck:Leak
   ...
   fun:add_history
}
```

**Usage**:
```bash
valgrind --leak-check=full \
         --suppressions=readline.supp \
         ./minishell
```

### Memory Test Script

**File**: `tests/test_memory.sh`
```bash
#!/bin/bash

echo "Memory leak testing with Valgrind..."

# Test 1: Simple command
echo "Test 1: Simple command"
valgrind --leak-check=full --error-exitcode=1 ./minishell << EOF
echo hello
exit
EOF

# Test 2: Pipeline
echo "Test 2: Pipeline"
valgrind --leak-check=full --error-exitcode=1 ./minishell << EOF
ls | grep minishell
exit
EOF

# Test 3: Redirections
echo "Test 3: Redirections"
valgrind --leak-check=full --error-exitcode=1 ./minishell << EOF
echo test > file.txt
cat < file.txt
exit
EOF

# Test 4: Environment
echo "Test 4: Environment"
valgrind --leak-check=full --error-exitcode=1 ./minishell << EOF
export VAR=value
echo \$VAR
unset VAR
exit
EOF

echo "Memory tests completed!"
```

### Common Memory Issues

1. **Not freeing malloc'd memory**
   - Use valgrind to find location
   - Ensure every malloc has a free

2. **Double free**
   - Set pointers to NULL after freeing
   - Check pointer before freeing

3. **Using freed memory**
   - Don't access memory after free
   - Set to NULL immediately after free

4. **File descriptor leaks**
   - Close all opened FDs
   - Use `--track-fds=yes` to find leaks

---

## ⚖️ Bash Comparison

### Comparison Script

**File**: `tests/compare_with_bash.sh`
```bash
#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

test_command() {
    local cmd="$1"
    
    echo "Testing: $cmd"
    
    # Run in bash
    bash_output=$(echo "$cmd" | bash 2>&1)
    bash_exit=$?
    
    # Run in minishell
    mini_output=$(echo "$cmd" | ./minishell 2>&1 | tail -n +2) # Skip prompt
    mini_exit=$?
    
    # Compare outputs
    if [ "$bash_output" = "$mini_output" ] && [ $bash_exit -eq $mini_exit ]; then
        echo -e "${GREEN}✓ PASS${NC}"
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Bash output: $bash_output (exit: $bash_exit)"
        echo "Mini output: $mini_output (exit: $mini_exit)"
    fi
    echo ""
}

echo "Comparing minishell with bash..."
echo ""

# Test cases
test_command "echo hello"
test_command "echo -n test"
test_command "ls | grep minishell"
test_command "cat < Makefile | wc -l"
test_command "export VAR=value; echo \$VAR"
test_command "cd /tmp; pwd"
test_command "invalid_command"

echo "Comparison complete!"
```

### Manual Comparison

```bash
# Test in bash
bash
echo $USER
exit

# Test in minishell
./minishell
echo $USER
exit

# Compare behavior
# Should be identical!
```

---

## 📝 Test Scripts

### Test Script Template

```bash
#!/bin/bash

# Test script for [feature name]

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Counters
PASSED=0
FAILED=0

# Test function
test_case() {
    local name="$1"
    local input="$2"
    local expected="$3"
    
    echo -n "Testing: $name... "
    
    actual=$(echo "$input" | ./minishell 2>&1 | tail -n +2)
    
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Expected: $expected"
        echo "  Got: $actual"
        ((FAILED++))
    fi
}

# Run tests
echo "Testing [Feature Name]"
echo "====================="

test_case "Test 1" "echo hello" "hello"
test_case "Test 2" "echo -n test" "test"

# Summary
echo ""
echo "====================="
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo "====================="

[ $FAILED -eq 0 ] && exit 0 || exit 1
```

### Built-ins Test Script

**File**: `tests/test_builtins.sh`
```bash
#!/bin/bash

echo "Testing Built-in Commands"
echo "========================="

# Test echo
echo "Testing echo..."
./minishell << 'EOF'
echo hello
echo -n test
echo "quoted string"
echo $USER
exit
EOF

# Test cd and pwd
echo "Testing cd and pwd..."
./minishell << 'EOF'
pwd
cd /tmp
pwd
cd ..
pwd
exit
EOF

# Test export and env
echo "Testing export and env..."
./minishell << 'EOF'
export TEST=value
echo $TEST
env | grep TEST
exit
EOF

# Test unset
echo "Testing unset..."
./minishell << 'EOF'
export VAR=test
echo $VAR
unset VAR
echo $VAR
exit
EOF

echo "Built-in tests completed!"
```

### Redirections Test Script

**File**: `tests/test_redirections.sh`
```bash
#!/bin/bash

echo "Testing Redirections"
echo "===================="

# Clean up
rm -f test_*.txt

# Test output redirection
echo "Testing output redirection..."
./minishell << 'EOF'
echo "test output" > test_out.txt
cat test_out.txt
exit
EOF

# Test append
echo "Testing append..."
./minishell << 'EOF'
echo "line 1" > test_append.txt
echo "line 2" >> test_append.txt
cat test_append.txt
exit
EOF

# Test input redirection
echo "Testing input redirection..."
echo "test input" > test_in.txt
./minishell << 'EOF'
cat < test_in.txt
exit
EOF

# Test heredoc
echo "Testing heredoc..."
./minishell << 'EOF'
cat << END
heredoc line 1
heredoc line 2
END
exit
EOF

# Clean up
rm -f test_*.txt

echo "Redirection tests completed!"
```

---

## 🎯 Common Test Cases

### Edge Cases

```bash
# Empty input
./minishell
[Enter]
[Enter]
exit

# Very long command (1000+ characters)
echo "a very long string..." | ./minishell

# Many arguments (100+)
echo arg1 arg2 arg3 ... arg100

# Deep pipeline (10+ commands)
echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat

# Multiple redirections
cat < in.txt > out.txt 2> err.txt

# Unclosed quotes
echo "unclosed

# Invalid syntax
| echo test
echo test |
< > echo
```

### Stress Tests

```bash
# Rapid commands
for i in {1..100}; do
    echo "echo test$i" | ./minishell
done

# Large environment
for i in {1..100}; do
    export VAR$i=value$i
done

# Many files
for i in {1..100}; do
    echo test > file$i.txt
done
cat file*.txt | wc -l
```

### Error Cases

```bash
# Command not found
./minishell
invalid_command_xyz

# Permission denied
chmod 000 file.txt
cat file.txt

# Directory as command
/usr

# No such file (redirection)
cat < nonexistent.txt

# Cannot write
echo test > /etc/readonly

# Invalid option
cd -x

# Invalid argument
exit abc
```

---

## ✅ Pre-Evaluation Checklist

### Before Submitting for Evaluation

- [ ] All mandatory features work
- [ ] No crashes with valid input
- [ ] No crashes with invalid input
- [ ] Handles edge cases gracefully
- [ ] Memory leak free (except readline)
- [ ] No file descriptor leaks
- [ ] Norminette passes
- [ ] Compiles with no warnings
- [ ] Signal handling works correctly
- [ ] All built-ins work as expected
- [ ] Redirections work correctly
- [ ] Pipes work correctly
- [ ] Quote handling works
- [ ] Variable expansion works
- [ ] Exit status is correct
- [ ] Bash comparison passes
- [ ] Test scripts all pass

### Evaluation Preparation

```bash
# Run all tests
bash tests/test_all.sh

# Check norm
norminette

# Check memory leaks
valgrind --leak-check=full ./minishell

# Compare with bash
bash tests/compare_with_bash.sh

# Clean up
make fclean

# Test from scratch
make
./minishell
```

---

## 📚 Testing Resources

### Tools

- **Valgrind**: Memory leak detection
- **GDB**: Debugging
- **Strace**: System call tracing
- **Ltrace**: Library call tracing

### Commands

```bash
# Valgrind
valgrind --leak-check=full ./minishell

# GDB
gdb ./minishell
(gdb) run
(gdb) bt  # backtrace on crash

# Strace
strace ./minishell

# Check open file descriptors
lsof -p <pid>
```

### Documentation

- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Valgrind Documentation](https://valgrind.org/docs/)
- [GDB Tutorial](https://www.gnu.org/software/gdb/documentation/)

---

**Document Version**: 1.0  
**Last Updated**: November 3, 2025  
**For**: Minishell Testing
