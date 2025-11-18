
#ifndef MINISHELL_H
#define MINISHELL_H


#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include "Libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>

# include <unistd.h>



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
size_t  get_quoted_size(char *line, char quote);
char    *get_quoted_text(char *line, char quote);
void    create_quoted_token(t_token **last_token, t_token **head, char *line, char quote);

/* ---------------- Funções para operadores ---------------- */
void    handle_pipe_or_or(char *line, int *i, t_token **last_token, t_token **head);
void    handle_and(char *line, int *i, t_token **last_token, t_token **head);
void    handle_redap_or_redout(char *line, int *i, t_token **last_token, t_token **head);
void    handle_redin_or_heredoc(char *line, int *i, t_token **last_token, t_token **head);
void	handle_parentesis(char *line, int *i, t_token **last_token, t_token **head);

/* ---------------- Funções principais ---------------- */
void    handle_quote(char *line, int *i, t_token **last_token, t_token **head);
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
