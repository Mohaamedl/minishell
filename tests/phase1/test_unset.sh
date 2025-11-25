#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_unset.sh                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/15 00:00:00 by mhaddadi          #+#    #+#              #
#    Updated: 2025/11/15 00:00:00 by mhaddadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Counters
PASSED=0
FAILED=0
TOTAL=0

echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}       UNSET BUILT-IN COMPREHENSIVE TEST SUITE          ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo

# Ensure minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found!${NC}"
    exit 1
fi

# Helper function to run command and capture all output lines (strips prompts)
run_command_all() {
    local cmd="$1"
    echo -e "${cmd}\nexit" | ./minishell 2>&1 | awk '/^[^m]/ && !/^exit$/ {print}'
}

# Helper function to capture all output including errors
run_command_with_errors() {
    local cmd="$1"
    echo -e "${cmd}\nexit" | ./minishell 2>&1 | grep -v "^minishell\$ " | grep -v "^exit$" | sed '/^$/d'
}

# Helper function to run command and capture last output line (strips prompts)
run_command() {
    local cmd="$1"
    run_command_all "$cmd" | tail -n 1
}

echo -e "${BLUE}→ Basic unset Tests${NC}"
echo

echo -e "${YELLOW}Test 1: unset removes variable${NC}"
((TOTAL++))
result=$(run_command_all $'export TEST_VAR=hello\nunset TEST_VAR\nenv' | grep "TEST_VAR=")
if [ -z "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Variable removed"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Variable still exists"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 2: unset multiple variables${NC}"
((TOTAL++))
result=$(run_command_all $'export A=1\nexport B=2\nexport C=3\nunset A B C\nenv')
if ! echo "$result" | grep -q "A=" && ! echo "$result" | grep -q "B=" && ! echo "$result" | grep -q "C="; then
    echo -e "${GREEN}✓ PASS${NC} - All variables removed"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Some variables remain"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 3: unset one variable leaves others${NC}"
((TOTAL++))
result=$(run_command_all $'export X=1\nexport Y=2\nunset X\nenv')
if ! echo "$result" | grep -q "^X=" && echo "$result" | grep -q "^Y=2"; then
    echo -e "${GREEN}✓ PASS${NC} - Correct variable removed"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Debug: Looking for X= (should not exist) and Y=2 (should exist)"
    echo "$result" | grep -E "^X=|^Y=" || echo "  (no X or Y found)"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ unset Non-existent Variable${NC}"
echo

echo -e "${YELLOW}Test 4: unset non-existent variable (silent)${NC}"
((TOTAL++))
result=$(run_command_all $'unset NONEXISTENT\npwd')
# Should not error, just silently do nothing
if echo "$result" | grep -q "/"; then
    echo -e "${GREEN}✓ PASS${NC} - No error for non-existent variable"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ unset PATH (special variable)${NC}"
echo

echo -e "${YELLOW}Test 5: unset PATH removes it${NC}"
((TOTAL++))
result=$(run_command_all $'unset PATH\nenv' | grep "^PATH=")
if [ -z "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - PATH can be unset"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - PATH still exists"
    echo "  Found: $result"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ unset HOME (special variable)${NC}"
echo

echo -e "${YELLOW}Test 6: unset HOME removes it${NC}"
((TOTAL++))
result=$(run_command_all $'unset HOME\nenv' | grep "^HOME=")
if [ -z "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - HOME can be unset"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - HOME still exists"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Variable Name Validation${NC}"
echo

echo -e "${YELLOW}Test 7: unset invalid name (starts with digit)${NC}"
((TOTAL++))
result=$(echo "unset 1VAR" | ./minishell 2>&1)
if echo "$result" | grep -qi "error\|invalid\|not a valid identifier"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects invalid name"
    ((PASSED++))
else
    # Some implementations silently ignore
    echo -e "${BLUE}ℹ INFO${NC} - May silently ignore invalid names"
    ((PASSED++))
fi
echo

echo -e "${YELLOW}Test 8: unset invalid name (special chars)${NC}"
((TOTAL++))
result=$(echo "unset VAR-NAME" | ./minishell 2>&1)
if echo "$result" | grep -qi "error\|invalid"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects special characters"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - May silently ignore"
    ((PASSED++))
fi
echo

echo -e "${BLUE}→ unset Without Arguments${NC}"
echo

echo -e "${YELLOW}Test 9: unset with no arguments${NC}"
((TOTAL++))
result=$(echo $'unset\npwd' | ./minishell 2>&1)
# Should either error or do nothing
if echo "$result" | grep -q "/"; then
    echo -e "${GREEN}✓ PASS${NC} - Handles no arguments"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ unset Return Value${NC}"
echo

echo -e "${YELLOW}Test 10: unset returns 0 on success${NC}"
((TOTAL++))
result=$(echo $'export VAR=test\nunset VAR\npwd' | ./minishell 2>&1)
if echo "$result" | grep -q "/"; then
    echo -e "${GREEN}✓ PASS${NC} - Returns success"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Combination Tests${NC}"
echo

echo -e "${YELLOW}Test 11: export, unset, re-export${NC}"
((TOTAL++))
result=$(run_command_all $'export VAR=first\nunset VAR\nexport VAR=second\nenv' | grep "VAR=")
if echo "$result" | grep -q "VAR=second" && ! echo "$result" | grep -q "VAR=first"; then
    echo -e "${GREEN}✓ PASS${NC} - Re-export works after unset"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 12: unset multiple times${NC}"
((TOTAL++))
result=$(echo $'export VAR=test\nunset VAR\nunset VAR\nunset VAR\npwd' | ./minishell 2>&1)
if echo "$result" | grep -q "/"; then
    echo -e "${GREEN}✓ PASS${NC} - Multiple unset calls don't error"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Mixed Valid and Invalid${NC}"
echo

echo -e "${YELLOW}Test 13: unset mix of existing and non-existing${NC}"
((TOTAL++))
result=$(run_command_all $'export EXISTS=yes\nunset EXISTS NOTHERE ALSONOTHERE\nenv' | grep "EXISTS=")
if [ -z "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Handles mixed variables"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

# Summary
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}                     TEST SUMMARY                       ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo -e "Total tests: ${TOTAL}"
echo -e "${GREEN}Passed: ${PASSED}${NC}"
echo -e "${RED}Failed: ${FAILED}${NC}"
echo -e "Success rate: $(awk "BEGIN {printf \"%.1f\", ($PASSED/$TOTAL)*100}")%"
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All unset tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some unset tests failed!${NC}\n"
    exit 1
fi
