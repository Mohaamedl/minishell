#include "minishell.h"

	//REDIR_IN,    // <
	//REDIR_OUT,   // >
	//APPEND,      // >>
	//HEREDOC,      // <<

void	apply_redir_out(t_redir *redir)
{
	int fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
void	apply_redirections(t_ast *cmd_node)
{
	t_redir *tmp_redir = cmd_node -> cmd ->redirs;
	while(tmp_redir)
	{
		if(tmp_redir ->type == REDIR_OUT)
			apply_redir_out(tmp_redir);
		tmp_redir = tmp_redir -> next;
	}
}
