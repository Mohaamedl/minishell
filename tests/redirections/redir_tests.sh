#!/bin/bash
MSH="../../minishell"
# Cores
GREEN="\e[32m"
RED="\e[31m"
RESET="\e[0m"
mkdir -p tmp_bash tmp_mini
echo "Teste de entrada" > input.txt

cleanup_folders() {
    rm input.txt
    rm -rf tmp_bash tmp_mini
}

compare_folders() {
    local name="$1"
    echo -e "\n=== TEST: $name ==="
    local diff_output
    diff_output=$(diff -ur tmp_bash tmp_mini)
    if [ -z "$diff_output" ]; then
        echo -e "${GREEN}SUCCESS${RESET} - Files are identical"
    else
       echo -e "${RED}ERROR${RESET} - Files differ:"
        # Colorir as linhas do diff
        echo "$diff_output" | while IFS= read -r line; do
            if [[ "$line" == +* ]]; then
                echo -e "${GREEN}$line${RESET}"
            elif [[ "$line" == -* ]]; then
                echo -e "${RED}$line${RESET}"
            else
                echo "$line"
            fi
        done
    fi
}

# Função para executar no Bash
run_test_bash() {
    local name="$1"
    local cmd="$2"
    eval "$cmd"
}
# Função para executar no Minishell
run_test_mini() {
    local name="$1"
    local cmd="$2"
    printf "%b\n" "$cmd" | $MSH
}

###############################################
# TESTES
###############################################
rm -f tmp_bash/* tmp_mini/*
# Saída overwrite
run_test_bash "saida_overwrite" \
"echo 'Primeira linha' > tmp_bash/saida_overwrite.txt
echo 'Segunda linha' > tmp_bash/saida_overwrite.txt"

run_test_mini "saida_overwrite" \
"echo 'Primeira linha' > tmp_mini/saida_overwrite.txt
echo 'Segunda linha' > tmp_mini/saida_overwrite.txt"

compare_folders "saida_overwrite"
rm -f tmp_bash/* tmp_mini/*

# Saída append
run_test_bash "saida_append" \
"echo 'Linha A' >> tmp_bash/saida_append.txt
echo 'Linha B' >> tmp_bash/saida_append.txt"

run_test_mini "saida_append" \
"echo 'Linha A' >> tmp_mini/saida_append.txt
echo 'Linha B' >> tmp_mini/saida_append.txt"

compare_folders "saida_append"
rm -f tmp_bash/* tmp_mini/*

# Redireção de entrada
run_test_bash "entrada_redir" \
"cat < input.txt > tmp_bash/saida_entrada.txt"

run_test_mini "entrada_redir" \
"cat < input.txt > tmp_mini/saida_entrada.txt"

compare_folders "entrada_redir"
rm -f tmp_bash/* tmp_mini/*
###############################################
# MULTIPLAS REDIREÇÕES
###############################################


run_test_bash "mult_redir_overwrite" \
"echo 'Mult Redir' > tmp_bash/mult1.txt > tmp_bash/mult2.txt"

run_test_mini "mult_redir_overwrite" \
"echo 'Mult Redir' > tmp_mini/mult1.txt > tmp_mini/mult2.txt"

compare_folders "mult_redir_overwrite"
rm -f tmp_bash/* tmp_mini/*


run_test_bash "mult_redir_append" \
"echo 'Append 1' >> tmp_bash/mult_append1.txt >> tmp_bash/mult_append2.txt"

run_test_mini "mult_redir_append" \
"echo 'Append 1' >> tmp_mini/mult_append1.txt >> tmp_mini/mult_append2.txt"

compare_folders "mult_redir_append"
rm -f tmp_bash/* tmp_mini/*

cleanup_folders