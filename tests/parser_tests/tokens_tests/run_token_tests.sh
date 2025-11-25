#!/bin/bash

MINISHELL=../../../minishell
TEST_FILE=tests_data/cmds.txt

GREEN="\033[1;32m"
RED="\033[1;31m"
RESET="\033[0m"

run_test() {
    CMD="$1"
    echo -e "\n=== Test: $CMD ==="

    echo "--- Bash output ---"
    # Executa no Bash e mostra tudo
    bash -c "$CMD"

    echo "--- Minishell output ---"
    # Executa no Minishell e mostra tudo
    $echo "$CMD" | $MINISHELL

}

echo "Running tests from $TEST_FILE"

while IFS= read -r LINE || [[ -n "$LINE" ]]; do
    # Ignora linhas vazias ou comentários
    [[ -z "$LINE" || "$LINE" =~ ^# ]] && continue
    run_test "$LINE"
done < "$TEST_FILE"
