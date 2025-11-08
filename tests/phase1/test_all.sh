#!/bin/bash

# Master test script for Phase 1
# Runs all Phase 1 tests

RESET="\033[0m"
GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[34m"
YELLOW="\033[33m"

echo -e "${BLUE}======================================${RESET}"
echo -e "${BLUE}    PHASE 1: FOUNDATION TESTS${RESET}"
echo -e "${BLUE}======================================${RESET}"
echo ""

TOTAL_FAILED=0

# Test 1: Environment Management
echo -e "${YELLOW}Running Environment Tests...${RESET}"
bash tests/phase1/test_environment.sh
ENV_RESULT=$?
TOTAL_FAILED=$((TOTAL_FAILED + ENV_RESULT))
echo ""

# Test 2: Signal Handling
echo -e "${YELLOW}Running Signal Tests...${RESET}"
bash tests/phase1/test_signals.sh
SIG_RESULT=$?
TOTAL_FAILED=$((TOTAL_FAILED + SIG_RESULT))
echo ""

# Test 3: Utility Functions (basic check)
echo -e "${YELLOW}Running Utility Function Tests...${RESET}"
bash tests/phase1/test_utils.sh
UTILS_RESULT=$?
TOTAL_FAILED=$((TOTAL_FAILED + UTILS_RESULT))
echo ""

# Summary
echo -e "${BLUE}======================================${RESET}"
echo -e "${BLUE}           FINAL RESULTS${RESET}"
echo -e "${BLUE}======================================${RESET}"
if [ $ENV_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${RESET} Environment Tests: PASSED"
else
    echo -e "${RED}✗${RESET} Environment Tests: FAILED"
fi

if [ $SIG_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${RESET} Signal Tests: PASSED"
else
    echo -e "${RED}✗${RESET} Signal Tests: FAILED"
fi

if [ $UTILS_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓${RESET} Utility Tests: PASSED"
else
    echo -e "${RED}✗${RESET} Utility Tests: FAILED"
fi

echo ""
if [ $TOTAL_FAILED -eq 0 ]; then
    echo -e "${GREEN}All Phase 1 tests passed!${RESET}"
else
    echo -e "${RED}Some tests failed. Total failures: $TOTAL_FAILED${RESET}"
fi

exit $TOTAL_FAILED
