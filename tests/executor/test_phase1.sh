#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_phase1.sh                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/06 00:00:00 by codespace         #+#    #+#              #
#    Updated: 2024/12/06 00:00:00 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Phase 1 Foundation Tests
# Tests: KAN-53 (fork), KAN-54 (execve), KAN-55 (wait)

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

TESTS_PASSED=0
TESTS_FAILED=0

echo -e "${YELLOW}=== PHASE 1 FOUNDATION TESTS ===${NC}\n"

# ============================================================================ #
# Test 1: Simple external command (ls)
# ============================================================================ #
echo -n "Test 1: Simple external command (ls)... "
OUTPUT=$(echo "ls" | ./minishell 2>&1)
EXPECTED="minishell"
if echo "$OUTPUT" | grep -q "$EXPECTED"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected to find: $EXPECTED"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Test 2: Absolute path command (/bin/ls)
# ============================================================================ #
echo -n "Test 2: Absolute path command (/bin/ls)... "
OUTPUT=$(echo "/bin/ls" | ./minishell 2>&1)
EXPECTED="minishell"
if echo "$OUTPUT" | grep -q "$EXPECTED"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected to find: $EXPECTED"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Test 3: Command with arguments (ls -l)
# ============================================================================ #
echo -n "Test 3: Command with arguments (ls -l)... "
OUTPUT=$(echo "ls -l" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "minishell"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected to find: minishell"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Test 4: Command not found (should return 127)
# ============================================================================ #
echo -n "Test 4: Command not found error... "
OUTPUT=$(echo "nonexistentcmd123" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "command not found"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected: command not found error"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Test 5: Environment variable in PATH (echo)
# ============================================================================ #
echo -n "Test 5: Command from PATH (echo)... "
OUTPUT=$(echo "echo hello" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "hello"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected: hello"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Test 6: Multiple commands with status tracking
# ============================================================================ #
echo -n "Test 6: Exit status tracking... "
OUTPUT=$(printf "ls\necho \$?\n" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "0"; then
    echo -e "${GREEN}PASS${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}FAIL${NC}"
    echo "Expected exit status 0"
    echo "Got: $OUTPUT"
    ((TESTS_FAILED++))
fi

# ============================================================================ #
# Summary
# ============================================================================ #
echo -e "\n${YELLOW}=== TEST SUMMARY ===${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"
TOTAL=$((TESTS_PASSED + TESTS_FAILED))
echo -e "Total:  $TOTAL\n"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All Phase 1 tests passed!${NC}\n"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}\n"
    exit 1
fi
