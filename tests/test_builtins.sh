#!/bin/bash

# ============================================================================
# MINISHELL BUILT-IN TESTS
# ============================================================================
# 
# Comprehensive test suite for all 7 built-in commands.
# Tests each command's functionality, edge cases, and error handling.
#
# Usage: ./test_builtins.sh
# 
# Author: mohammoh
# Date: 2025-11-06
# Related Jira: KAN-60
# ============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell binary path
MINISHELL="./minishell"

# ============================================================================
# HELPER FUNCTIONS
# ============================================================================

print_header() {
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
}

print_test() {
    echo -e "${YELLOW}Testing:${NC} $1"
}

print_pass() {
    echo -e "${GREEN}✓ PASS${NC}: $1"
    ((PASSED_TESTS++))
    ((TOTAL_TESTS++))
}

print_fail() {
    echo -e "${RED}✗ FAIL${NC}: $1"
    echo -e "${RED}  Expected: $2${NC}"
    echo -e "${RED}  Got: $3${NC}"
    ((FAILED_TESTS++))
    ((TOTAL_TESTS++))
}

# Run command in minishell and capture output
run_minishell() {
    local cmd="$1"
    echo -e "$cmd\nexit" | $MINISHELL 2>&1 | grep -v "minishell\$" | grep -v "^exit$"
}

# ============================================================================
# ECHO TESTS
# ============================================================================

test_echo() {
    print_header "TESTING: echo Built-in"
    
    # Test 1: Basic echo
    print_test "echo hello world"
    output=$(run_minishell "echo hello world")
    expected="hello world"
    if [ "$output" = "$expected" ]; then
        print_pass "Basic echo works"
    else
        print_fail "Basic echo" "$expected" "$output"
    fi
    
    # Test 2: echo with -n option
    print_test "echo -n test"
    output=$(run_minishell "echo -n test")
    expected="test"
    if [ "$output" = "$expected" ]; then
        print_pass "echo -n works"
    else
        print_fail "echo -n" "$expected" "$output"
    fi
    
    # Test 3: echo with multiple -n options
    print_test "echo -n -n -n test"
    output=$(run_minishell "echo -n -n -n test")
    expected="test"
    if [ "$output" = "$expected" ]; then
        print_pass "Multiple -n options work"
    else
        print_fail "Multiple -n" "$expected" "$output"
    fi
    
    # Test 4: echo with -nnn option
    print_test "echo -nnn test"
    output=$(run_minishell "echo -nnn test")
    expected="test"
    if [ "$output" = "$expected" ]; then
        print_pass "echo -nnn works"
    else
        print_fail "echo -nnn" "$expected" "$output"
    fi
    
    # Test 5: Empty echo
    print_test "echo (empty)"
    output=$(run_minishell "echo")
    expected=""
    if [ "$output" = "$expected" ]; then
        print_pass "Empty echo works"
    else
        print_fail "Empty echo" "blank line" "$output"
    fi
    
    # Test 6: echo with quotes
    print_test "echo \"hello world\""
    output=$(run_minishell "echo \"hello world\"")
    expected="hello world"
    if [ "$output" = "$expected" ]; then
        print_pass "echo with quotes works"
    else
        print_fail "echo with quotes" "$expected" "$output"
    fi
    
    echo ""
}

# ============================================================================
# PWD TESTS
# ============================================================================

test_pwd() {
    print_header "TESTING: pwd Built-in"
    
    # Test 1: Basic pwd
    print_test "pwd"
    output=$(run_minishell "pwd")
    expected=$(pwd)
    if [ "$output" = "$expected" ]; then
        print_pass "Basic pwd works"
    else
        print_fail "Basic pwd" "$expected" "$output"
    fi
    
    # Test 2: pwd after cd
    print_test "pwd after cd /tmp"
    output=$(run_minishell "cd /tmp\npwd")
    expected="/tmp"
    if [ "$output" = "$expected" ]; then
        print_pass "pwd after cd works"
    else
        print_fail "pwd after cd" "$expected" "$output"
    fi
    
    echo ""
}

# ============================================================================
# CD TESTS
# ============================================================================

