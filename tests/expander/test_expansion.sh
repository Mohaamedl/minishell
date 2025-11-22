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

# Setup test environment
export TEST_VAR="hello"
export EMPTY_VAR=""
export USER_NAME="test_user"

# Test function
test_expansion() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(echo "$input" | $MINISHELL 2>&1 | head -n 1)
    
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

echo "========================================"
echo "VARIABLE EXPANSION TESTS"
echo "========================================"

echo ""
echo "----------------------------------------"
echo "Basic Variable Expansion"
echo "----------------------------------------"

test_expansion "Simple variable" \
    "echo \$TEST_VAR" \
    "hello"

test_expansion "Variable with text before" \
    "echo prefix\$TEST_VAR" \
    "prefixhello"

test_expansion "Variable with text after" \
    "echo \$TEST_VAR suffix" \
    "hello suffix"

test_expansion "Variable surrounded by text" \
    "echo prefix\$TEST_VAR suffix" \
    "prefixhello suffix"

test_expansion "Multiple variables" \
    "echo \$TEST_VAR \$TEST_VAR" \
    "hello hello"

echo ""
echo "----------------------------------------"
echo "Special Variables"
echo "----------------------------------------"

# First run a successful command
echo "true" | $MINISHELL > /dev/null 2>&1

test_expansion "Exit status after success" \
    "echo \$?" \
    "0"

# Run a command that fails
echo "false" | $MINISHELL > /dev/null 2>&1

test_expansion "Exit status after failure" \
    "echo \$?" \
    "1"

echo ""
echo "----------------------------------------"
echo "Undefined Variables"
echo "----------------------------------------"

test_expansion "Undefined variable expands to empty" \
    "echo \$UNDEFINED_VAR" \
    ""

test_expansion "Undefined with text" \
    "echo prefix\$UNDEFINED_VAR suffix" \
    "prefix suffix"

echo ""
echo "----------------------------------------"
echo "Empty Variables"
echo "----------------------------------------"

test_expansion "Empty variable" \
    "echo \$EMPTY_VAR" \
    ""

test_expansion "Empty variable with text" \
    "echo prefix\$EMPTY_VAR suffix" \
    "prefix suffix"

echo ""
echo "----------------------------------------"
echo "Quote Handling"
echo "----------------------------------------"

test_expansion "Variable in double quotes" \
    'echo "$TEST_VAR"' \
    "hello"

test_expansion "Variable in single quotes (no expansion)" \
    "echo '\$TEST_VAR'" \
    "\$TEST_VAR"

test_expansion "Mixed quotes" \
    'echo "$TEST_VAR" '"'"'$TEST_VAR'"'"'' \
    "hello \$TEST_VAR"

echo ""
echo "----------------------------------------"
echo "Complex Cases"
echo "----------------------------------------"

test_expansion "Variable with underscore in name" \
    "echo \$USER_NAME" \
    "test_user"

test_expansion "Variable followed by alphanumeric (not expanded)" \
    "echo \$TEST_VAR123" \
    ""

test_expansion "Multiple expansions in one arg" \
    "echo \$TEST_VAR-\$TEST_VAR" \
    "hello-hello"

test_expansion "Dollar sign not followed by variable" \
    "echo price\$" \
    "price$"

test_expansion "Dollar sign in middle" \
    "echo hello\$world" \
    "hello"

test_expansion "Multiple dollar signs" \
    "echo \$\$" \
    ""

echo ""
echo "----------------------------------------"
echo "Edge Cases"
echo "----------------------------------------"

test_expansion "Just a dollar sign" \
    "echo $" \
    "$"

test_expansion "Dollar with space" \
    "echo \$ test" \
    "$ test"

test_expansion "Dollar with special char" \
    "echo \$@" \
    ""

test_expansion "Variable at end of string" \
    "echo prefix\$TEST_VAR" \
    "prefixhello"

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All expansion tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
