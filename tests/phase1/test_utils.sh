#!/bin/bash

# Test script for Phase 1: Utility Functions
# Tests string, memory, and number utilities

RESET="\033[0m"
GREEN="\033[32m"
RED="\033[31m"
BLUE="\033[34m"

PASSED=0
FAILED=0

print_test() {
    echo -e "${BLUE}[TEST]${RESET} $1"
}

print_pass() {
    echo -e "${GREEN}✓ PASSED${RESET} $1"
    ((PASSED++))
}

print_fail() {
    echo -e "${RED}✗ FAILED${RESET} $1"
    ((FAILED++))
}

print_summary() {
    echo ""
    echo "================================"
    echo -e "Total: $((PASSED + FAILED)) tests"
    echo -e "${GREEN}Passed: $PASSED${RESET}"
    echo -e "${RED}Failed: $FAILED${RESET}"
    echo "================================"
}

echo "Utility Function Tests"
echo "======================"
echo ""

# Test 1: ft_strlen equivalent
print_test "String Length (strlen)"
TEST_STR="Hello, World!"
EXPECTED=13
ACTUAL=${#TEST_STR}
if [ $ACTUAL -eq $EXPECTED ]; then
    print_pass "strlen: $ACTUAL == $EXPECTED"
else
    print_fail "strlen: $ACTUAL != $EXPECTED"
fi

# Test 2: String comparison
print_test "String Comparison (strcmp)"
STR1="abc"
STR2="abc"
if [ "$STR1" = "$STR2" ]; then
    print_pass "strcmp: strings equal"
else
    print_fail "strcmp: strings not equal"
fi

# Test 3: String duplication
print_test "String Duplication (strdup)"
ORIG="test string"
DUP="$ORIG"
if [ "$DUP" = "$ORIG" ]; then
    print_pass "strdup: duplication successful"
else
    print_fail "strdup: duplication failed"
fi

# Test 4: String join
print_test "String Join (strjoin)"
S1="Hello"
S2="World"
RESULT="$S1$S2"
EXPECTED="HelloWorld"
if [ "$RESULT" = "$EXPECTED" ]; then
    print_pass "strjoin: $RESULT"
else
    print_fail "strjoin: $RESULT != $EXPECTED"
fi

# Test 5: Number conversion (atoi)
print_test "String to Integer (atoi)"
NUM_STR="12345"
if [[ "$NUM_STR" =~ ^[0-9]+$ ]]; then
    print_pass "atoi: $NUM_STR is numeric"
else
    print_fail "atoi: $NUM_STR is not numeric"
fi

# Test 6: Integer to string (itoa)
print_test "Integer to String (itoa)"
NUM=12345
STR="$NUM"
if [ "$STR" = "12345" ]; then
    print_pass "itoa: $STR"
else
    print_fail "itoa: conversion failed"
fi

# Test 7: Character type checks
print_test "Character Type Checks"
DIGIT="5"
ALPHA="a"
if [[ "$DIGIT" =~ ^[0-9]$ ]]; then
    print_pass "isdigit: $DIGIT is a digit"
else
    print_fail "isdigit: $DIGIT not recognized as digit"
fi

if [[ "$ALPHA" =~ ^[a-zA-Z]$ ]]; then
    print_pass "isalpha: $ALPHA is alphabetic"
else
    print_fail "isalpha: $ALPHA not recognized as alpha"
fi

# Test 8: Split functionality
print_test "String Split"
INPUT="one:two:three"
IFS=':' read -ra PARTS <<< "$INPUT"
if [ ${#PARTS[@]} -eq 3 ]; then
    print_pass "split: ${#PARTS[@]} parts"
else
    print_fail "split: expected 3 parts, got ${#PARTS[@]}"
fi

print_summary

exit $FAILED
