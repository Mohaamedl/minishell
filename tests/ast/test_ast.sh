#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TOTAL=0
PASSED=0
FAILED=0

# Get the minishell binary path
MINISHELL="./minishell"

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell binary not found${NC}"
    exit 1
fi

# Test function
test_ast() {
    local test_name="$1"
    local input="$2"
    local expected_output="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(echo "$input" | $MINISHELL 2>&1)
    
    # Compare output
    if [ "$output" == "$expected_output" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected: $expected_output"
        echo "  Got: $output"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "AST EXECUTION TESTS"
echo "========================================"

echo ""
echo "----------------------------------------"
echo "Simple Commands"
echo "----------------------------------------"

test_ast "Echo command" "echo hello" "hello"
test_ast "Echo multiple args" "echo hello world" "hello world"
test_ast "PWD command" "pwd" "$(pwd)"

echo ""
echo "----------------------------------------"
echo "Command Execution Order"
echo "----------------------------------------"

# Test that AST preserves correct execution order
test_ast "Sequential echo" "echo first" "first"

echo ""
echo "----------------------------------------"
echo "Variable Expansion in AST"
echo "----------------------------------------"

export TEST_VAR="test_value"
test_ast "Variable expansion" "echo \$TEST_VAR" "test_value"
test_ast "Exit status" "echo \$?" "0"

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All AST tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
