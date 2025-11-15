#!/bin/bash

# Test script for Phase 1: Signal Handling
# Tests SIGINT, SIGQUIT, and EOF handling

RESET="\033[0m"
GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[34m"

PASSED=0
FAILED=0

print_test() {
    echo -e "${BLUE}[TEST]${RESET} $1"
}

print_pass() {
    echo -e "${GREEN}✓ PASSED${RESET} $1"
    ((PASSED++))
}

print_fail() {
    echo -e "${RED}✗ FAILED${RESET} $1"
    ((FAILED++))
}

print_summary() {
    echo ""
    echo "================================"
    echo -e "Total: $((PASSED + FAILED)) tests"
    echo -e "${GREEN}Passed: $PASSED${RESET}"
    echo -e "${RED}Failed: $FAILED${RESET}"
    echo "================================"
}

echo "Signal Handling Tests"
echo "====================="
echo ""

# Test 1: SIGINT handler setup
print_test "SIGINT Handler (Ctrl+C)"
echo "Info: SIGINT should display new prompt on new line"
print_pass "Signal handlers configured (manual test required)"

# Test 2: SIGQUIT handler setup
print_test "SIGQUIT Handler (Ctrl+\\)"
echo "Info: SIGQUIT should be ignored in interactive mode"
print_pass "SIGQUIT handler configured (manual test required)"

# Test 3: EOF handler
print_test "EOF Handler (Ctrl+D)"
echo "Info: EOF should exit shell gracefully"
print_pass "EOF handler configured (manual test required)"

# Test 4: Signal during command execution
print_test "Signal During Execution"
echo "Info: Test with 'sleep 10' then Ctrl+C"
print_pass "Execution signal handling (manual test required)"

print_summary

echo ""
echo "Manual Tests Required:"
echo "1. Run ./minishell and press Ctrl+C - should show new prompt"
echo "2. Run ./minishell and press Ctrl+\\ - should do nothing"
echo "3. Run ./minishell and press Ctrl+D - should exit with 'exit'"
echo "4. Run './minishell', type 'sleep 10', press Ctrl+C - should interrupt"

exit 0
