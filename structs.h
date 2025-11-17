#ifndef STRUCTS_H
#define STRUCTS_H

//---------------------------------------------------LISTA DE TOKENS--------------------------------------------------------
typedef enum e_token_type
{
	WORD,        // texto normal: comando, argumento, nome de arquivo
	PIPE,        // |
	REDIR_IN,    // <
	REDIR_OUT,   // >
	APPEND,      // >>
	HEREDOC,      // <<
	OR,
	AND,
	LPAREN,      // (
	RPAREN,      // )
	CMD, //este token_type e apenas utilizado em nodes, ate la todos os tokens que nao sao operadores nem redirecionamentos sao WORD
	SUBTREE, //()
}	t_token_type;

typedef struct s_token
{
	int is_operator;
	int	expandable; //0 - does not expand ,1 - expands
	char            *value;   // conteúdo do token \("cat", ">", etc.)
	t_token_type    type;     // tipo \(WORD, PIPE, etc.)
	struct s_token  *next;    // próximo token
}	t_token;
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------LISTA DE NODES-------------------------------------------------
// os nodes serao utilizados para criar a AST, um node tanto pode ser um comando como um operador(&&, |, ||)

// -------------------ESTRUTURA DA LISTA DE REDIRECTS-----------------------

typedef struct s_redir
{
	t_token_type	type; // Tipo de redireção , e do tipo token_type mas so vai assumir valores de redirecoes
	char			*file;   // Nome do arquivo ou delimitador \(no caso de heredoc)
	int				file_name_is_expandable;
	struct s_redir	*next; // Próxima redireção
}	t_redir;

//------------------ESTRUTURA DA LISTA DE ARGUMENTOS---------------------

typedef struct s_arg
{
	char			*value;   // Nome do arquivo ou delimitador \(no caso de heredoc)
	int				is_expandable;
	struct s_arg	*next; // Próxima redireção
}	t_arg;

//-------------------ESTRUTURA DO COMANDO----------------------------------------
typedef struct s_cmd
{
	char        *cmd_name;   // "ls"
	t_arg        *args;      // e' uma linked list para poder conter a informacao se e expandivel ou nao,
							//depois de passar pela expansao de variaveis e antes de entrar no execve e'tranformado num array de args ["ls", "-l", NULL]
	t_redir     *redirs;     // lista ligada de redireções
}	t_cmd;

//-------------------ESTRUTURA DO NODE--------------------------------------------

typedef struct s_ast
{
	t_token_type type;    // tipo do nó \(comando, pipe, etc) usa o enum t_token_type mas apenas assume valores de operadores (e nao de redirecionamentos)
	struct s_ast *left;
	struct s_ast *right;
	int is_expandable;

	t_cmd *cmd;          // ponteiro para a estrutura do comando (se for um comando, caso seja um operador aponta para NULL)
}	t_ast;
//---------------------------------------------------------------------------------
#endif


