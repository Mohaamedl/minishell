/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddadi <mhaddadi@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:26:21 by mhaddadi          #+#    #+#             */
/*   Updated: 2025/11/08 14:26:40 by mhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <limits.h>
# include "../structs.h"
# include "../Libft/libft.h"

/*
** ============================================================================
** MACROS AND CONSTANTS
** ============================================================================
*/

# define SUCCESS 0
# define ERROR 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126

# define STDIN 0
# define STDOUT 1
# define STDERR 2

/*
** ============================================================================
** DATA STRUCTURES
** ============================================================================
*/

/**
 * @brief Environment variable node
 * 
 * Linked list structure to store environment variables.
 * Each node contains a key-value pair.
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/**
 * @brief Shell state structure
 * 
 * Maintains the overall state of the minishell including
 * environment variables and exit status.
 */
typedef struct s_shell
{
	t_env	*env_list;
	int		last_exit_status;
	int		running;
}	t_shell;

/*
** ============================================================================
** GLOBAL VARIABLES
** ============================================================================
*/

extern volatile sig_atomic_t	g_signal_received;

/*
** ============================================================================
** SIGNAL HANDLING
** ============================================================================
*/

/* signals.c */
void	setup_signals_interactive(void);
void	setup_signals_executing(void);
void	setup_signals_default(void);

/*
** ============================================================================
** SHELL INITIALIZATION & CLEANUP
** ============================================================================
*/

/* init.c */
t_shell	*init_shell(char **envp);

/* cleanup.c */
void	cleanup_shell(t_shell *shell);

/*
** ============================================================================
** ENVIRONMENT MANAGEMENT
** ============================================================================
*/

/* env_init.c */
t_env	*init_env(char **envp);
void	free_env(t_env *env);

/* env_get.c */
char	*get_env_value(t_env *env, const char *key);
t_env	*find_env_node(t_env *env, const char *key);

/* env_set.c */
int		set_env_value(t_env **env, const char *key, const char *value);
int		unset_env_value(t_env **env, const char *key);

/* env_utils.c */
char	**env_list_to_array(t_env *env);
void	free_env_array(char **env_array);
int		env_list_size(t_env *env);
void	print_env_list(t_env *env);

/*
** ============================================================================
** BUILT-IN COMMANDS
** ============================================================================
*/

/* echo.c */
int		builtin_echo(char **args);

/* cd.c */
int		builtin_cd(char **args, t_env **env);

/* pwd.c */
int		builtin_pwd(void);

/* export.c */
int		builtin_export(char **args, t_env **env);

/* unset.c */
int		builtin_unset(char **args, t_env **env);

/* env.c */
int		builtin_env(t_env *env);

/* exit.c */
int		builtin_exit(char **args, int last_exit_status, t_shell *shell);

/* builtin_utils.c */
int		is_builtin(const char *cmd);
int		execute_builtin(char **args, t_shell *shell);
int		is_valid_identifier(const char *str);
int		count_args(char **args);

/*
** ============================================================================
** UTILITY FUNCTIONS
** ============================================================================
*/

/* string_utils.c */
char	*ft_strdup(const char *s);
char	*ft_strjoin(const char *s1, const char *s2);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(const char *s, char c);

/* string_utils2.c */
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);

/* memory_utils.c */
void	*ft_calloc(size_t count, size_t size);
void	*ft_memset(void *b, int c, size_t len);
void	free_array(char **arr);

/* number_utils.c */
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
int		is_numeric_string(const char *str);

/* error.c */
void	print_error(const char *cmd, const char *arg, const char *msg);
void	print_error_prefix(const char *cmd);

/*
** ============================================================================
** TOKENIZER & PARSER
** ============================================================================
*/

/* Tokenizer */
t_token	*tokenize(char *input);
void	free_tokens(t_token *head);
void	print_tokens(t_token *tmp);
t_token	*create_token(char *value, t_token_type type, int is_expandable, int is_operator);
void	append_token(t_token **head, t_token **last_token, t_token *new_token);
int		is_space(char c);
int		is_quote(char c);
int		is_single_quote(char c);
int		is_double_quote(char c);
int		is_operator(char *s);
void	handle_quote(char *line, int *i, t_token **last_token, t_token **head);
void	handle_word(char *line, int *i, t_token **last_token, t_token **head);
void	handle_pipe_or_or(char *line, int *i, t_token **last_token, t_token **head);
void	handle_and(char *line, int *i, t_token **last_token, t_token **head);
void	handle_redin_or_heredoc(char *line, int *i, t_token **last_token, t_token **head);
void	handle_redap_or_redout(char *line, int *i, t_token **last_token, t_token **head);
void	handle_parentesis(char *line, int *i, t_token **last_token, t_token **head);
size_t	get_quoted_size(char *line, char quote);
char	*get_quoted_text(char *line, char quote);
void	create_quoted_token(t_token **last_token, t_token **head, char *line, char quote);

/* AST Building */
t_ast	*build_cmds_and_ops_list(t_token *head);
t_ast	*build_tree(t_ast *start_node, t_ast *end_node);
void	build_sub_trees(t_ast **root_node);
t_ast	*get_last_node(t_ast *head);
t_ast	*create_node(t_token *token, t_cmd *cmd);
void	create_cmd_node(t_token **tmp_token, t_token *head, t_ast **first_node, t_ast **last_node);
void	create_op_node(t_token **tmp_token, t_token *head, t_ast **first_node, t_ast **last_node);
void	append_node(t_ast *new_node, t_ast *last_node);

/* AST Debug */
void	print_tree(t_ast *root);
void	print_tree_node(t_ast *node);

/* AST Memory */
void	free_tree(t_ast *root);
void	free_node(t_ast *node);

/*
** ============================================================================
** AST EXECUTION
** ============================================================================
*/

/* execute_ast.c */
int		execute_ast(t_ast *node, t_shell *shell);

/*
** ============================================================================
** SIMPLE PARSER (TEMPORARY FOR PHASE 3 TESTING)
** ============================================================================
*/

/* parser_simple.c */
char	**simple_parse(char *line);
int		execute_command(char **args, t_shell *shell);

#endif
