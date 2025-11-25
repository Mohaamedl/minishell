#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_all_tests.sh                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/15 00:00:00 by mhaddadi          #+#    #+#              #
#    Updated: 2025/11/15 00:00:00 by mhaddadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# Banner
echo -e "${CYAN}"
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                                                                ║"
echo "║          MINISHELL COMPREHENSIVE TEST SUITE                    ║"
echo "║          Built-in Commands Testing Framework                   ║"
echo "║                                                                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Check if minishell exists
if [ ! -f "../../minishell" ]; then
    echo -e "${RED}ERROR: minishell executable not found!${NC}"
    echo -e "${YELLOW}Building minishell...${NC}"
    cd ../.. && make
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build failed! Exiting.${NC}"
        exit 1
    fi
    cd tests/phase1
    echo -e "${GREEN}Build successful!${NC}"
    echo
fi

# Test files
tests=(
    "test_echo.sh"
    "test_pwd.sh"
    "test_cd.sh"
    "test_env.sh"
    "test_export.sh"
    "test_unset.sh"
    "test_exit.sh"
)

# Results tracking
total_suites=0
passed_suites=0
failed_suites=0

echo -e "${MAGENTA}════════════════════════════════════════════════════════════════${NC}"
echo -e "${MAGENTA}                    RUNNING TEST SUITES                         ${NC}"
echo -e "${MAGENTA}════════════════════════════════════════════════════════════════${NC}"
echo

# Run each test suite
for test in "${tests[@]}"; do
    if [ -f "$test" ]; then
        ((total_suites++))
        
        test_name=$(basename "$test" .sh)
        echo -e "${BLUE}┌────────────────────────────────────────────────────────────┐${NC}"
        echo -e "${BLUE}│ Running: ${test_name}${NC}"
        echo -e "${BLUE}└────────────────────────────────────────────────────────────┘${NC}"
        
        # Run the test
        ./"$test"
        result=$?
        
        if [ $result -eq 0 ]; then
            ((passed_suites++))
            echo -e "${GREEN}✓ ${test_name} PASSED${NC}"
        else
            ((failed_suites++))
            echo -e "${RED}✗ ${test_name} FAILED${NC}"
        fi
        echo
        echo -e "${BLUE}────────────────────────────────────────────────────────────${NC}"
        echo
    else
        echo -e "${YELLOW}Warning: ${test} not found!${NC}"
        echo
    fi
done

# Final summary
echo -e "${CYAN}"
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                                                                ║"
echo "║                      FINAL TEST SUMMARY                        ║"
echo "║                                                                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

echo -e "Total test suites run: ${total_suites}"
echo -e "${GREEN}Passed: ${passed_suites}${NC}"
echo -e "${RED}Failed: ${failed_suites}${NC}"

if [ $total_suites -gt 0 ]; then
    success_rate=$(awk "BEGIN {printf \"%.1f\", ($passed_suites/$total_suites)*100}")
    echo -e "Success rate: ${success_rate}%"
fi

echo

if [ $failed_suites -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}║  🎉 🎊 ALL TEST SUITES PASSED! EXCELLENT WORK! 🎊 🎉          ║${NC}"
    echo -e "${GREEN}║                                                                ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║                                                                ║${NC}"
    echo -e "${RED}║  ❌ SOME TEST SUITES FAILED - REVIEW RESULTS ABOVE             ║${NC}"
    echo -e "${RED}║                                                                ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════════════════════╝${NC}"
    exit 1
fi
