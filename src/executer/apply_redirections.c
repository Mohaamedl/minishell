#include "minishell.h"

	//REDIR_IN,    // <
	//REDIR_OUT,   // >
	//APPEND,      // >>
	//HEREDOC,      // <<

void 	apply_redir_in(t_redir *redir)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	//lidar com erro de nao encontrar o file ou nao ter permissao
	dup2(fd, STDIN_FILENO);
	close(fd);
}
void	apply_redir_app(t_redir *redir)
{
	int fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644); 
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
void	apply_redir_out(t_redir *redir)
{
	int fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
void	apply_redirections(t_ast *cmd_node)
{
	t_redir *curr_redir = cmd_node -> cmd ->redirs;
	while(curr_redir)
	{
		if(curr_redir ->type == REDIR_OUT)
			apply_redir_out(curr_redir);
		if(curr_redir ->type == APPEND)
			apply_redir_app(curr_redir);
		if(curr_redir ->type == REDIR_IN)
			apply_redir_in(curr_redir);
		curr_redir = curr_redir -> next;
	}
}
