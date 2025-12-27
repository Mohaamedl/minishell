#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

MINISHELL=./minishell
TOTAL=0
PASSED=0
FAILED=0

test_unclosed_quote() {
    local test_name="$1"
    local input="$2"
    local expected_exit=2  # SYNTAX_ERROR
    
    TOTAL=$((TOTAL + 1))
    
    # Run test and capture exit code
    echo "$input" | $MINISHELL > /dev/null 2>&1
    exit_code=$?
    
    # Check that it doesn't segfault (exit 139) and returns syntax error (2)
    if [ $exit_code -ne 139 ] && [ $exit_code -eq $expected_exit ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected exit: $expected_exit, Got: $exit_code"
        if [ $exit_code -eq 139 ]; then
            echo "  ERROR: Segmentation fault detected!"
        fi
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "UNCLOSED QUOTES TESTS (Issue #25)"
echo "========================================"
echo ""

test_unclosed_quote "Unclosed double quote" '"'
test_unclosed_quote "Unclosed double quote with text" '"hello'
test_unclosed_quote "Unclosed double quote with asterisk" '"*'
test_unclosed_quote "Unclosed single quote" "'"
test_unclosed_quote "Unclosed single quote with text" "'hello"
test_unclosed_quote "Unclosed single quote with asterisk" "'*"
test_unclosed_quote "Mixed quotes - unclosed double" "echo 'hello' \"world"
test_unclosed_quote "Mixed quotes - unclosed single" "echo \"hello\" 'world"

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo "Total tests:  $TOTAL"
echo "Passed:       $PASSED"
echo "Failed:       $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All unclosed quotes tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi
