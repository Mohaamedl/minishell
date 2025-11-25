#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_exit.sh                                       :+:      :+:    :+:    #
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
echo -e "${BLUE}        EXIT BUILT-IN COMPREHENSIVE TEST SUITE          ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo

# Ensure minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found!${NC}"
    exit 1
fi

echo -e "${BLUE}→ Basic exit Tests${NC}"
echo

echo -e "${YELLOW}Test 1: exit with no arguments (status 0)${NC}"
((TOTAL++))
echo "exit" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 0"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 0, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 2: exit 0${NC}"
((TOTAL++))
echo "exit 0" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 0"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 0, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 3: exit 1${NC}"
((TOTAL++))
echo "exit 1" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 1 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 1"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 1, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 4: exit 42${NC}"
((TOTAL++))
echo "exit 42" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 42 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 42"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 42, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ exit With Large Numbers${NC}"
echo

echo -e "${YELLOW}Test 5: exit 255 (max valid)${NC}"
((TOTAL++))
echo "exit 255" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 255 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 255"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 255, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 6: exit 256 (should wrap to 0)${NC}"
((TOTAL++))
echo "exit 256" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code wraps to 0 (256 % 256)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 0, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 7: exit 257 (should wrap to 1)${NC}"
((TOTAL++))
echo "exit 257" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 1 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code wraps to 1 (257 % 256)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 1, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 8: exit 1000 (large number)${NC}"
((TOTAL++))
echo "exit 1000" | ./minishell >/dev/null 2>&1
exit_code=$?
expected=$((1000 % 256))
if [ $exit_code -eq $expected ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is $expected (1000 % 256)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected $expected, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ exit With Negative Numbers${NC}"
echo

echo -e "${YELLOW}Test 9: exit -1${NC}"
((TOTAL++))
echo "exit -1" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 255 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is 255 (bash behavior)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 255, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 10: exit -42${NC}"
((TOTAL++))
echo "exit -42" | ./minishell >/dev/null 2>&1
exit_code=$?
expected=214  # (256 - 42)
if [ $exit_code -eq $expected ]; then
    echo -e "${GREEN}✓ PASS${NC} - Exit code is $expected"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected $expected, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ exit With Invalid Arguments${NC}"
echo

echo -e "${YELLOW}Test 11: exit with non-numeric argument${NC}"
((TOTAL++))
result=$(echo "exit abc" | ./minishell 2>&1)
if echo "$result" | grep -qi "numeric\|error\|invalid"; then
    echo -e "${GREEN}✓ PASS${NC} - Error message for non-numeric"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should show error"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 12: exit with too many arguments${NC}"
((TOTAL++))
result=$(echo "exit 1 2 3" | ./minishell 2>&1)
if echo "$result" | grep -qi "too many arguments\|error"; then
    echo -e "${GREEN}✓ PASS${NC} - Error for too many arguments"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should show error"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 13: exit with mixed valid/invalid${NC}"
((TOTAL++))
result=$(echo "exit 42abc" | ./minishell 2>&1)
if echo "$result" | grep -qi "numeric\|error\|invalid"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects mixed input"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ exit Edge Cases${NC}"
echo

echo -e "${YELLOW}Test 14: exit with leading zeros${NC}"
((TOTAL++))
echo "exit 007" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 7 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Handles leading zeros (exit code 7)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 7, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 15: exit with plus sign${NC}"
((TOTAL++))
echo "exit +42" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 42 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Handles + sign (exit code 42)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 42, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 16: exit with spaces${NC}"
((TOTAL++))
echo "exit    42" | ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 42 ]; then
    echo -e "${GREEN}✓ PASS${NC} - Handles spaces (exit code 42)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected 42, got $exit_code"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Shell Continues After Other Commands${NC}"
echo

echo -e "${YELLOW}Test 17: Commands before exit execute${NC}"
((TOTAL++))
result=$(echo $'echo test\nexit' | ./minishell 2>&1 | grep "test")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Commands before exit run"
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
    echo -e "\n${GREEN}🎉 All exit tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some exit tests failed!${NC}\n"
    exit 1
fi
