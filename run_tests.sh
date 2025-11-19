#!/usr/bin/env bash

EXEC=./minishell
TEST_DIR=tests
PASS=0
FAIL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'


for infile in "$TEST_DIR"/*.in; do
    outfile="${infile%.in}.out"
    testname=$(basename "$infile")

    # Lê a expressão do ficheiro .in
    expr=$(cat "$infile")

    # Executa o parser lendo da stdin
    output=$("$EXEC" <<< "$expr")

	# Remove linhas que começam com "minishell$>"
    output_clean=$(echo "$output" | grep -v '^minishell\$>')
    expected_clean=$(sed 's/[ \t]*$//' "$outfile")

    # Compara com diff
    if diff -u <(printf "%s\n" "$expected_clean") <(printf "%s\n" "$output_clean") > /dev/null; then
        echo -e "${GREEN}PASS${RESET}: $testname"
        PASS=$((PASS+1))
    else
        echo -e "${RED}FAIL${RESET}: $testname"
        echo "Expected:"
        echo "$expected_clean"
        echo "Got:"
        echo "$output_clean"
        FAIL=$((FAIL+1))
        echo "----------------------"
    fi
done

# Resumo
echo
echo "SUMMARY:"
echo -e "  ${GREEN}PASS${RESET}: $PASS"
echo -e "  ${RED}FAIL${RESET}: $FAIL"

# Código de saída indica sucesso/falha
if [ "$FAIL" -eq 0 ]; then
    exit 0
else
    exit 1
fi
