#!/bin/bash
# Integration tests for REPL improvements (KAN-56, KAN-57)

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASS=0
FAIL=0

echo -e "${BLUE}=== REPL IMPROVEMENTS TEST SUITE ===${NC}"
echo ""

# Test 1: Empty line handling
echo -n "Test 1: Empty lines are skipped... "
OUTPUT=$(echo -e "\n\n  \t  \necho test\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "test"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 2: Multiple commands in sequence
echo -n "Test 2: Multiple commands execute correctly... "
OUTPUT=$(echo -e "echo one\necho two\necho three\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "one" && echo "$OUTPUT" | grep -q "two" && echo "$OUTPUT" | grep -q "three"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 3: Exit status tracking
echo -n "Test 3: Exit status updates after commands... "
OUTPUT=$(echo -e "ls\necho \$?\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "0"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 4: Pipes work correctly
echo -n "Test 4: Pipe execution works... "
OUTPUT=$(echo -e "echo hello | cat\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "hello"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 5: Builtins work correctly
echo -n "Test 5: Builtins execute properly... "
OUTPUT=$(echo -e "pwd\necho test\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "minishell" && echo "$OUTPUT" | grep -q "test"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 6: External commands work
echo -n "Test 6: External commands execute... "
OUTPUT=$(echo -e "ls | grep Makefile\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "Makefile"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 7: Error handling for invalid commands
echo -n "Test 7: Invalid command error handling... "
OUTPUT=$(echo -e "nonexistentcmd123\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "command not found"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 8: Complex pipeline
echo -n "Test 8: Complex pipeline works... "
OUTPUT=$(echo -e "cat Makefile | grep NAME | head -1\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "NAME"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 9: Builtin in standalone context (cd)
echo -n "Test 9: cd builtin (no fork)... "
OUTPUT=$(echo -e "cd /tmp\npwd\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "/tmp"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

# Test 10: Export builtin (no fork)
echo -n "Test 10: export builtin (no fork)... "
OUTPUT=$(echo -e "export TEST=value\nenv | grep TEST\nexit" | ./minishell 2>&1)
if echo "$OUTPUT" | grep -q "TEST=value"; then
    echo -e "${GREEN}✓ PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}✗ FAIL${NC}"
    ((FAIL++))
fi

echo ""
echo -e "${BLUE}===========================${NC}"
echo -e "Results: ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}"
TOTAL=$((PASS + FAIL))
echo -e "Total: $TOTAL tests"

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}✓ ALL TESTS PASSED${NC}"
    exit 0
else
    echo -e "${RED}✗ SOME TESTS FAILED${NC}"
    exit 1
fi
