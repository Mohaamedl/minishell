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

# Test function - tests if command is parsed without syntax errors
test_parser() {
    local test_name="$1"
    local input="$2"
    local should_pass="$3"  # 1 for valid syntax, 0 for invalid
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(echo "$input" | $MINISHELL 2>&1)
    exit_code=$?
    
    # Check for syntax error in output
    if echo "$output" | grep -qi "syntax error"; then
        is_syntax_error=1
    else
        is_syntax_error=0
    fi
    
    if [ "$should_pass" -eq 1 ]; then
        # Should be valid syntax
        if [ $is_syntax_error -eq 0 ]; then
            echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
            echo "  Input: $input"
            echo "  Expected: Valid syntax"
            echo "  Got: Syntax error"
            FAILED=$((FAILED + 1))
        fi
    else
        # Should be invalid syntax
        if [ $is_syntax_error -eq 1 ]; then
            echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
            echo "  Input: $input"
            echo "  Expected: Syntax error"
            echo "  Got: No error"
            FAILED=$((FAILED + 1))
        fi
    fi
}

echo "========================================"
echo "PARSER TESTS"
echo "========================================"

echo ""
echo "----------------------------------------"
echo "Valid Syntax"
echo "----------------------------------------"

test_parser "Simple command" "echo hello" 1
test_parser "Command with pipes" "cat file | grep test" 1
test_parser "Multiple pipes" "cat | grep x | wc -l" 1
test_parser "Input redirection" "cat < file" 1
test_parser "Output redirection" "echo hello > file" 1
test_parser "Append redirection" "echo hello >> file" 1
test_parser "Multiple redirections" "cat < in > out" 1
test_parser "Heredoc" "cat << EOF" 1
test_parser "AND operator" "echo hello && echo world" 1
test_parser "OR operator" "echo hello || echo world" 1
test_parser "Parentheses grouping" "(echo hello)" 1
test_parser "Complex command" "(echo a && echo b) || echo c" 1

echo ""
echo "----------------------------------------"
echo "Invalid Syntax"
echo "----------------------------------------"

test_parser "Unclosed quotes (single)" "echo 'hello" 0
test_parser "Unclosed quotes (double)" 'echo "hello' 0
test_parser "Pipe at start" "| echo hello" 0
test_parser "Pipe at end" "echo hello |" 0
test_parser "Double pipe" "echo hello || echo world" 1  # OR operator is valid
test_parser "Redirection without file" "echo >" 0
test_parser "Redirection without command" "> file" 1  # bash allows this - creates empty file
test_parser "Unclosed parentheses" "(echo hello" 0
test_parser "Mismatched parentheses" "echo hello)" 0
test_parser "Empty parentheses" "()" 0

echo ""
echo "----------------------------------------"
echo "Edge Cases"
echo "----------------------------------------"

test_parser "Empty command" "" 1
test_parser "Only spaces" "   " 1
test_parser "Command with tabs" "echo\thello" 1
test_parser "Multiple semicolons (not supported)" "echo a ; echo b" 0
test_parser "Backslash (not supported)" "echo hello\ world" 1

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All parser tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
