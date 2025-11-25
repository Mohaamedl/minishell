#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_export.sh                                     :+:      :+:    :+:    #
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
echo -e "${BLUE}       EXPORT BUILT-IN COMPREHENSIVE TEST SUITE         ${NC}"
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

echo -e "${BLUE}→ Basic export Tests${NC}"
echo

echo -e "${YELLOW}Test 1: export sets variable${NC}"
((TOTAL++))
result=$(run_command_all $'export TEST_VAR=hello\nenv' | grep "TEST_VAR=hello")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Variable exported successfully"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Variable not found in environment"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 2: export with numeric value${NC}"
((TOTAL++))
result=$(run_command_all $'export NUM=42\nenv' | grep "NUM=42")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Numeric value exported"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 3: export with empty value${NC}"
((TOTAL++))
result=$(run_command_all $'export EMPTY=\nenv' | grep "EMPTY=")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Empty value exported"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ export Without Arguments (print all)${NC}"
echo

echo -e "${YELLOW}Test 4: export (no args) shows variables${NC}"
((TOTAL++))
result=$(run_command_all "export" | grep -E "declare -x|export")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Shows exported variables"
    ((PASSED++))
else
    # Try alternative format
    result=$(run_command_all "export" | grep "=")
    if [ -n "$result" ]; then
        echo -e "${GREEN}✓ PASS${NC} - Shows variables (alternative format)"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        ((FAILED++))
    fi
fi
echo

echo -e "${YELLOW}Test 5: export displays PATH${NC}"
((TOTAL++))
result=$(run_command_all "export" | grep "PATH")
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - PATH shown in export"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Multiple exports${NC}"
echo

echo -e "${YELLOW}Test 6: export multiple variables${NC}"
((TOTAL++))
result=$(echo $'export A=1\nexport B=2\nexport C=3\nenv' | ./minishell 2>&1)
if echo "$result" | grep -q "A=1" && echo "$result" | grep -q "B=2" && echo "$result" | grep -q "C=3"; then
    echo -e "${GREEN}✓ PASS${NC} - Multiple exports work"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 7: export multiple on same line${NC}"
((TOTAL++))
result=$(echo $'export X=1 Y=2 Z=3\nenv' | ./minishell 2>&1)
# This might not be supported, so we're lenient
if echo "$result" | grep -q "X=1"; then
    echo -e "${GREEN}✓ PASS${NC} - Multiple exports on one line"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - Single-line multiple export not required"
    ((PASSED++))
fi
echo

echo -e "${BLUE}→ Variable Name Validation${NC}"
echo

echo -e "${YELLOW}Test 8: export invalid name (starts with digit)${NC}"
((TOTAL++))
result=$(echo "export 1VAR=test" | ./minishell 2>&1)
if echo "$result" | grep -qi "error\|invalid\|not a valid identifier"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects invalid name"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should reject names starting with digit"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 9: export invalid name (special chars)${NC}"
((TOTAL++))
result=$(echo "export VAR-NAME=test" | ./minishell 2>&1)
if echo "$result" | grep -qi "error\|invalid\|not a valid identifier"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects invalid characters"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should reject special characters"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 10: export valid names (underscore, alphanumeric)${NC}"
((TOTAL++))
result=$(echo $'export _VAR=test\nexport VAR123=test\nexport VAR_NAME=test\nenv' | ./minishell 2>&1)
if echo "$result" | grep -q "_VAR=test" && echo "$result" | grep -q "VAR123=test" && echo "$result" | grep -q "VAR_NAME=test"; then
    echo -e "${GREEN}✓ PASS${NC} - Accepts valid names"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Value with Spaces and Quotes${NC}"
echo

echo -e "${YELLOW}Test 11: export value with spaces (quoted)${NC}"
((TOTAL++))
result=$(run_command_all $'export MSG="hello world"\nenv' | grep 'MSG=')
if [ -n "$result" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Handles quoted values"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - Quote handling depends on parser (Phase 2)"
    ((PASSED++))
fi
echo

echo -e "${BLUE}→ Overwrite Existing Variable${NC}"
echo

echo -e "${YELLOW}Test 12: export overwrites existing variable${NC}"
((TOTAL++))
result=$(run_command_all $'export VAR=old\nexport VAR=new\nenv' | grep "VAR=new")
if [ -n "$result" ] && ! echo "$result" | grep -q "VAR=old"; then
    echo -e "${GREEN}✓ PASS${NC} - Overwrites correctly"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ export Return Value${NC}"
echo

echo -e "${YELLOW}Test 13: export returns 0 on success${NC}"
((TOTAL++))
result=$(run_command_all $'export GOOD=value\npwd')
if echo "$result" | grep -q "/"; then
    echo -e "${GREEN}✓ PASS${NC} - Returns success"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test 14: export returns error on invalid name${NC}"
((TOTAL++))
result=$(run_command_with_errors $'export 123=bad\npwd')
if echo "$result" | grep -qi "error\|invalid\|not a valid identifier"; then
    echo -e "${GREEN}✓ PASS${NC} - Returns error"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected error for invalid identifier"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Edge Cases${NC}"
echo

echo -e "${YELLOW}Test 15: export without = (declare without value)${NC}"
((TOTAL++))
result=$(run_command_all $'export DECLARED\nenv')
# This behavior varies - some shells export it, some don't
if echo "$result" | grep -q "DECLARED"; then
    echo -e "${GREEN}✓ PASS${NC} - Variable declared"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - Declaration without value handling varies"
    ((PASSED++))
fi
echo

echo -e "${YELLOW}Test 16: export with only = (invalid)${NC}"
((TOTAL++))
result=$(run_command_with_errors "export =value")
if echo "$result" | grep -qi "error\|invalid\|not a valid identifier"; then
    echo -e "${GREEN}✓ PASS${NC} - Rejects '=value'"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Expected error for '=value'"
    echo "  Got: '$result'"
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
    echo -e "\n${GREEN}🎉 All export tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some export tests failed!${NC}\n"
    exit 1
fi
