void validate_token_list(t_token *head)
{
    validate_parentesis()//percorre a lista e verifica se os parentesis fecham e nao estao vazios
    validate_operators()//valida se operadores tem sempre im comando antes e um comando a fernte nao pode haver operadores juntos
    validate_redirections()//valida se as redirecoes tem sempre um file name a frente 
}