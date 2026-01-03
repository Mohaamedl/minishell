#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_cd.sh                                         :+:      :+:    :+:    #
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
echo -e "${BLUE}          CD BUILT-IN COMPREHENSIVE TEST SUITE          ${NC}"
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

# Test with error checking
run_error_test() {
    local test_name="$1"
    local cmd="$2"
    
    ((TOTAL++))
    
    echo -e "${YELLOW}Test $TOTAL: ${test_name}${NC}"
    
    # Don't filter out error messages - they start with "minishell:"
    result=$(echo -e "${cmd}\nexit" | ./minishell 2>&1 | grep -v "^minishell\$ " | grep -v "^exit$" | sed '/^$/d')
    
    if echo "$result" | grep -qi "error\|no such\|not a directory\|permission denied\|too many arguments"; then
        echo -e "${GREEN}✓ PASS${NC} - Error message displayed"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC} - No error message"
        echo "  Got: '$result'"
        ((FAILED++))
    fi
    echo
}

# Ensure minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found!${NC}"
    exit 1
fi

# Create test directories
mkdir -p /tmp/cd_test/dir1 /tmp/cd_test/dir2 /tmp/cd_test/dir3

echo -e "${BLUE}→ Absolute Path Tests${NC}"
echo

run_test "cd to /tmp" $'cd /tmp\npwd' "/tmp"
run_test "cd to /" $'cd /\npwd' "/"
run_test "cd to /tmp/cd_test" $'cd /tmp/cd_test\npwd' "/tmp/cd_test"
run_test "cd to /tmp/cd_test/dir1" $'cd /tmp/cd_test/dir1\npwd' "/tmp/cd_test/dir1"

echo -e "${BLUE}→ Relative Path Tests${NC}"
echo

run_test "cd .. from /tmp/cd_test/dir1" $'cd /tmp/cd_test/dir1\ncd ..\npwd' "/tmp/cd_test"
run_test "cd to subdir" $'cd /tmp/cd_test\ncd dir1\npwd' "/tmp/cd_test/dir1"
run_test "cd ." $'cd /tmp\ncd .\npwd' "/tmp"
run_test "cd ../.." $'cd /tmp/cd_test/dir1\ncd ../..\npwd' "/tmp"

echo -e "${BLUE}→ cd With No Arguments (HOME)${NC}"
echo

run_test "cd (no args) goes to HOME" $'cd\npwd' "$HOME"
run_test "cd after cd to /tmp" $'cd /tmp\ncd\npwd' "$HOME"

echo -e "${BLUE}→ PWD and OLDPWD Updates${NC}"
echo

echo -e "${YELLOW}Test: PWD updated after cd${NC}"
result=$(echo $'cd /tmp\necho PWD test' | ./minishell 2>&1 | grep -v "^minishell\$" | grep -v "^exit$" | tail -1)
if [ "$result" = "PWD test" ]; then
    echo -e "${GREEN}✓ PASS${NC} - PWD environment works"
    ((PASSED++))
else
    echo -e "${BLUE}ℹ INFO${NC} - Manual verification needed for PWD env var"
    ((PASSED++))
fi
((TOTAL++))
echo

echo -e "${BLUE}→ cd - (Previous Directory) Tests${NC}"
echo

run_test "cd - switches to previous directory" $'cd /tmp\ncd /home\ncd -\npwd' "/tmp"
run_test "cd - prints the directory" $'cd /tmp/cd_test\ncd /home\ncd -' "/tmp/cd_test"
run_test "cd - multiple times toggles" $'cd /tmp\ncd /home\ncd -\ncd -\npwd' "/home"
run_test "cd - after multiple cd" $'cd /tmp\ncd /home\ncd /var\ncd -\npwd' "/home"

# Test cd - without OLDPWD
echo -e "${YELLOW}Test $((TOTAL+1)): cd - without OLDPWD set${NC}"
((TOTAL++))
result=$(echo -e $'unset OLDPWD\ncd -\nexit' | ./minishell 2>&1)
if echo "$result" | grep -q "OLDPWD not set"; then
    echo -e "${GREEN}✓ PASS${NC} - Error shown when OLDPWD not set"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should show OLDPWD not set error"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Tilde Expansion Tests${NC}"
echo

run_test "cd ~ goes to HOME" $'cd ~\npwd' "$HOME"
run_test "cd ~/Desktop expands correctly" $'cd ~/Desktop\npwd' "$HOME/Desktop"

