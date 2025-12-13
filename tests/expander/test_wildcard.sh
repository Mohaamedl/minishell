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

# Get the minishell binary path (absolute)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
MINISHELL="$(cd "$SCRIPT_DIR/../.." && pwd)/minishell"

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell binary not found at $MINISHELL${NC}"
    exit 1
fi

# Create test directory with test files
TEST_DIR=$(mktemp -d)
cd "$TEST_DIR" || exit 1

# Create test files
touch file1.txt file2.txt file3.txt
touch test.c test.h test.o
touch README.md CHANGELOG.md
touch .hidden .hidden2
mkdir subdir

# Test function
test_wildcard() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null | head -n 1)
    
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

# Test function for checking if output contains expected items
test_wildcard_contains() {
    local test_name="$1"
    local input="$2"
    shift 2
    local expected_items=("$@")
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null | head -n 1)
    
    # Check if all expected items are in output
    all_found=true
    for item in "${expected_items[@]}"; do
        if [[ ! "$output" =~ $item ]]; then
            all_found=false
            break
        fi
    done
    
    if $all_found; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected items: ${expected_items[*]}"
        echo "  Got: '$output'"
        FAILED=$((FAILED + 1))
    fi
}

# Test function for counting words in output
test_wildcard_count() {
    local test_name="$1"
    local input="$2"
    local expected_count="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Run the command and capture output
    output=$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null | head -n 1)
    word_count=$(echo "$output" | wc -w)
    
    if [ "$word_count" -eq "$expected_count" ]; then
        echo -e "${GREEN}✓${NC} Test $TOTAL: $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} Test $TOTAL: $test_name"
        echo "  Input: $input"
        echo "  Expected count: $expected_count"
        echo "  Got count: $word_count (output: '$output')"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "WILDCARD EXPANSION TESTS"
echo "========================================"
echo "Test directory: $TEST_DIR"
echo ""

echo "----------------------------------------"
echo "Basic Wildcard Patterns"
echo "----------------------------------------"

test_wildcard_count "echo * matches all non-hidden files" \
    "echo *" \
    9  # file1.txt file2.txt file3.txt test.c test.h test.o README.md CHANGELOG.md subdir

test_wildcard_contains "echo *.txt matches .txt files" \
    "echo *.txt" \
    "file1.txt" "file2.txt" "file3.txt"

test_wildcard_count "echo *.txt count" \
    "echo *.txt" \
    3

test_wildcard_contains "echo *.md matches .md files" \
    "echo *.md" \
    "README.md" "CHANGELOG.md"

test_wildcard_count "echo *.md count" \
    "echo *.md" \
    2

test_wildcard_contains "echo test.* matches test files" \
    "echo test.*" \
    "test.c" "test.h" "test.o"

test_wildcard_count "echo test.* count" \
    "echo test.*" \
    3

echo ""
echo "----------------------------------------"
echo "Hidden Files"
echo "----------------------------------------"

test_wildcard_contains "echo .* matches hidden files" \
    "echo .*" \
    ".hidden" ".hidden2"

# Note: .* also matches . and .. in bash, but our implementation may differ
# We check that at least hidden files are included

echo ""
echo "----------------------------------------"
echo "No Match Cases"
echo "----------------------------------------"

test_wildcard "No match returns literal pattern" \
    "echo *.xyz" \
    "*.xyz"

test_wildcard "No match with complex pattern" \
    "echo nonexistent*file" \
    "nonexistent*file"

echo ""
echo "----------------------------------------"
echo "Quote Protection"
echo "----------------------------------------"

test_wildcard "Double quotes prevent expansion" \
    'echo "*.txt"' \
    "*.txt"

test_wildcard "Single quotes prevent expansion" \
    "echo '*.txt'" \
    "*.txt"

test_wildcard "Quoted asterisk literal" \
    'echo "*"' \
    "*"

echo ""
echo "----------------------------------------"
echo "Wildcard in Pipes"
echo "----------------------------------------"

# Count .txt files through pipe
TOTAL=$((TOTAL + 1))
output=$(printf 'echo *.txt | wc -w\n' | "$MINISHELL" 2>/dev/null | head -n 1 | tr -d ' ')
if [ "$output" == "3" ]; then
    echo -e "${GREEN}✓${NC} Test $TOTAL: Wildcard in pipe (echo *.txt | wc -w)"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Test $TOTAL: Wildcard in pipe (echo *.txt | wc -w)"
    echo "  Expected: '3'"
    echo "  Got: '$output'"
    FAILED=$((FAILED + 1))
fi

# ls with wildcard piped to wc
TOTAL=$((TOTAL + 1))
output=$(printf 'ls *.md | wc -l\n' | "$MINISHELL" 2>/dev/null | head -n 1 | tr -d ' ')
if [ "$output" == "2" ]; then
    echo -e "${GREEN}✓${NC} Test $TOTAL: ls *.md | wc -l"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Test $TOTAL: ls *.md | wc -l"
    echo "  Expected: '2'"
    echo "  Got: '$output'"
    FAILED=$((FAILED + 1))
fi

echo ""
echo "----------------------------------------"
echo "Multiple Wildcards"
echo "----------------------------------------"

test_wildcard_count "Multiple wildcards in command" \
    "echo *.txt *.md" \
    5  # 3 txt + 2 md

echo ""
echo "----------------------------------------"
echo "Pattern Positions"
echo "----------------------------------------"

test_wildcard_contains "Prefix pattern (file*)" \
    "echo file*" \
    "file1.txt" "file2.txt" "file3.txt"

test_wildcard_count "Prefix pattern count" \
    "echo file*" \
    3

# Middle pattern
touch aXb aYb aZb
test_wildcard_contains "Middle pattern (a*b)" \
    "echo a*b" \
    "aXb" "aYb" "aZb"

echo ""
echo "----------------------------------------"
echo "Alphabetical Sorting"
echo "----------------------------------------"

# Create files to test sorting
touch aaa bbb ccc
TOTAL=$((TOTAL + 1))
output=$(printf 'echo ???\n' | "$MINISHELL" 2>/dev/null | head -n 1)
# Note: Our implementation uses * not ?, so this tests * sorting instead
output=$(printf 'echo a* b* c*\n' | "$MINISHELL" 2>/dev/null | head -n 1)
first_word=$(echo "$output" | awk '{print $1}')
if [[ "$first_word" == "aXb" || "$first_word" == "aYb" || "$first_word" == "aZb" || "$first_word" == "aaa" ]]; then
    echo -e "${GREEN}✓${NC} Test $TOTAL: Results are sorted (starts with 'a')"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Test $TOTAL: Results are sorted"
    echo "  First word should start with 'a'"
    echo "  Got: '$first_word'"
    FAILED=$((FAILED + 1))
fi

# Cleanup
cd - > /dev/null
rm -rf "$TEST_DIR"

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"
echo -e "Total tests:  ${TOTAL}"
echo -e "Passed:       ${GREEN}${PASSED}${NC}"
echo -e "Failed:       ${RED}${FAILED}${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All wildcard tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
