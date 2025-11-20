#include "minishell.h"

void handle_invalid_redirections(t_token *head) //preciso de dar free a line
{
    free_tokens(head);
    //free(line);
    fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
    exit(2); // nao prosso encerrar o programa, aqui serve apenas para testar
}

void validate_redirections(t_token *head)
{
    t_token *tmp;

    tmp = head;
    while(tmp)
    {
        if(is_redirect_token(tmp))//se for um redirect token tem que ter um file name associado no caso do heredoc tem de ter um Terminador
        {
            if(tmp->next==NULL|| tmp->next->type != WORD)
                handle_invalid_redirections(head);
        }
        tmp = tmp->next;
    }
}
void handle_unclosed_quotes(t_token *head) //preciso de dar free a line
{
    free_tokens(head);
    //free(line);
    fprintf(stderr, "Error: unclosed quote\n");
    exit(2); // ou o código que preferires
}
void validate_token_list(t_token *head)
{
    //validate_parentesis();//percorre a lista e verifica se os parentesis fecham e nao estao vazios
    //validate_operators();//valida se operadores tem sempre im comando antes e um comando a fernte nao pode haver operadores juntos
    validate_redirections(head);//valida se as redirecoes tem sempre um file name a frente 
}