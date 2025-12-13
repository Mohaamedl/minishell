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
test_logical() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output (filter out error messages for cleaner comparison)
    output=$(echo "$input" | $MINISHELL 2>/dev/null | tr '\n' ' ' | sed 's/ $//')
    
    # Compare output
    if [ "$output" == "$expected" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected: '$expected'"
        echo "  Got: '$output'"
        FAILED=$((FAILED + 1))
    fi
}

# Test function for exit status
test_exit_status() {
    local test_name="$1"
    local input="$2"
    local expected_status="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and get the output of echo $?
    output=$(echo -e "${input}\necho \$?" | $MINISHELL 2>/dev/null | tail -n 1)
    
    if [ "$output" == "$expected_status" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected exit status: '$expected_status'"
        echo "  Got: '$output'"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "LOGICAL OPERATORS TESTS (BONUS)"
echo "========================================"

echo ""
echo "----------------------------------------"
echo "AND Operator (&&) - Basic"
echo "----------------------------------------"

test_logical "true && echo success" \
    "echo start && echo success" \
    "start success"

test_logical "false && echo never (should not print never)" \
    "nonexistent_cmd && echo never" \
    ""

test_logical "Multiple && all succeed" \
    "echo a && echo b && echo c" \
    "a b c"

test_logical "Multiple && first fails" \
    "nonexistent_cmd && echo b && echo c" \
    ""

test_logical "Multiple && middle fails" \
    "echo a && nonexistent_cmd && echo c" \
    "a"

echo ""
echo "----------------------------------------"
echo "OR Operator (||) - Basic"
echo "----------------------------------------"

test_logical "true || echo never (should not print never)" \
    "echo success || echo never" \
    "success"

test_logical "false || echo fallback" \
    "nonexistent_cmd || echo fallback" \
    "fallback"

test_logical "Multiple || first succeeds" \
    "echo first || echo second || echo third" \
    "first"

test_logical "Multiple || first fails, second succeeds" \
    "nonexistent_cmd || echo second || echo third" \
    "second"

test_logical "Multiple || all fail except last" \
    "nonexistent1 || nonexistent2 || echo last" \
    "last"

echo ""
echo "----------------------------------------"
echo "Mixed Operators"
echo "----------------------------------------"

test_logical "cmd1 && cmd2 || cmd3 (both succeed)" \
    "echo a && echo b || echo c" \
    "a b"

test_logical "cmd1 && cmd2 || cmd3 (first fails)" \
    "nonexistent && echo b || echo fallback" \
    "fallback"

test_logical "cmd1 && cmd2 || cmd3 (second fails)" \
    "echo a && nonexistent || echo recovered" \
    "a recovered"

test_logical "cmd1 || cmd2 && cmd3 (first succeeds)" \
    "echo first || echo second && echo third" \
    "first third"

test_logical "cmd1 || cmd2 && cmd3 (first fails)" \
    "nonexistent || echo second && echo third" \
    "second third"

echo ""
echo "----------------------------------------"
echo "With Parentheses"
echo "----------------------------------------"

test_logical "(cmd1 && cmd2) success" \
    "(echo a && echo b)" \
    "a b"

test_logical "(cmd1 && cmd2) || cmd3 (group succeeds)" \
    "(echo a && echo b) || echo c" \
    "a b"

test_logical "(cmd1 && cmd2) || cmd3 (group fails)" \
    "(nonexistent && echo b) || echo fallback" \
    "fallback"

test_logical "(cmd1 || cmd2) && cmd3" \
    "(echo a || echo b) && echo c" \
    "a c"

# Note: Nested parentheses (((...))) may have issues - testing single level only

echo ""
echo "----------------------------------------"
echo "With Pipes"
echo "----------------------------------------"

test_logical "pipe && echo" \
    "echo test | cat && echo done" \
    "test done"

test_logical "pipe || echo (pipe succeeds)" \
    "echo test | cat || echo never" \
    "test"

test_logical "echo && pipe" \
    "echo start && echo piped | cat" \
    "start piped"

echo ""
echo "----------------------------------------"
echo "Exit Status Tests"
echo "----------------------------------------"

test_exit_status "true && true = 0" \
    "echo a && echo b" \
    "0"

test_exit_status "true && false = non-zero" \
    "echo a && nonexistent_cmd" \
    "127"

test_exit_status "false && true = non-zero (first cmd status)" \
    "nonexistent_cmd && echo b" \
    "127"

test_exit_status "true || true = 0" \
    "echo a || echo b" \
    "0"

test_exit_status "false || true = 0" \
    "nonexistent_cmd || echo b" \
    "0"

test_exit_status "false || false = non-zero" \
    "nonexistent1 || nonexistent2" \
    "127"

echo ""
echo "----------------------------------------"
echo "Edge Cases"
echo "----------------------------------------"

test_logical "Empty output with && failure" \
    "nonexistent && echo test" \
    ""

test_logical "Builtin echo with && success" \
    "echo start && echo done" \
    "start done"

test_logical "Builtin cd && echo" \
    "cd /tmp && pwd" \
    "/tmp"

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All logical operator tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
