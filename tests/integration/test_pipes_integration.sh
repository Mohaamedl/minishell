#!/bin/bash
# Integration test for pipe execution

echo "=== PIPE EXECUTION INTEGRATION TESTS ==="
echo ""

MINISHELL="./minishell"
PASS=0
FAIL=0

test_pipe() {
    local cmd="$1"
    local expected="$2"
    local result

    result=$(echo "$cmd" | $MINISHELL 2>&1)
    
    if [ "$result" == "$expected" ]; then
        echo "✓ PASS: $cmd"
        ((PASS++))
    else
        echo "✗ FAIL: $cmd"
        echo "  Expected: $expected"
        echo "  Got: $result"
        ((FAIL++))
    fi
}

# Test 1: Basic echo pipe
test_pipe "echo hello | cat" "hello"

# Test 2: ls with grep
echo "ls | grep README" | $MINISHELL > /tmp/test_output.txt
if grep -q "README.md" /tmp/test_output.txt; then
    echo "✓ PASS: ls | grep README"
    ((PASS++))
else
    echo "✗ FAIL: ls | grep README"
    ((FAIL++))
fi

# Test 3: cat with grep
echo "cat Makefile | grep NAME" | $MINISHELL > /tmp/test_output.txt
if grep -q "NAME" /tmp/test_output.txt; then
    echo "✓ PASS: cat Makefile | grep NAME"
    ((PASS++))
else
    echo "✗ FAIL: cat Makefile | grep NAME"
    ((FAIL++))
fi

# Test 4: echo with wc
result=$(echo "echo -e 'line1\nline2\nline3' | wc -l" | $MINISHELL 2>&1)
if [ "$result" == "3" ]; then
    echo "✓ PASS: echo with wc -l"
    ((PASS++))
else
    echo "✗ FAIL: echo with wc -l"
    ((FAIL++))
fi

# Cleanup
rm -f /tmp/test_output.txt

echo ""
echo "==========================="
echo "Results: $PASS passed, $FAIL failed"
if [ $FAIL -eq 0 ]; then
    echo "✓ ALL TESTS PASSED"
    exit 0
else
    echo "✗ SOME TESTS FAILED"
    exit 1
fi
