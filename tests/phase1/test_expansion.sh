#!/bin/bash

# ============================================================================
# VARIABLE EXPANSION TESTS
# ============================================================================
# Tests for environment variable expansion ($VAR) and exit status ($?)
# ============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Test function
test_expansion() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command
    result=$(echo "$input" | ./minishell 2>&1 | tail -n 1)
    
    # Compare with expected
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo -e "  Input:    '$input'"
        echo -e "  Expected: '$expected'"
        echo -e "  Got:      '$result'"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "VARIABLE EXPANSION TESTS"
echo "========================================"
echo ""

# Set up test environment variables
export TEST_VAR="hello"
export USER_NAME="test_user"
export PATH_TEST="/usr/bin:/bin"
export EMPTY_VAR=""

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
    "echo \$TEST_VAR \$USER_NAME" \
    "hello test_user"

echo ""
echo "----------------------------------------"
echo "Special Variables"
echo "----------------------------------------"

# Test $? with a command that succeeds
test_expansion "Exit status after success" \
    $'pwd\necho $?' \
    "0"

# Test $? with exit command (note: exit terminates shell, so we test differently)
# We'll test with a failing builtin instead
test_expansion "Exit status after command" \
    $'cd /nonexistent 2>/dev/null\necho $?' \
    "1"

echo ""
echo "----------------------------------------"
echo "Undefined Variables"
echo "----------------------------------------"

test_expansion "Undefined variable expands to empty" \
    "echo \$UNDEFINED_VAR_TEST" \
    ""

test_expansion "Undefined with text" \
    "echo prefix\$UNDEFINED_VAR_TEST suffix" \
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
    'echo "$TEST_VAR" '\''$TEST_VAR'\''' \
    "hello \$TEST_VAR"

echo ""
echo "----------------------------------------"
echo "Complex Cases"
echo "----------------------------------------"

test_expansion "Variable with underscore in name" \
    "echo \$USER_NAME" \
    "test_user"

test_expansion "Variable followed by alphanumeric" \
    "echo \$TEST_VAR123" \
    ""

test_expansion "Multiple expansions" \
    "echo \$TEST_VAR \$TEST_VAR" \
    "hello hello"

test_expansion "Dollar sign not followed by variable" \
    "echo price\$" \
    "price$"

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
    echo -e "\n${RED}✗ Some expansion tests failed${NC}"
    exit 1
fi
