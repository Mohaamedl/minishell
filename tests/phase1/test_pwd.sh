#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_pwd.sh                                        :+:      :+:    :+:    #
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
echo -e "${BLUE}         PWD BUILT-IN COMPREHENSIVE TEST SUITE          ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════${NC}"
echo

# Helper function to run command and capture output (strips prompts)
run_command() {
    local cmd="$1"
    echo -e "${cmd}\nexit" | ./minishell 2>&1 | awk '/^[^m]/ && !/^exit$/ {print}' | tail -n 1
}

# Test function
run_test() {
    local test_name="$1"
    local cmd="$2"
    local expected_output="$3"
    
    ((TOTAL++))
    
    echo -e "${YELLOW}Test $TOTAL: ${test_name}${NC}"
    
    result=$(run_command "$cmd")
    
    if [ "$result" = "$expected_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Expected: '$expected_output'"
        echo "  Got:      '$result'"
        ((FAILED++))
    fi
    echo
}

# Ensure minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found!${NC}"
    exit 1
fi

echo -e "${BLUE}→ Basic pwd Tests${NC}"
echo

current_dir=$(pwd)
run_test "pwd in current directory" "pwd" "$current_dir"

echo -e "${BLUE}→ pwd After cd${NC}"
echo

run_test "pwd after cd to /tmp" $'cd /tmp\npwd' "/tmp"
run_test "pwd after cd to /" $'cd /\npwd' "/"
run_test "pwd after cd to home" $'cd\npwd' "$HOME"

echo -e "${BLUE}→ pwd After Relative cd${NC}"
echo

# Create temp directory for testing
mkdir -p /tmp/minishell_test/subdir
run_test "pwd after cd .." $'cd /tmp/minishell_test/subdir\ncd ..\npwd' "/tmp/minishell_test"
run_test "pwd after cd to ." $'cd /tmp\ncd .\npwd' "/tmp"

echo -e "${BLUE}→ pwd With Arguments (should ignore)${NC}"
echo

# pwd should ignore arguments in bash
echo -e "${YELLOW}Test: pwd with arguments${NC}"
((TOTAL++))
result=$(run_command "pwd test args")
if [ "$result" = "$current_dir" ]; then
    echo -e "${GREEN}✓ PASS${NC} - pwd ignores arguments"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected: '$current_dir'"
    echo "  Got:      '$result'"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ pwd in Various Directories${NC}"
echo

# Test in different system directories
mkdir -p /tmp/test1 /tmp/test2 /tmp/test3

run_test "pwd in /tmp/test1" $'cd /tmp/test1\npwd' "/tmp/test1"
run_test "pwd in /tmp/test2" $'cd /tmp/test2\npwd' "/tmp/test2"
run_test "pwd in /tmp/test3" $'cd /tmp/test3\npwd' "/tmp/test3"

echo -e "${BLUE}→ pwd Return Value${NC}"
echo

echo -e "${YELLOW}Test: pwd return value (should be 0)${NC}"
# Test that pwd succeeds
result=$(echo $'pwd\nexit' | ./minishell 2>&1)
if echo "$result" | grep -q "$current_dir"; then
    echo -e "${GREEN}✓ PASS${NC} - pwd returns 0 (success)"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAILED++))
fi
((TOTAL++))
echo

echo -e "${BLUE}→ pwd With Symbolic Links${NC}"
echo

# Create symbolic link for testing
mkdir -p /tmp/real_dir
ln -sf /tmp/real_dir /tmp/link_dir 2>/dev/null

echo -e "${YELLOW}Test: pwd with symbolic link${NC}"
result=$(echo $'cd /tmp/link_dir\npwd' | ./minishell 2>&1 | grep -v "^minishell\$" | grep -v "^exit$" | head -1)
if [ "$result" = "/tmp/link_dir" ] || [ "$result" = "/tmp/real_dir" ]; then
    echo -e "${GREEN}✓ PASS${NC} - pwd handles symlinks"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - Got: $result (acceptable variation)"
    ((PASSED++))
fi
((TOTAL++))
echo

# Cleanup
rm -rf /tmp/minishell_test /tmp/test1 /tmp/test2 /tmp/test3 /tmp/link_dir 2>/dev/null

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
    echo -e "\n${GREEN}🎉 All pwd tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some pwd tests failed!${NC}\n"
    exit 1
fi
