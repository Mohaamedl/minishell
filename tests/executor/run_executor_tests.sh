#!/bin/bash
# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   run_executor_tests.sh                              :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: mhaddadi <mhaddadi@student.42.fr>          +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/12/06 00:00:00 by mhaddadi          #+#    #+#             #
#   Updated: 2025/12/06 00:00:00 by mhaddadi         ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Executor Module Test Runner
# Tests KAN-52, KAN-53, KAN-54, KAN-55

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TEST_DIR="$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}EXECUTOR MODULE TEST SUITE${NC}"
echo -e "${BLUE}================================${NC}"
echo ""

# Navigate to project root
cd "$PROJECT_ROOT"

# Build the project first
echo -e "${YELLOW}[1/5] Building project...${NC}"
make re > /dev/null 2>&1 || {
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
}
echo -e "${GREEN}✓ Build successful${NC}"
echo ""

# Compile test files
echo -e "${YELLOW}[2/5] Compiling tests...${NC}"

CFLAGS="-Wall -Wextra -Werror -g"
INCLUDES="-I$PROJECT_ROOT/include -I$PROJECT_ROOT/Libft"
LIBS="-L$PROJECT_ROOT -L$PROJECT_ROOT/Libft -lft"

# Compile each test
gcc $CFLAGS $INCLUDES -o "$TEST_DIR/test_process" \
    "$TEST_DIR/test_process.c" \
    "$PROJECT_ROOT/src/executor/process.c" \
    "$PROJECT_ROOT/src/executor/wait.c" \
    $LIBS || {
    echo -e "${RED}✗ Failed to compile test_process${NC}"
    exit 1
}

gcc $CFLAGS $INCLUDES -o "$TEST_DIR/test_wait" \
    "$TEST_DIR/test_wait.c" \
    "$PROJECT_ROOT/src/executor/wait.c" \
    $LIBS || {
    echo -e "${RED}✗ Failed to compile test_wait${NC}"
    exit 1
}

gcc $CFLAGS $INCLUDES -o "$TEST_DIR/test_exec" \
    "$TEST_DIR/test_exec.c" \
    "$PROJECT_ROOT/src/executor/exec.c" \
    "$PROJECT_ROOT/src/environment/env_init.c" \
    "$PROJECT_ROOT/src/environment/env_get.c" \
    "$PROJECT_ROOT/src/environment/env_set.c" \
    "$PROJECT_ROOT/src/environment/env_utils.c" \
    "$PROJECT_ROOT/src/utils/string_utils.c" \
    "$PROJECT_ROOT/src/utils/string_utils2.c" \
    $LIBS || {
    echo -e "${RED}✗ Failed to compile test_exec${NC}"
    exit 1
}

gcc $CFLAGS $INCLUDES -o "$TEST_DIR/test_pipes" \
    "$TEST_DIR/test_pipes.c" \
    "$PROJECT_ROOT/src/executor/pipes.c" \
    "$PROJECT_ROOT/src/executor/pipes_utils.c" \
    $LIBS || {
    echo -e "${RED}✗ Failed to compile test_pipes${NC}"
    exit 1
}

echo -e "${GREEN}✓ All tests compiled${NC}"
echo ""

# Run tests
echo -e "${YELLOW}[3/5] Running tests...${NC}"
echo ""

FAILED=0

# Test 1: Process (KAN-53)
echo -e "${BLUE}Running test_process...${NC}"
if "$TEST_DIR/test_process"; then
    echo -e "${GREEN}✓ test_process PASSED${NC}"
else
    echo -e "${RED}✗ test_process FAILED${NC}"
    FAILED=$((FAILED + 1))
fi
echo ""

# Test 2: Wait (KAN-55)
echo -e "${BLUE}Running test_wait...${NC}"
if "$TEST_DIR/test_wait"; then
    echo -e "${GREEN}✓ test_wait PASSED${NC}"
else
    echo -e "${RED}✗ test_wait FAILED${NC}"
    FAILED=$((FAILED + 1))
fi
echo ""

# Test 3: Exec (KAN-54)
echo -e "${BLUE}Running test_exec...${NC}"
if "$TEST_DIR/test_exec"; then
    echo -e "${GREEN}✓ test_exec PASSED${NC}"
else
    echo -e "${RED}✗ test_exec FAILED${NC}"
    FAILED=$((FAILED + 1))
fi
echo ""

# Test 4: Pipes (KAN-52)
echo -e "${BLUE}Running test_pipes...${NC}"
if "$TEST_DIR/test_pipes"; then
    echo -e "${GREEN}✓ test_pipes PASSED${NC}"
else
    echo -e "${RED}✗ test_pipes FAILED${NC}"
    FAILED=$((FAILED + 1))
fi
echo ""

# Memory leak check with valgrind (if available)
if command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}[4/5] Running memory leak checks...${NC}"
    
    LEAK_FOUND=0
    
    for test in test_process test_wait test_exec test_pipes; do
        echo -e "${BLUE}Checking $test for leaks...${NC}"
        valgrind --leak-check=full --error-exitcode=1 --quiet \
            "$TEST_DIR/$test" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  ✓ No leaks in $test${NC}"
        else
            echo -e "${RED}  ✗ Memory leaks detected in $test${NC}"
            LEAK_FOUND=1
        fi
    done
    
    if [ $LEAK_FOUND -eq 0 ]; then
        echo -e "${GREEN}✓ No memory leaks detected${NC}"
    else
        echo -e "${RED}✗ Memory leaks found${NC}"
        FAILED=$((FAILED + 1))
    fi
else
    echo -e "${YELLOW}[4/5] Valgrind not found, skipping leak checks${NC}"
fi
echo ""

# FD leak check
echo -e "${YELLOW}[5/5] Checking for FD leaks...${NC}"
# The test_pipes already includes FD leak tests
echo -e "${GREEN}✓ FD leak checks included in pipe tests${NC}"
echo ""

# Cleanup
rm -f "$TEST_DIR/test_process" \
      "$TEST_DIR/test_wait" \
      "$TEST_DIR/test_exec" \
      "$TEST_DIR/test_pipes"

# Summary
echo -e "${BLUE}================================${NC}"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}ALL TESTS PASSED ✓${NC}"
    echo -e "${BLUE}================================${NC}"
    exit 0
else
    echo -e "${RED}$FAILED TEST(S) FAILED ✗${NC}"
    echo -e "${BLUE}================================${NC}"
    exit 1
fi
