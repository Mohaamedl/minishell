#!/bin/bash

# Minishell Test Suite - Master Runner
# Runs all tests across all modules

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Global counters
TOTAL_SUITES=0
PASSED_SUITES=0
FAILED_SUITES=0

# Test results storage
declare -a SUITE_RESULTS
declare -a SUITE_NAMES

# Function to run a test suite
run_suite() {
    local suite_path="$1"
    local suite_name="$2"
    
    TOTAL_SUITES=$((TOTAL_SUITES + 1))
    
    echo ""
    echo -e "${CYAN}========================================"
    echo -e "Running: ${suite_name}"
    echo -e "========================================${NC}"
    echo ""
    
    # Make executable and run
    chmod +x "$suite_path"
    bash "$suite_path"
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        PASSED_SUITES=$((PASSED_SUITES + 1))
        SUITE_RESULTS+=("PASS")
        echo -e "${GREEN}✓ ${suite_name} completed successfully${NC}"
    else
        FAILED_SUITES=$((FAILED_SUITES + 1))
        SUITE_RESULTS+=("FAIL")
        echo -e "${RED}✗ ${suite_name} failed${NC}"
    fi
    
    SUITE_NAMES+=("$suite_name")
}

# Banner
echo -e "${BLUE}"
echo "╔════════════════════════════════════════╗"
echo "║     MINISHELL COMPREHENSIVE TEST SUITE ║"
echo "║                                        ║"
echo "║  Testing all modules and components   ║"
echo "╚════════════════════════════════════════╝"
echo -e "${NC}"

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell binary not found!${NC}"
    echo "Please run 'make' first to build the project."
    exit 1
fi

echo -e "${YELLOW}Starting test execution...${NC}"
echo ""

# Run all test suites in order
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}UNIT TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Tokenizer tests
if [ -f "tests/tokenizer/test_tokenizer.sh" ]; then
    run_suite "tests/tokenizer/test_tokenizer.sh" "Tokenizer Tests"
fi

# Parser tests
if [ -f "tests/parser/test_parser.sh" ]; then
    run_suite "tests/parser/test_parser.sh" "Parser Tests"
fi

# AST tests
if [ -f "tests/ast/test_ast.sh" ]; then
    run_suite "tests/ast/test_ast.sh" "AST Tests"
fi

# Expander tests
if [ -f "tests/expander/test_expansion.sh" ]; then
    run_suite "tests/expander/test_expansion.sh" "Variable Expansion Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}BUILTIN COMMAND TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Builtin tests
if [ -f "tests/builtins/test_echo.sh" ]; then
    run_suite "tests/builtins/test_echo.sh" "Echo Builtin Tests"
fi

if [ -f "tests/builtins/test_cd.sh" ]; then
    run_suite "tests/builtins/test_cd.sh" "CD Builtin Tests"
fi

if [ -f "tests/builtins/test_pwd.sh" ]; then
    run_suite "tests/builtins/test_pwd.sh" "PWD Builtin Tests"
fi

if [ -f "tests/builtins/test_env.sh" ]; then
    run_suite "tests/builtins/test_env.sh" "ENV Builtin Tests"
fi

if [ -f "tests/builtins/test_export.sh" ]; then
    run_suite "tests/builtins/test_export.sh" "Export Builtin Tests"
fi

if [ -f "tests/builtins/test_unset.sh" ]; then
    run_suite "tests/builtins/test_unset.sh" "Unset Builtin Tests"
fi

if [ -f "tests/builtins/test_exit.sh" ]; then
    run_suite "tests/builtins/test_exit.sh" "Exit Builtin Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}EXECUTOR & PROCESS TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Executor tests
if [ -f "tests/executor/run_executor_tests.sh" ]; then
    run_suite "tests/executor/run_executor_tests.sh" "Executor Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}INTEGRATION TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Integration tests
if [ -f "tests/integration/test_pipes_integration.sh" ]; then
    run_suite "tests/integration/test_pipes_integration.sh" "Pipes Integration Tests"
fi

if [ -f "tests/integration/test_repl_improvements.sh" ]; then
    run_suite "tests/integration/test_repl_improvements.sh" "REPL Improvements Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}REDIRECTION TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Redirection tests
if [ -f "tests/redirections/redir_tests.sh" ]; then
    run_suite "tests/redirections/redir_tests.sh" "Redirection Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}EXPANSION & WILDCARD TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Wildcard tests
if [ -f "tests/expander/test_wildcard.sh" ]; then
    run_suite "tests/expander/test_wildcard.sh" "Wildcard Expansion Tests"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}BONUS TESTS${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Bonus tests (logical operators)
if [ -f "tests/bonus/test_logical_operators.sh" ]; then
    run_suite "tests/bonus/test_logical_operators.sh" "Logical Operators Tests"
fi

# Final Summary
echo ""
echo ""
echo -e "${BLUE}╔════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║          FINAL TEST SUMMARY            ║${NC}"
echo -e "${BLUE}╔════════════════════════════════════════╗${NC}"
echo ""

# Display results for each suite
for i in "${!SUITE_NAMES[@]}"; do
    name="${SUITE_NAMES[$i]}"
    result="${SUITE_RESULTS[$i]}"
    
    if [ "$result" == "PASS" ]; then
        echo -e "  ${GREEN}✓${NC} ${name}"
    else
        echo -e "  ${RED}✗${NC} ${name}"
    fi
done

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "Total Test Suites:  ${TOTAL_SUITES}"
echo -e "Passed:             ${GREEN}${PASSED_SUITES}${NC}"
echo -e "Failed:             ${RED}${FAILED_SUITES}${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Calculate success rate
if [ $TOTAL_SUITES -gt 0 ]; then
    success_rate=$((PASSED_SUITES * 100 / TOTAL_SUITES))
    echo ""
    echo -e "Success Rate: ${success_rate}%"
fi

echo ""

# Final verdict
if [ $FAILED_SUITES -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║   ✓ ALL TESTS PASSED SUCCESSFULLY!    ║${NC}"
    echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
    echo ""
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════╗${NC}"
    echo -e "${RED}║     ✗ SOME TESTS FAILED                ║${NC}"
    echo -e "${RED}╔════════════════════════════════════════╗${NC}"
    echo ""
    exit 1
fi
