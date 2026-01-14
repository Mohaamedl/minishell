/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:31:25 by mhaddadi          #+#    #+#             */
/*   Updated: 2026/01/14 11:20:30 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

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
        line = get_next_line_non_interactive(); // Encapsula o getline aqui
    return (line);
}

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
