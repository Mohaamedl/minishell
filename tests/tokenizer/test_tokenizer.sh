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
test_tokenizer() {
    local test_name="$1"
    local input="$2"
    local expected_token_count="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output and exit code
    output=$(echo "$input" | $MINISHELL 2>&1)
    exit_code=$?
    
    # For now, we test if the command doesn't crash
    # Accept: 0 (success), 1 (command error), 130 (Ctrl+C), 2 (syntax error)
    # Reject: 139 (segfault), 124 (timeout)
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 2 ] || [ $exit_code -eq 130 ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Exit code: $exit_code"
        if [ $exit_code -eq 139 ]; then
            echo "  ERROR: Segmentation fault!"
        elif [ $exit_code -eq 124 ]; then
            echo "  ERROR: Timeout!"
        fi
        FAILED=$((FAILED + 1))
    fi
}


echo "========================================"
echo "TOKENIZER TESTS"
echo "========================================"

echo ""
echo "----------------------------------------"
echo "Basic Tokenization"
echo "----------------------------------------"

test_tokenizer "Simple command" "echo hello" 2
test_tokenizer "Command with multiple args" "echo hello world" 3
test_tokenizer "Command with flags" "ls -la" 2
test_tokenizer "Multiple spaces" "echo    hello    world" 3

echo ""
echo "----------------------------------------"
echo "Quote Tokenization"
echo "----------------------------------------"

test_tokenizer "Single quotes" "echo 'hello world'" 2
test_tokenizer "Double quotes" 'echo "hello world"' 2
test_tokenizer "Mixed quotes" "echo 'hello' \"world\"" 3
test_tokenizer "Nested quotes (single in double)" 'echo "hello '\''world'\''"' 2
test_tokenizer "Empty single quotes" "echo ''" 2
test_tokenizer "Empty double quotes" 'echo ""' 2

echo ""
echo "----------------------------------------"
echo "Special Characters"
echo "----------------------------------------"

test_tokenizer "Pipe operator" "echo hello | cat" 4
test_tokenizer "Input redirection" "cat < file" 3
test_tokenizer "Output redirection" "echo hello > file" 4
test_tokenizer "Append redirection" "echo hello >> file" 4
test_tokenizer "Heredoc" "cat << EOF" 3
test_tokenizer "Multiple redirections" "cat < in > out" 5

echo ""
echo "----------------------------------------"
echo "Logical Operators"
echo "----------------------------------------"

test_tokenizer "AND operator" "echo hello && echo world" 5
test_tokenizer "OR operator" "echo hello || echo world" 5
test_tokenizer "Parentheses" "(echo hello)" 3

echo ""
echo "----------------------------------------"
echo "Variable Tokens"
echo "----------------------------------------"

test_tokenizer "Environment variable" "echo \$USER" 2
test_tokenizer "Exit status" "echo \$?" 2
test_tokenizer "Multiple variables" "echo \$USER \$HOME" 3
test_tokenizer "Variable in quotes" 'echo "$USER"' 2

echo ""
echo "----------------------------------------"
echo "Edge Cases"
echo "----------------------------------------"

test_tokenizer "Command only" "echo" 1
test_tokenizer "Multiple pipes" "cat | grep x | wc -l" 7
test_tokenizer "Redirection without space" "cat<file" 3
test_tokenizer "Multiple operators" "echo hello && echo world || echo fail" 9

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All tokenizer tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
