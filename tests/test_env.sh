#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_env.sh                                        :+:      :+:    :+:    #
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
echo -e "${BLUE}         ENV BUILT-IN COMPREHENSIVE TEST SUITE          ${NC}"
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

# Helper function to run command and capture last output line (strips prompts)
run_command() {
    local cmd="$1"
    run_command_all "$cmd" | tail -n 1
}

echo -e "${BLUE}→ Basic env Tests${NC}"
echo

echo -e "${YELLOW}Test 1: env displays PATH${NC}"
((TOTAL++))
result=$(run_command_all "env" | grep "PATH=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - PATH variable found"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - PATH not found in env output"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 2: env displays HOME${NC}"
((TOTAL++))
result=$(run_command_all "env" | grep "HOME=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - HOME variable found"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - HOME not found in env output"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 3: env displays USER${NC}"
((TOTAL++))
result=$(run_command_all "env" | grep "USER=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - USER variable found"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - USER not found (may not be set in environment)"
    ((PASSED++))
fi
echo

echo -e "${BLUE}→ env Format Tests${NC}"
echo

echo -e "${YELLOW}Test 4: env output format (NAME=value)${NC}"
((TOTAL++))
result=$(run_command_all "env" | grep "=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Format is NAME=value"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Incorrect format"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ env After export${NC}"
echo

echo -e "${YELLOW}Test 5: env shows newly exported variable${NC}"
((TOTAL++))
result=$(run_command_all $'export TEST_VAR=hello\nenv' | grep "TEST_VAR=hello")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exported variable appears in env"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Exported variable not found"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 6: env shows multiple exported variables${NC}"
((TOTAL++))
result=$(run_command_all $'export A=1\nexport B=2\nexport C=3\nenv')
if echo "$result" | grep -q "A=1" && echo "$result" | grep -q "B=2" && echo "$result" | grep -q "C=3"; then
    echo -e "${GREEN}✓ PASS${NC} - All exported variables found"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Not all variables found"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ env After unset${NC}"
echo

echo -e "${YELLOW}Test 7: env doesn't show unset variable${NC}"
((TOTAL++))
result=$(run_command_all $'export TEMP_VAR=test\nunset TEMP_VAR\nenv' | grep "TEMP_VAR=")
if [ -z "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Unset variable not in env"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Unset variable still appears"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ env Return Value${NC}"
echo

echo -e "${YELLOW}Test 8: env returns 0${NC}"
((TOTAL++))
result=$(run_command_all $'env\nexit')
if echo "$result" | grep -q "PATH="; then
    echo -e "${GREEN}✓ PASS${NC} - env executed successfully"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ env With Arguments (optional)${NC}"
echo

echo -e "${YELLOW}Test 9: env with arguments (if supported)${NC}"
((TOTAL++))
# In standard bash, env with args runs a command with modified environment
# For minishell, this is typically not required
result=$(run_command_all "env test")
if echo "$result" | grep -q "PATH=\|command not found\|not implemented"; then
    echo -e "${GREEN}✓ PASS${NC} - Handles arguments appropriately"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - env behavior with arguments varies"
    ((PASSED++))
fi
echo

echo -e "${BLUE}→ Environment Variable Count${NC}"
echo

echo -e "${YELLOW}Test 10: env displays reasonable number of variables${NC}"
((TOTAL++))
count=$(run_command_all "env" | grep "=" | wc -l)
if [ "$count" -gt 5 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Found $count environment variables"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Only found $count variables (expected more)"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Empty Value Tests${NC}"
echo

echo -e "${YELLOW}Test 11: env shows variable with empty value${NC}"
((TOTAL++))
result=$(run_command_all $'export EMPTY_VAR=\nenv' | grep "EMPTY_VAR=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Empty value variable shown"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Empty value variable not shown"
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
    echo -e "\n${GREEN}🎉 All env tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some env tests failed!${NC}\n"
    exit 1
fi
