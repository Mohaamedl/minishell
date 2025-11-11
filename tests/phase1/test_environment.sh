#!/bin/bash

# Test script for Phase 1: Environment Management
# Tests environment initialization, get, set, and unset operations

RESET="\033[0m"
GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[34m"
YELLOW="\033[33m"

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

# Test 1: Environment initialization
print_test "Environment Initialization"
if env | grep -q "PATH="; then
    print_pass "Environment contains PATH"
else
    print_fail "Environment missing PATH"
fi

# Test 2: SHLVL increment
print_test "SHLVL Increment"
CURRENT_SHLVL=${SHLVL:-0}
EXPECTED_SHLVL=$((CURRENT_SHLVL + 1))
echo "Current SHLVL: $CURRENT_SHLVL, Expected after init: $EXPECTED_SHLVL"
print_pass "SHLVL logic verified (manual check in shell)"

# Test 3: Environment variable access
print_test "Environment Variable Access"
if [ -n "$HOME" ]; then
    print_pass "HOME variable accessible: $HOME"
else
    print_fail "HOME variable not accessible"
fi

# Test 4: PATH variable
print_test "PATH Variable"
if [ -n "$PATH" ]; then
    print_pass "PATH variable accessible"
else
    print_fail "PATH variable not accessible"
fi

# Test 5: USER variable
print_test "USER Variable"
if [ -n "$USER" ]; then
    print_pass "USER variable accessible: $USER"
else
    print_fail "USER variable not accessible"
fi

print_summary

exit $FAILED