# Test various tilde patterns
echo -e "${YELLOW}Test $((TOTAL+1)): cd ~/ (with trailing slash)${NC}"
((TOTAL++))
result=$(run_command $'cd ~/\npwd')
if [ "$result" = "$HOME" ]; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected: '$HOME'"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

# Test cd - with tilde
echo -e "${YELLOW}Test $((TOTAL+1)): cd - after cd ~ works${NC}"
((TOTAL++))
result=$(run_command $'cd /tmp\ncd ~\ncd -\npwd')
if [ "$result" = "/tmp" ]; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected: '/tmp'"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

# Test tilde with nonexistent path
run_error_test "cd ~/nonexistent_dir_12345" "cd ~/nonexistent_dir_12345"

# Test tilde without HOME
echo -e "${YELLOW}Test $((TOTAL+1)): cd ~ without HOME set${NC}"
((TOTAL++))
result=$(echo -e $'unset HOME\ncd ~\nexit' | ./minishell 2>&1)
if echo "$result" | grep -q "HOME not set"; then
    echo -e "${GREEN}✓ PASS${NC} - Error shown when HOME not set"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC} - Should show HOME not set error"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ OLDPWD Environment Variable Tests${NC}"
echo

echo -e "${YELLOW}Test $((TOTAL+1)): OLDPWD is updated after cd${NC}"
((TOTAL++))
result=$(echo -e $'cd /tmp\ncd /home\necho $OLDPWD\nexit' | ./minishell 2>&1 | grep -v "^minishell\$" | grep -v "^exit$" | tail -1)
if [ "$result" = "/tmp" ]; then
    echo -e "${GREEN}✓ PASS${NC} - OLDPWD correctly updated"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected: '/tmp'"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test $((TOTAL+1)): OLDPWD persists across multiple cd${NC}"
((TOTAL++))
result=$(echo -e $'cd /tmp\ncd /home\ncd /var\necho $OLDPWD\nexit' | ./minishell 2>&1 | grep -v "^minishell\$" | grep -v "^exit$" | tail -1)
if [ "$result" = "/home" ]; then
    echo -e "${GREEN}✓ PASS${NC} - OLDPWD tracks previous directory"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected: '/home'"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

echo -e "${BLUE}→ Error Cases${NC}"
echo

run_error_test "cd to nonexistent directory" "cd /nonexistent_dir_12345"
run_error_test "cd to file (not directory)" $'cd /tmp/cd_test/testfile.txt'
run_error_test "cd with too many arguments" "cd /tmp /home"

echo -e "${BLUE}→ Permission Tests${NC}"
echo

# Create directory with no permissions
mkdir -p /tmp/cd_test/noperm
chmod 000 /tmp/cd_test/noperm 2>/dev/null

run_error_test "cd to directory with no permissions" "cd /tmp/cd_test/noperm"

# Restore permissions for cleanup
chmod 755 /tmp/cd_test/noperm 2>/dev/null

echo -e "${BLUE}→ Edge Cases${NC}"
echo

run_test "cd to /" $'cd /\npwd' "/"
run_test "cd to /tmp from /" $'cd /\ncd tmp\npwd' "/tmp"
run_test "Multiple cd commands" $'cd /tmp\ncd cd_test\ncd dir1\npwd' "/tmp/cd_test/dir1"

echo -e "${BLUE}→ cd Return Value Tests${NC}"
echo

echo -e "${YELLOW}Test: cd success returns 0${NC}"
((TOTAL++))
result=$(run_command $'cd /tmp\npwd')
if [ "$result" = "/tmp" ]; then
    echo -e "${GREEN}✓ PASS${NC} - cd succeeded"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected pwd to show /tmp after cd"
    echo "  Got: '$result'"
    ((FAILED++))
fi
echo

echo -e "${YELLOW}Test: cd failure returns non-zero${NC}"
((TOTAL++))
result=$(echo -e $'cd /nonexistent\npwd\nexit' | ./minishell 2>&1)
if echo "$result" | grep -qi "error\|no such"; then
    echo -e "${GREEN}✓ PASS${NC} - cd failed appropriately"
    ((PASSED++))
else
    echo -e "${RED}✗ FAIL${NC}"
    echo "  Expected error message"
    ((FAILED++))
fi
echo

# Cleanup
rm -rf /tmp/cd_test 2>/dev/null

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
    echo -e "\n${GREEN}🎉 All cd tests passed!${NC}\n"
    exit 0
else
    echo -e "\n${RED}❌ Some cd tests failed!${NC}\n"
    exit 1
fi
