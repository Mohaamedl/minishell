/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/14 12:04:41 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;
/**
 * @brief Main entry point of the minishell.
 * * Initializes the shell structure, enters the main command loop, 
 * and performs final cleanup before exiting with the last recorded 
 * exit status.
 * * @param argc Argument count (unused).
 * @param argv Argument vector (unused).
 * @param envp Environment variables from the system.
 * @return int The last command's exit status.
 */
int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell_struct(&shell, envp);
    shell_loop(&shell);
    free_env(shell.env_list);
    return (shell.last_exit_status);
}

/**
 * @brief The REPL (Read-Eval-Print Loop) of the shell.
 * * Continuously prompts the user for input, manages signal configurations 
 * for interactive mode, and directs the input to the processing engine.
 * Handles EOF (Ctrl+D) to exit the loop.
 * * @param shell Pointer to the main shell state structure.
 */
void shell_loop(t_shell *shell)
{
    char *line;

    while (shell->running)
    {
        if (shell->is_interactive)
            setup_signals_interactive();
        line = get_input_line(shell);
        if (!line)
        {
            if (shell->is_interactive)
                printf("exit\n");
            break;
        }
        if (!is_empty_or_whitespace(line))
        {
            if (shell->is_interactive)
                add_history(line);
            process_and_execute(line, shell);
        }
        free(line);
    }
}

/**
 * @brief Fetches a line of input from the user or script.
 * * Depending on the shell mode, it uses readline for interactive 
 * terminals or a non-interactive method (like get_next_line) for pipes.
 * Also handles the SIGINT signal state to update exit status.
 * * @param shell Pointer to the shell structure for status updates.
 * @return char* The input string, or NULL if EOF is reached.
 */
char *get_input_line(t_shell *shell)
{
    char *line;

    if (shell->is_interactive)
    {
        line = readline("minishell$ ");
        if (g_signal_received == SIGINT)
        {
            shell->last_exit_status = EXIT_SIGINT;
            g_signal_received = SUCCESS;
        }
    }
    else
        line = get_next_line_non_interactive();
    return (line);
}

/**
 * @brief Orchestrates the parsing, tree building, and execution.
 * * This function handles the full pipeline:
 * 1. Tokenization of the raw string.
 * 2. Token validation (syntax check).
 * 3. AST (Abstract Syntax Tree) construction.
 * 4. Signal setup for child processes.
 * 5. Execution and subsequent memory cleanup.
 * * @param line The raw input string to process.
 * @param shell Pointer to the shell state structure.
 */
void process_and_execute(char *line, t_shell *shell)
{
    t_token *head;
    t_ast   *root;

    head = tokenize(line);
    if (!head || validate_token_list(head) != 1)
    {
        if (!head)
            fprintf(stderr, "syntax error: Unclosed quotes\n");
        shell->last_exit_status = SYNTAX_ERROR;
        return (free_tokens(head));
    }
    root = build_complete_ast(head);
    if (shell->is_interactive)
        setup_signals_executing();
    execute_ast(root, shell);
    free_tree(root);
    free_tokens(head);
}
