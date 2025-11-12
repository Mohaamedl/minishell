/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: framiran <framiran@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:44:03 by framiran          #+#    #+#             */
/*   Updated: 2025/11/12 18:34:13 by framiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//esta funcao recebe o node com o primeiro parentesis e vai saltar para o ultimo parentesis, a funcao get_OP_node_based_on_type()
//depois avalia se este e o ultimo node ou nao se for retorna null (nao encontrou operador) caso nao seja o ultimo passa para a frente
t_ast	*skip_subtree_nodes(t_ast *tmp_node)
{
	while(tmp_node)
	{
		if(tmp_node -> type == RPAREN)
			return (tmp_node);
		tmp_node = tmp_node -> right;
	}
	return (NULL);
	//se chegar aqui e porque nao encontrou a o parentesis que fecha ")", vou ter que pensar no que fazer neste caso
}

t_ast *get_OP_node_based_on_type(t_ast *start_node, t_ast *end_node, t_token_type type)
{
	t_ast *tmp_node;

	tmp_node = start_node;
	while(tmp_node)
	{
		if(tmp_node ->type == LPAREN)
			tmp_node = skip_subtree_nodes(tmp_node);
		if (tmp_node ->type == type)
			return tmp_node;
		if(tmp_node == end_node)//tinha chegado ao fim e ja avaliei este node nas linhas acima entao dou break do ciclo
			break;
		tmp_node = tmp_node -> right;
	}
	return (NULL);
}

t_ast *get_split_op_node(t_ast *start_node, t_ast *end_node)
{
	t_ast *tmp_node;

	tmp_node = NULL;
	tmp_node = get_OP_node_based_on_type(start_node, end_node, OR);
	if (tmp_node != NULL)
		return tmp_node;
	else
	{
		tmp_node = get_OP_node_based_on_type(start_node, end_node, AND);
		if (tmp_node != NULL)
			return tmp_node;
		else
		{
			tmp_node = get_OP_node_based_on_type(start_node, end_node, PIPE);
			if (tmp_node != NULL)
				return tmp_node;
		}
	}
	return NULL;
}
//Quando ja tiver retornado a arvore principal(sem considerar as subtrees) e s'o me sobrarem "folhas"(nos isolados) sejam comandos ou subtrees
//vou percorrer a arvore e se encontrar uma subtree chamo esta funcao para o conteudo que esta dentro de parentesis ou seja
// start_node e o node imediatamente a frente de "("  e end node e o node que esta imediatamente atras de ")"
//agora desta vez nao tenho parentesis na lista de nodes a analizar e get_OP_node_based_on_type() nao os vai pular e vai considerar
//o que esta la dentro
t_ast *build_tree(t_ast *start_node, t_ast *end_node)
{
	t_ast *tmp_node;
	t_ast *original_start_node;
	t_ast *original_end_node;

	original_start_node = start_node;
	original_end_node = end_node;
	tmp_node = get_split_op_node(start_node, end_node);// esta funcao retorna um ponteiro para um OR ou AND ou PIPE ou NULL caso nao encontre avaliando sempre por esta ordem
	if (tmp_node != NULL) //Esta funcao retorna um ponteiro para um node OR case encontre, caso contrario retorna NULL
	{
		end_node = tmp_node -> left;
		tmp_node -> left = build_tree(original_start_node, end_node);
		start_node = tmp_node -> right;
		tmp_node ->right =  build_tree(start_node, original_end_node);
		return (tmp_node);
	}
	else //cheguei a um comando isolado ou a uma subtree isolada
		return start_node;
}
//se o root node for uma subtree o pointer vai mudar ex. (a && b)
//antes apontava para o primeiro parentesis, agora vai apontar para &&;
void	build_sub_trees(t_ast **root_node)
{
	t_ast *start_node;
	t_ast *end_node;
	//percorro a arvore, se encontrar uma subtree, chamo a funcao build tree ,
	//start_node e o node imediatamente a frente de "("  e end node e o node que esta imediatamente atras de ")"
	if((*root_node) ->type == CMD || (*root_node )-> type == LPAREN) //encontrei um comando ou uma subtree;
	{
		if((*root_node )-> type == LPAREN) // no caso de uma subtree, independentemente de ela estar a direita ou esquerda do split_operator o root node e o parentesis esquerdo "("
		{
			//ok, e uma subtree, este node vai se transformar no output de build_tree()
			start_node = (*root_node) -> right;//o node imediatamente apos  "("
			end_node = skip_subtree_nodes(*root_node);
			end_node = end_node -> left; //o node imediatamente antes de ")" (uma das vantagens desta tree ter comecado como uma lista duplamente ligada)
			*root_node = build_tree(start_node, end_node);
		}
		return;
	}
	else //estou num operador;
	{
		build_sub_trees(&((*root_node)->left));
		build_sub_trees(&((*root_node)->right));
	}
}

//dou skip ao subtree nodes mas a condicao de paragem de ser igual ao end_node nao se verifica porque eu mando dois nodes iguais (root_node)
//ver se tenho que deslinkar a double link que tenho