test_cd() {
    print_header "TESTING: cd Built-in"
    
    # Test 1: cd to /tmp
    print_test "cd /tmp && pwd"
    output=$(run_minishell "cd /tmp\npwd")
    expected="/tmp"
    if [ "$output" = "$expected" ]; then
        print_pass "cd to absolute path works"
    else
        print_fail "cd to absolute path" "$expected" "$output"
    fi
    
    # Test 2: cd ..
    print_test "cd /tmp && cd .. && pwd"
    output=$(run_minishell "cd /tmp\ncd ..\npwd")
    expected="/"
    if [ "$output" = "$expected" ]; then
        print_pass "cd .. works"
    else
        print_fail "cd .." "$expected" "$output"
    fi
    
    # Test 3: cd with no arguments (should go to HOME)
    print_test "cd (no args)"
    output=$(run_minishell "cd\npwd")
    expected="$HOME"
    if [ "$output" = "$expected" ]; then
        print_pass "cd with no args works"
    else
        print_fail "cd no args" "$expected" "$output"
    fi
    
    # Test 4: cd to nonexistent directory
    print_test "cd to nonexistent directory"
    output=$(run_minishell "cd /nonexistent 2>&1")
    if echo "$output" | grep -q "No such file or directory"; then
        print_pass "cd to nonexistent gives error"
    else
        print_fail "cd nonexistent" "error message" "$output"
    fi
    
    echo ""
}

# ============================================================================
# ENV TESTS
# ============================================================================

test_env() {
    print_header "TESTING: env Built-in"
    
    # Test 1: env displays variables
    print_test "env"
    output=$(run_minishell "env")
    if echo "$output" | grep -q "PATH="; then
        print_pass "env displays PATH"
    else
        print_fail "env displays PATH" "PATH=..." "$output"
    fi
    
    # Test 2: env after export
    print_test "env after export"
    output=$(run_minishell "export TEST_VAR=hello\nenv")
    if echo "$output" | grep -q "TEST_VAR=hello"; then
        print_pass "env shows exported variable"
    else
        print_fail "env after export" "TEST_VAR=hello" "$output"
    fi
    
    # Test 3: env after unset
    print_test "env after unset"
    output=$(run_minishell "export TEST_VAR=hello\nunset TEST_VAR\nenv")
    if ! echo "$output" | grep -q "TEST_VAR"; then
        print_pass "env doesn't show unset variable"
    else
        print_fail "env after unset" "no TEST_VAR" "$output"
    fi
    
    echo ""
}

# ============================================================================
# EXPORT TESTS
# ============================================================================

test_export() {
    print_header "TESTING: export Built-in"
    
    # Test 1: export new variable
    print_test "export VAR=value"
    output=$(run_minishell "export VAR=value\nenv")
    if echo "$output" | grep -q "VAR=value"; then
        print_pass "export sets new variable"
    else
        print_fail "export new variable" "VAR=value" "$output"
    fi
    
    # Test 2: export without value
    print_test "export VAR (no value)"
    output=$(run_minishell "export VAR\nexport")
    if echo "$output" | grep -q "declare -x VAR"; then
        print_pass "export without value works"
    else
        print_fail "export without value" "declare -x VAR" "$output"
    fi
    
    # Test 3: export with empty value
    print_test "export VAR="
    output=$(run_minishell "export VAR=\nenv")
    if echo "$output" | grep -q "VAR="; then
        print_pass "export with empty value works"
    else
        print_fail "export empty value" "VAR=" "$output"
    fi
    
    # Test 4: export invalid identifier
    print_test "export 123VAR=value"
    output=$(run_minishell "export 123VAR=value 2>&1")
    if echo "$output" | grep -q "not a valid identifier"; then
        print_pass "export rejects invalid identifier"
    else
        print_fail "export invalid" "error message" "$output"
    fi
    
    # Test 5: export with no args (list variables)
    print_test "export (no args)"
    output=$(run_minishell "export")
    if echo "$output" | grep -q "declare -x"; then
        print_pass "export lists variables"
    else
        print_fail "export list" "declare -x ..." "$output"
    fi
    
    # Test 6: export multiple variables
    print_test "export A=1 B=2 C=3"
    output=$(run_minishell "export A=1 B=2 C=3\nenv")
    if echo "$output" | grep -q "A=1" && echo "$output" | grep -q "B=2" && echo "$output" | grep -q "C=3"; then
        print_pass "export multiple variables works"
    else
        print_fail "export multiple" "A=1, B=2, C=3" "$output"
    fi
    
    echo ""
}

# ============================================================================
# UNSET TESTS
# ============================================================================

