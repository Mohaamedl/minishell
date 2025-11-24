
#ifndef MINISHELL_H
#define MINISHELL_H

#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "Libft/libft.h"

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
** DATA STRUCTURES (from old minishell.h)
** ============================================================================
*/

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

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

void	setup_signals_interactive(void);
void	setup_signals_executing(void);
void	setup_signals_default(void);

/*
** ============================================================================
** SHELL INITIALIZATION & CLEANUP
** ============================================================================
*/

t_shell	*init_shell(char **envp);
void	cleanup_shell(t_shell *shell);

/*
** ============================================================================
** ENVIRONMENT MANAGEMENT
** ============================================================================
*/

t_env	*init_env(char **envp);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, const char *key);
t_env	*find_env_node(t_env *env, const char *key);
int		set_env_value(t_env **env, const char *key, const char *value);
int		unset_env_value(t_env **env, const char *key);
char	**env_list_to_array(t_env *env);
void	free_env_array(char **env_array);
int		env_list_size(t_env *env);
void	print_env_list(t_env *env);

/*
** ============================================================================
** BUILT-IN COMMANDS
** ============================================================================
*/

int		builtin_echo(char **args);
int		builtin_cd(char **args, t_env **env);
int		builtin_pwd(void);
int		builtin_export(char **args, t_env **env);
int		builtin_unset(char **args, t_env **env);
int		builtin_env(t_env *env);
int		builtin_exit(char **args, int last_exit_status, t_shell *shell);
int		is_builtin(const char *cmd);
int		execute_builtin(char **args, t_shell *shell);
int		is_valid_identifier(const char *str);
int		count_args(char **args);

/*
** ============================================================================
** UTILITY FUNCTIONS
** ============================================================================
*/

char	*ft_strdup(const char *s);
char	*ft_strjoin(const char *s1, const char *s2);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(const char *s, char c);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memset(void *b, int c, size_t len);
void	free_array(char **arr);
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
int		is_numeric_string(const char *str);
void	print_error(const char *cmd, const char *arg, const char *msg);
void	print_error_prefix(const char *cmd);


//----------------------------------------------------TOKENIZER-----------------------------------------------------------------------------

/* ---------------- Funções auxiliares ---------------- */
int     is_space(char c);
int     is_quote(char c);
int     is_single_quote(char c);
int     is_double_quote(char c);
int     is_operator(char *line);

/* ---------------- Funções de criação e manipulação de tokens ---------------- */
t_token *create_token(char *value, t_token_type type, int is_expandable, int is_op);
void    append_token(t_token **head, t_token **last_token, t_token *new_token);

/* ---------------- Funções para lidar com aspas ---------------- */
int  get_quoted_size(char *line, char quote);
char    *get_quoted_text(char *line, char quote);
int    create_quoted_token(t_token **last_token, t_token **head, char *line, char quote);

/* ---------------- Funções para operadores ---------------- */
void    handle_pipe_or_or(char *line, int *i, t_token **last_token, t_token **head);
void    handle_and(char *line, int *i, t_token **last_token, t_token **head);
void    handle_redap_or_redout(char *line, int *i, t_token **last_token, t_token **head);
void    handle_redin_or_heredoc(char *line, int *i, t_token **last_token, t_token **head);
void	handle_parentesis(char *line, int *i, t_token **last_token, t_token **head);

/* ---------------- Funções principais ---------------- */
int		handle_quote(char *line, int *i, t_token **last_token, t_token **head); //returns the status: 0-the quote did not close 1-sucess
void    handle_word(char *line, int *i, t_token **last_token, t_token **head);
void    handle_ops_and_reds(char *line, int *i, t_token **last_token, t_token **head);
void    skip_spaces(char *line, int *i);
t_token *tokenize(char *line);

/* ---------------- Funções de Memoria---------------- */
void	free_tokens(t_token *head);
/*------------------Funcoes Debug-------------------*/
void	print_tokens(t_token *tmp);
const char *token_type_to_str(t_token_type type);
void	print_tree_visual(t_ast *root);

/*-------------Funções para validar a token list-----*/
int	validate_token_list(t_token *head);
int	validate_redirections(t_token *head);
int validate_connector_operators(t_token *head);
int validate_parentesis(t_token *head);
//HELPERS
t_token *find_left_token(t_token *head,t_token *op_token);
int validate_right_and_left_tokens(t_token *left,t_token *right);
int is_connector_operator(t_token *token);
int	check_balance(t_token *head);

//-----------------------------------------------------------CRIACAO DA LISTA DE COMANDOS E OPERADORES---------------------------------------------------
//GERAIS
t_ast	*build_cmds_and_ops_list(t_token *head);
t_ast	*create_node(t_token *token, t_cmd* cmd);
void	append_node(t_ast *new_node,t_ast *last_node);

//ESPECIFICAS DO NODE COMANDO
void	create_cmd_node(t_token **tmp_token,t_token *head, t_ast **first_node, t_ast **last_node);
t_cmd	*create_cmd(t_token *tmp_token);
void	handle_arg_token(t_cmd *cmd, t_token **tmp_token, t_arg **arg_list_head, t_arg **arg_list_last);
void	handle_redirect_token(t_cmd *cmd,t_token **tmp_token, t_redir **redir_list_head, t_redir **redir_list_last);
void	update_token_to_eval(t_token **tmp_token);
int		calculate_numb_of_args(t_token *tmp_token);
int		is_redirect_token(t_token *tmp_token);

//ESPECIFICAS NODE OPERADOR
void	create_op_node(t_token **tmp_token, t_token *head, t_ast **first_node, t_ast **last_node);

//DEBUG
void	print_nodes(t_ast *node);
void	print_cmd(t_cmd *cmd);
void	print_redirs(t_redir *redir);
void	print_args(t_arg *arg);
//FUNCOES DE MEMORIA
void	free_redirects_list(t_redir *head);
void	free_args_list(t_arg *head);
void	free_cmd(t_cmd *cmd);
void	free_node(t_ast *node);
void	free_node_list(t_ast *head); //esta funcao serve apenas para dar free a uma lista de comandos e operadores,
									//e uma funcao temporaria, no futuro terei que a alterar para dar free a arvore

t_ast	*get_last_node(t_ast *head); //vai buscar o ultimo node da lista, preciso dele na funcao build_tree()


//----------------------------------------CRIACAO DA ARVORE BINARIA---------------------------------------------------

t_ast	*build_tree(t_ast *start_node, t_ast *end_node);
t_ast	*get_split_op_node(t_ast *start_node, t_ast *end_node);
t_ast	*get_OP_node_based_on_type(t_ast *start_node, t_ast *end_node, t_token_type type);
t_ast	*skip_subtree_nodes(t_ast *tmp_node);
void	build_sub_trees (t_ast **root_node);
//DEBUGGERS
void	print_tree_node(t_ast *node);
void	print_tree(t_ast *root);
//MEMORY
void	free_tree(t_ast *root);
void	free_parentesis_nodes(t_ast *start_node,t_ast *left_node);

//-----------------------------------------EXPANSAO DE VARIAVEIS--------------------------------------------------
//turn_args_list_into_args_array() depois de expandir vou ter que chamar esta funcao para ter o array de args pronto para o execve;
#endif
