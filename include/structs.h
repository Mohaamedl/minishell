/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 11:46:23 by framiran          #+#    #+#             */
/*   Updated: 2026/01/19 11:48:23 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <sys/types.h>

//---------------------LISTA DE TOKENS-------------
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	OR,
	AND,
	LPAREN,
	RPAREN,
	SEMICOLON,
	CMD,
}	t_token_type;

typedef struct s_token
{
	int				is_operator;
	int				expandable;
	int				is_quoted;
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;
//--------------------------------------------------

//--------------LISTA DE NODES-----------

// -----ESTRUTURA DA LISTA DE REDIRECTS--------

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				file_name_is_expandable;
	int				file_was_expanded;
	struct s_redir	*next;
}	t_redir;

//-----ESTRUTURA DA LISTA DE ARGUMENTOS-----

typedef struct s_arg
{
	char			*value;
	int				is_expandable;
	int				is_wildcard_expandable;
	int				was_expanded;
	struct s_arg	*next;
}	t_arg;

//------------ESTRUTURA DO COMANDO---------
typedef struct s_cmd
{
	char	*cmd_name;
	t_arg	*args;
	t_redir	*redirs;
}	t_cmd;

//----------ESTRUTURA DO NODE--------

typedef struct s_ast
{
	t_token_type	type;
	struct s_ast	*left;
	struct s_ast	*right;
	int				is_expandable;
	t_cmd			*cmd;
}	t_ast;
//-----------------------------------

typedef struct s_shell	t_shell;

//-----PIPELINE CONTEXT STRUCTURE----------
typedef struct s_pipe_ctx
{
	int			*pipes;
	pid_t		*pids;
	int			pipe_indice;
	int			numb_of_pipes;
	int			spawned;
	t_shell		*shell;
}	t_pipe_ctx;

#endif