test_unset() {
    print_header "TESTING: unset Built-in"
    
    # Test 1: unset existing variable
    print_test "unset existing variable"
    output=$(run_minishell "export VAR=value\nunset VAR\nenv")
    if ! echo "$output" | grep -q "VAR="; then
        print_pass "unset removes variable"
    else
        print_fail "unset existing" "no VAR" "$output"
    fi
    
    # Test 2: unset nonexistent variable
    print_test "unset nonexistent variable"
    output=$(run_minishell "unset NONEXISTENT 2>&1")
    # Should not error
    if ! echo "$output" | grep -q "error"; then
        print_pass "unset nonexistent is silent"
    else
        print_fail "unset nonexistent" "no error" "$output"
    fi
    
    # Test 3: unset invalid identifier
    print_test "unset 123VAR"
    output=$(run_minishell "unset 123VAR 2>&1")
    if echo "$output" | grep -q "not a valid identifier"; then
        print_pass "unset rejects invalid identifier"
    else
        print_fail "unset invalid" "error message" "$output"
    fi
    
    # Test 4: unset multiple variables
    print_test "unset A B C"
    output=$(run_minishell "export A=1 B=2 C=3\nunset A B C\nenv")
    if ! echo "$output" | grep -q "A=" && ! echo "$output" | grep -q "B=" && ! echo "$output" | grep -q "C="; then
        print_pass "unset multiple variables works"
    else
        print_fail "unset multiple" "no A, B, C" "$output"
    fi
    
    echo ""
}

# ============================================================================
# EXIT TESTS
# ============================================================================

test_exit() {
    print_header "TESTING: exit Built-in"
    
    # Test 1: exit with no args
    print_test "exit (no args)"
    echo "echo test" | $MINISHELL > /dev/null 2>&1
    exit_code=$?
    # After echo, exit should use that status (0)
    if [ $exit_code -eq 0 ]; then
        print_pass "exit with no args uses last status"
    else
        print_fail "exit no args" "0" "$exit_code"
    fi
    
    # Test 2: exit with numeric arg
    print_test "exit 42"
    echo "exit 42" | $MINISHELL > /dev/null 2>&1
    exit_code=$?
    if [ $exit_code -eq 42 ]; then
        print_pass "exit with numeric arg works"
    else
        print_fail "exit 42" "42" "$exit_code"
    fi
    
    # Test 3: exit with non-numeric arg
    print_test "exit abc"
    output=$(echo "exit abc" | $MINISHELL 2>&1)
    exit_code=$?
    if [ $exit_code -eq 2 ] && echo "$output" | grep -q "numeric argument required"; then
        print_pass "exit with non-numeric gives error 2"
    else
        print_fail "exit abc" "2 and error" "$exit_code"
    fi
    
    # Test 4: exit with too many args
    print_test "exit 1 2"
    output=$(echo -e "exit 1 2\necho still here" | $MINISHELL 2>&1)
    if echo "$output" | grep -q "too many arguments" && echo "$output" | grep -q "still here"; then
        print_pass "exit with too many args doesn't exit"
    else
        print_fail "exit too many" "error and continues" "$output"
    fi
    
    # Test 5: exit with large number (modulo 256)
    print_test "exit 300"
    echo "exit 300" | $MINISHELL > /dev/null 2>&1
    exit_code=$?
    expected=$((300 % 256))
    if [ $exit_code -eq $expected ]; then
        print_pass "exit modulo 256 works"
    else
        print_fail "exit 300" "$expected" "$exit_code"
    fi
    
    echo ""
}

# ============================================================================
# MAIN TEST RUNNER
# ============================================================================

main() {
    echo ""
    print_header "MINISHELL BUILT-IN TESTS"
    echo ""
    
    # Check if minishell exists
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell binary not found at $MINISHELL${NC}"
        echo -e "${YELLOW}Please compile minishell first with 'make'${NC}"
        exit 1
    fi
    
    # Run all tests
    test_echo
    test_pwd
    test_cd
    test_env
    test_export
    test_unset
    test_exit
    
    # Print summary
    print_header "TEST SUMMARY"
    echo -e "${BLUE}Total tests: $TOTAL_TESTS${NC}"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    echo ""
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
        echo -e "${GREEN}  ✓ ALL TESTS PASSED! ${NC}"
        echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
        exit 0
    else
        echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
        echo -e "${RED}  ✗ SOME TESTS FAILED${NC}"
        echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
        exit 1
    fi
}

# Run tests
main
